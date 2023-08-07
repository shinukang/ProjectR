// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRFirearm.h"
#include "Engine/ActorChannel.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerController.h"
#include "Character/Animation/PRCharacterAnimInstance.h"
#include "Library/PRItemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Component/PRBaseComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Component/PRInventoryComponent.h"
#include "Item/PRBullet.h"
#include "Item/PRScope.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APRFirearm::APRFirearm()
{
	bReplicates = true;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyMesh");
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BodyMesh;

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	MagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MagMesh->SetupAttachment(BodyMesh);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarrelMesh->SetupAttachment(BodyMesh);

	GripMesh = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	GripMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GripMesh->SetupAttachment(BodyMesh);

	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScopeMesh"));
	ScopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ADSCam = CreateDefaultSubobject<UCameraComponent>("ADSCam");
	ADSCam->SetupAttachment(ScopeMesh);
	ADSCam->SetFieldOfView(40.0f);

	ScopeCam = CreateDefaultSubobject<UCameraComponent>("ScopeCam");
	ScopeCam->SetupAttachment(ScopeMesh);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(ScopeCam);

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CamShake_Fire(TEXT("Blueprint'/Game/Character/CameraSystem/PRCameraShake_Fire.PRCameraShake_Fire_C'"));

	if(CamShake_Fire.Succeeded())
	{
		CameraShake_Fire = CamShake_Fire.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Data/DT_Ballistic.DT_Ballistic'"));

	if (DataTable.Succeeded())
	{
		BallisticDataTable = DataTable.Object;
	}
}

void APRFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APRFirearm, ItemData);
	DOREPLIFETIME(APRFirearm, FirearmData);
	DOREPLIFETIME(APRFirearm, LoadedBullets);
}

void APRFirearm::BeginPlay()
{
	Super::BeginPlay();

	if(APRPlayerController* PRPlayerController = Cast<APRPlayerController>(GetOwner()))
	{
		OnUpdateAttachment.AddUObject(PRPlayerController->HUD, &UPRWidgetBase::UpdateAttachment);

		if (UPRCharacterAnimInstance* PRAnimInstance = Cast<UPRCharacterAnimInstance>(PRPlayerController->PossessedCharacter->GetMesh()->GetAnimInstance()))
		{
			OnFire.BindUObject(PRAnimInstance, &UPRCharacterAnimInstance::SetRecoilAnimParams);
		}
	}
}

void APRFirearm::Init(FPRItemData NewItemData)
{
	ItemData = NewItemData;

	if (const FPRFirearmData* NewFirearmData = UPRItemLibrary::GetAdvancedData<FPRFirearmData>(ItemData.ID))
	{
		FirearmData = NewFirearmData;
	}
}

void APRFirearm::OnRep_ItemData()
{
	UE_LOG(LogTemp, Warning, TEXT("APRFirearm::OnRep_ItemData"));
}

void APRFirearm::OnRep_FirearmData()
{
	UE_LOG(LogTemp, Warning, TEXT("APRFirearm::OnRep_FirearmData"));

	BodyMesh->SetSkeletalMeshAsset(FirearmData.BodyMesh);
	MagMesh->SetStaticMesh(FirearmData.MagMesh);
	MagMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Mag_Socket")));
	BarrelMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Barrel")));
	BarrelMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	GripMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Grip")));
	Client_SetScope(NAME_None);
}

void APRFirearm::StartFire()
{
	bIsFiring = true;
	Fire();
}

