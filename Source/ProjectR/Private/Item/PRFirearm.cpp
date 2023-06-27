// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRFirearm.h"
#include "Engine/ActorChannel.h"
#include "Character/PRCharacter.h"
#include "Character/PRPlayerController.h"
#include "Library/PRItemLibrary.h"
#include "Library/RyanLibrary.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Component/PRInventoryComponent.h"
#include "Item/PRBullet.h"
#include "Item/PRScope.h"
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

	if(APRPlayerController* PlayerController = Cast<APRPlayerController>(GetOwner()))
	{
		OnUpdateAttachment.AddUObject(PlayerController->HUD, &UPRWidgetBase::UpdateAttachment);
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

	if (APRCharacter* OwnerCharacter = Cast<APRCharacter>(GetAttachParentActor()))
	{
		OwnerCharacter->PRInventoryComponent->OnUpdateFirearms.Broadcast(OwnerCharacter->PRInventoryComponent->Firearms);
	}
}

void APRFirearm::OnRep_FirearmData()
{
	UE_LOG(LogTemp, Warning, TEXT("APRFirearm::OnRep_FirearmData"));

	BodyMesh->SetSkeletalMeshAsset(FirearmData.BodyMesh);
	MagMesh->SetStaticMesh(FirearmData.MagMesh);
	MagMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Mag_Socket")));
	BarrelMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Barrel")));
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
			FVector CamLoc; // Player Camera Location
			FRotator CamRot; // Player Camera Rotation

			if(bIsOnADS)
			{
				UE_LOG(LogTemp, Warning, TEXT("ADS"));
				CamLoc = ADSCam->GetComponentLocation();
				CamRot = ADSCam->GetComponentRotation();

				DrawDebugLine(GetWorld(), CamLoc, CamLoc + FirearmData.EffectiveRange * CamRot.Vector(), FColor::Red, true, 5.0f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Third"));
				if (const APlayerCameraManager* ThirdCamera = Cast<APlayerController>(GetOwner())->PlayerCameraManager)
				{
					ThirdCamera->GetCameraViewPoint(CamLoc, CamRot);
				}
			}
			TArray<AActor*> ActorsToIgnore = { this, GetOwner(), GetAttachParentActor() };

			FHitResult HitResult;
			FVector SpawnLocation = BodyMesh->GetSocketLocation(FName(TEXT("Barrel")));
			FVector Destination = CamLoc + FirearmData.EffectiveRange * CamRot.Vector();
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

		
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + FirearmData.EffectiveRange * CamRot.Vector(), ECollisionChannel::ECC_Visibility, CollisionQueryParams))
			{
				Destination = HitResult.ImpactPoint;
				UE_LOG(LogTemp, Warning, TEXT("HitActor = %s"), *HitResult.GetActor()->GetName());
			}

			DrawDebugSphere(GetWorld(), Destination, 10.0f, 10, FColor::Red, false, 5.0f);

			FRotator SpawnRotation = FRotationMatrix::MakeFromX(Destination - SpawnLocation).Rotator();
			Server_Fire(SpawnLocation, SpawnRotation);
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APRFirearm::Fire, FirearmData.FireRate, false);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("No Ammo"));
			
		}
	}
}

void APRFirearm::StopFire()
{
	bIsFiring = false;
}

void APRFirearm::Server_Fire_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APlayerController>(GetOwner())->GetPawn();
	SpawnParameters.Owner = GetOwner();

	//DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + SpawnRotation.Vector() * FirearmData.EffectiveRange, FColor::Red, true, 5.0f);

	if(APRBullet* Bullet = GetWorld()->SpawnActor<APRBullet>(SpawnLocation, SpawnRotation, SpawnParameters))
	{
		Bullet->Init(FirearmData.Damage);
		LoadedBullets--;
	}
}

void APRFirearm::Reload()
{
	Server_Reload();
}

void APRFirearm::Server_Reload_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server_Reload"));

	if (UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetAttachParentActor()->GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		if (FPRItemData* InventoryItem = InventoryComponent->GetInventoryItem(FirearmData.AmmunitionID))
		{
			int32 MaxBullets = InventoryItem->Amount;
			int32 BulletsForReload = FirearmData.BulletsPerMag - LoadedBullets;

			if (BulletsForReload >= MaxBullets)
			{
				LoadedBullets += MaxBullets;
				InventoryItem->Amount = 0;
			}
			else
			{
				LoadedBullets += BulletsForReload;
				InventoryItem->Amount -= BulletsForReload;
			}
			UE_LOG(LogTemp, Warning, TEXT("LoadedBullet = %d, InventoryBullet = %d"), LoadedBullets, InventoryItem->Amount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There's no proper bullet in your inventory"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is invalid"));
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
					UMaterialInstanceDynamic* MID_Scope_Sight = ScopeMesh->CreateDynamicMaterialInstance(1);
					MID_Scope_Sight->SetTextureParameterValue(FName(TEXT("RenderTarget")), NewTextureRenderTarget);
					return;
				}
			}
			else
			{
				ScopeMesh->SetMaterial(1, nullptr);
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

	/*
	if (NewAttachmentItemData.MainCategory == EPRMainCategory::Attachment)
	{
		if (Attachments.Find(NewAttachmentItemData.SubCategory))
		{
			if (NewAttachmentItemData == Attachments[NewAttachmentItemData.SubCategory])
			{
				Attachments.Add(NewAttachmentItemData.SubCategory, FPRItemData());
				OnUpdateAttachment.Broadcast(NewAttachmentItemData.Index, NewAttachmentItemData.SubCategory, nullptr);

				switch (NewAttachmentItemData.SubCategory)
				{
				case EPRCategory::Attachment_Barrel:
					BarrelMesh->SetStaticMesh(nullptr);
					break;

				case EPRCategory::Attachment_Grip:
					GripMesh->SetStaticMesh(nullptr);
					break;

				case EPRCategory::Attachment_Scope:
					Client_SetScope(nullptr);
					break;

				default:
					break;
				}
				return;
			}
		}
		Attachments.Add(NewAttachmentItemData.SubCategory, NewAttachmentItemData);
		OnUpdateAttachment.Broadcast(NewAttachmentItemData.Index, NewAttachmentItemData.SubCategory, NewAttachmentItemData);

		switch (NewAttachmentItemData.SubCategory)
		{
		case EPRCategory::Attachment_Barrel:
			BarrelMesh->SetStaticMesh(NewAttachmentItemData.GetBaseData().Mesh);
			break;

		case EPRCategory::Attachment_Grip:
			GripMesh->SetStaticMesh(NewAttachmentItemData.GetBaseData().Mesh);
			break;

		case EPRCategory::Attachment_Scope:
			Client_SetScope(NewAttachmentItemData);
			break;

		default:
			break;
		}
	}
	*/
}