void APRFirearm::Fire()
{
	if (bIsFiring)
	{
		if(LoadedBullets > 0)
		{
			FVector CamLoc = FVector::ZeroVector; // Player Camera Location
			FRotator CamRot = FRotator::ZeroRotator; // Player Camera Rotation

			if(APRPlayerController* PlayerController = Cast<APRPlayerController>(GetOwner()))
			{
				if (APlayerCameraManager* ThirdCamera = PlayerController->PlayerCameraManager)
				{
					ThirdCamera->GetCameraViewPoint(CamLoc, CamRot);

					if(GripMesh->GetStaticMesh())
					{
						ThirdCamera->StartCameraShake(CameraShake_Fire);
					}
					else
					{
						ThirdCamera->StartCameraShake(CameraShake_Fire, 1.5f);
					}

					if (!bIsOnADS)
					{
						OnFire.Execute(0.05f, 1.0f, 0.0f);
					}
				}
				float RandYaw = UKismetMathLibrary::RandomFloatInRange(-0.05f, 0.05f);
				float RandPitch = UKismetMathLibrary::RandomFloatInRange(-0.15f, 0.0f);
				PlayerController->AddYawInput(RandYaw);
				PlayerController->AddPitchInput(RandPitch);
			}

			TArray<AActor*> ActorsToIgnore = { this, GetOwner(), GetAttachParentActor() };

			FHitResult HitResult;

			FVector SpawnLocation = BodyMesh->GetSocketLocation(FName(TEXT("Barrel")));
			FVector Destination = CamLoc + FirearmData.EffectiveRange * CamRot.Vector();
			FRotator SpawnRotation = FRotationMatrix::MakeFromX(Destination - SpawnLocation).Rotator();
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

			if(GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + CamRot.Vector() * FirearmData.EffectiveRange, ECollisionChannel::ECC_Visibility, CollisionQueryParams))
			{
				SpawnRotation = (HitResult.Location - SpawnLocation).Rotation();
			}

			Server_Fire(SpawnLocation, SpawnRotation, HitResult);

			if(ItemData.Category == EPRCategory::Firearm_AssaultRifle || ItemData.Category == EPRCategory::Firearm_SubMachineGun)
			{
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APRFirearm::Fire, FirearmData.FireRate, false);
			}
		}
		else
		{
			if(FirearmData.SFX_Dry)
			{
				UGameplayStatics::SpawnSoundAttached(FirearmData.SFX_Dry, BodyMesh, FName(TEXT("Barrel")));
			}
		}
	}
}

void APRFirearm::StopFire()
{
	bIsFiring = false;
}

void APRFirearm::Server_Fire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FHitResult TargetHitResult)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APlayerController>(GetOwner())->GetPawn();
	SpawnParameters.Owner = this;

	int32 Repeat = 1;

	if(ItemData.Category == EPRCategory::Firearm_Shotgun)
	{
		Repeat = 12;
	}

	for (int32 i = 0; i < Repeat; i++)
	{
		FRotator AdditiveRotation = FRotator(FMath::RandRange(-FirearmData.SpreadRate, FirearmData.SpreadRate), FMath::RandRange(-FirearmData.SpreadRate, FirearmData.SpreadRate), FMath::RandRange(-FirearmData.SpreadRate, FirearmData.SpreadRate));

		SpawnRotation += AdditiveRotation;

		if (TargetHitResult.Distance < 1000.0f)
		{
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.bReturnPhysicalMaterial = true;

			if (GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, SpawnLocation + SpawnRotation.Vector() * FirearmData.EffectiveRange, ECC_Visibility, Params))
			{
				Multicast_ApplyDamage(HitResult);
			}
		}
		else
		{
			GetWorld()->SpawnActor<APRBullet>(SpawnLocation, SpawnRotation, SpawnParameters);
		}
	}
	LoadedBullets--;
	Multicast_SpawnEffects();
}

void APRFirearm::ApplyDamage(FHitResult HitResult)
{
	UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), FirearmData.Damage, HitResult.ImpactNormal, HitResult, GetInstigator()->Controller, this, UDamageType::StaticClass());

	if (const UPhysicalMaterial* HitPhysicalMat = HitResult.PhysMaterial.Get())
	{
		const FName ID = FName(StaticEnum<EPhysicalSurface>()->GetValueAsString(HitPhysicalMat->SurfaceType));

		if (const FPRBallisticFX* BallisticFX = BallisticDataTable->FindRow<FPRBallisticFX>(ID, FString(TEXT(""))))
		{
			if (BallisticFX->VFX_Ballistic)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BallisticFX->VFX_Ballistic, HitResult.Location, HitResult.Normal.Rotation() - FRotator(90.0f, 0.0f, 0.0f));
			}

			if (BallisticFX->SFX_Ballistic)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BallisticFX->SFX_Ballistic, HitResult.Location);
			}
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("APRBullet::OnHit : No Ballistic Data."));
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("APRBullet::OnHit : No Physical Material."));
	}
}

void APRFirearm::Multicast_ApplyDamage_Implementation(FHitResult HitResult)
{
	ApplyDamage(HitResult);
}


void APRFirearm::Multicast_SpawnEffects_Implementation()
{
	if (FirearmData.VFX_Fire)
	{
		if(BarrelMesh->GetStaticMesh())
		{
			UGameplayStatics::SpawnEmitterAttached(FirearmData.VFX_Fire, BarrelMesh, FName(TEXT("Barrel")), FVector(0.0f), FRotator(0.0f), EAttachLocation::Type::SnapToTargetIncludingScale, true);
		}
		else
		{
			UGameplayStatics::SpawnEmitterAttached(FirearmData.VFX_Fire, BodyMesh, FName(TEXT("Barrel")), FVector(0.0f), FRotator(0.0f), EAttachLocation::Type::KeepRelativeOffset, true);
		}
	}

	if(FirearmData.SFX_Fire && FirearmData.SFX_Fire_Suppressed)
	{
		if(FirearmData.bSuppressorAttached)
		{
			UGameplayStatics::SpawnSoundAttached(FirearmData.SFX_Fire_Suppressed, BodyMesh, FName(TEXT("Barrel")));
		}
		else
		{
			UGameplayStatics::SpawnSoundAttached(FirearmData.SFX_Fire, BodyMesh, FName(TEXT("Barrel")));
		}
	}

	if(FirearmData.VFX_Ejection)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(FirearmData.VFX_Ejection, BodyMesh, FName(TEXT("Ejection")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}
}

FPRItemData* APRFirearm::CheckIsBulletsInInventory()
{
	if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetAttachParentActor()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		if (FPRItemData* InventoryItem = InventoryComponent->GetInventoryItem(FirearmData.AmmunitionID))
		{
			return InventoryItem;
		}
	}
	return nullptr;
}

void APRFirearm::Reload()
{
	Server_Reload();
}

void APRFirearm::Server_Reload_Implementation()
{
	if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetAttachParentActor()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		if (FPRItemData* BulletData = InventoryComponent->GetInventoryItem(FirearmData.AmmunitionID))
		{
			const int32 TotalBullets = BulletData->Amount;
			const int32 BulletsForReload = FirearmData.BulletsPerMag - LoadedBullets;

			if (BulletsForReload >= TotalBullets)
			{
				LoadedBullets += TotalBullets;
				InventoryComponent->TryRemoveFromInventory(FPRItemData(FirearmData.AmmunitionID, TotalBullets));
			}
			else
			{
				LoadedBullets += BulletsForReload;
				InventoryComponent->TryRemoveFromInventory(FPRItemData(FirearmData.AmmunitionID, BulletsForReload));
			}
		}
	}
}


void APRFirearm::Client_SetScope_Implementation(FName ID)
{
	UE_LOG(LogTemp, Warning, TEXT("APRFirearm::Client_SetScope : ID = %s"), *ID.ToString());

	if(UPRItemLibrary::IsValidItemID(ID))
	{
		ScopeMesh->SetStaticMesh(UPRItemLibrary::GetBaseData(ID).Mesh);
		ScopeMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Scope")));
		ADSCam->AttachToComponent(ScopeMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("ADS")));
		ScopeCam->AttachToComponent(ScopeMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Camera")));

		if (FPRScopeData* ScopeData = UPRItemLibrary::GetAdvancedData<FPRScopeData>(ID))
		{
			if (ScopeData->bNeedRenderTarget)
			{
				if (UTextureRenderTarget2D* NewTextureRenderTarget = NewObject<UTextureRenderTarget2D>())
				{
					NewTextureRenderTarget->InitAutoFormat(1024, 1024);
					SceneCapture->TextureTarget = NewTextureRenderTarget;
					SceneCapture->FOVAngle = ScopeData->FOV;
					ScopeMaterial = ScopeMesh->CreateDynamicMaterialInstance(1);
					ScopeMaterial->SetTextureParameterValue(FName(TEXT("RenderTarget")), NewTextureRenderTarget);
					return;
				}
			}
			else
			{
				ScopeMesh->SetMaterial(1, nullptr);
				ScopeMaterial = nullptr;
				return;
			}
		}
		return;
	}

	if (FirearmData.bNeedBaseScope)
	{
		ScopeMesh->SetStaticMesh(FirearmData.BaseScopeMesh);
		ScopeMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Scope")));
		ADSCam->AttachToComponent(ScopeMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("ADS")));
	}
	else
	{
		ScopeMesh->SetStaticMesh(nullptr);
		ScopeMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("ADS")));
		ADSCam->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("ADS")));
	}
	
}

void APRFirearm::Zoom(bool bIn)
{
	if(ScopeMaterial)
	{
		if (const FPRScopeData* ScopeData = UPRItemLibrary::GetAdvancedData<FPRScopeData>(Attachments[EPRCategory::Attachment_Scope]))
		{
			const float MaxFOV = 20.0f;
			const float MinFOV = ScopeData->FOV;
			const float DeltaFOV = (MaxFOV - MinFOV) / 10.0f;

			float NewFOV = 0.0f;

			if (bIn)
			{
				NewFOV = SceneCapture->FOVAngle - DeltaFOV;
			}
			else
			{
				NewFOV = SceneCapture->FOVAngle + DeltaFOV;
			}

			SceneCapture->FOVAngle = FMath::Clamp(NewFOV, MinFOV, MaxFOV);
		}
	}
}


void APRFirearm::Server_AddAttachment_Implementation(EPRCategory Category, FName ID, APRItem* NewAttachmentItem)
{
	UE_LOG(LogTemp, Warning, TEXT("APRFirearm::Server_AddAttachment"));

	if (APRCharacter* Character = Cast<APRCharacter>(GetAttachParentActor()))
	{
		// 아이템이 인벤토리에 존재한다면, 삭제하고
		if (Character->PRInventoryComponent->GetInventoryItem(FPRItemData(ID)))
		{
			UE_LOG(LogTemp, Warning, TEXT("APRFirearm::Server_UpdateAttachment : Attachment in Inventory"));
			Character->PRInventoryComponent->Server_RemoveFromInventory(FPRItemData(ID), false);
		}
		// 존재하지 않는다면, 바닥에 있는지 확인한다
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APRFirearm::Server_UpdateAttachment : Attachment not in Inventory"));
			if(NewAttachmentItem)
			{
				NewAttachmentItem->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("APRFirearm::Server_UpdateAttachment : Attachment on Ground"));
			}
		}
		Multicast_UpdateAttachment(ItemData.Index, Category, ID);
	}
}

void APRFirearm::Server_RemoveAttachment_Implementation(EPRCategory Category, bool bNeedToSpawn)
{
	if(const FName* ID = Attachments.Find(Category))
	{
		if(bNeedToSpawn)
		{
			if (APRItem* NewItem = GetWorld()->SpawnActor<APRItem>())
			{
				NewItem->Init(FPRItemData(*ID));
				Multicast_UpdateAttachment(ItemData.Index, Category, NAME_None);
			}
		}
		else
		{
			if (APRCharacter* Character = Cast<APRCharacter>(GetAttachParentActor()))
			{
				if(Character->PRInventoryComponent->TryAddItemToInventory(FPRItemData(*ID)))
				{
					Multicast_UpdateAttachment(ItemData.Index, Category, NAME_None);
				}
			}
		}
	}
}

void APRFirearm::Multicast_UpdateAttachment_Implementation(int32 Index, EPRCategory Category, FName ID)
{
	Attachments.Add(Category, ID);
	OnUpdateAttachment.Broadcast(Index, Category, ID);

	UStaticMesh* AttachmentMesh = UPRItemLibrary::GetBaseData(ID).Mesh;

	switch (Category)
	{
	case EPRCategory::Attachment_Barrel:
		BarrelMesh->SetStaticMesh(AttachmentMesh);

		FirearmData.bSuppressorAttached = false;

		if (const FPRAttachmentData* AttachmentData = UPRItemLibrary::GetAdvancedData<FPRAttachmentData>(ID, Category))
		{
			FirearmData.bSuppressorAttached = AttachmentData->Tags.Contains(FString(TEXT("Suppressor")));
		}

		break;

	case EPRCategory::Attachment_Grip:
		GripMesh->SetStaticMesh(AttachmentMesh);
		break;

	case EPRCategory::Attachment_Scope:
		Client_SetScope(ID);
		break;

	default:
		break;
	}
}
