#include "Character/PRCharacter.h"

#include "Character/PRPlayerController.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Library/PRItemLibrary.h"
#include "Library/RyanLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/PRGameInstance.h"
#include "System/PRLiveCharacterSpawnPoint.h"
#include "System/PRLobbyPawn.h"
#include "System/PRPlayerState.h"

APRCharacter::APRCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	Top = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(GetMesh());
	Top->SetLeaderPoseComponent(GetMesh());

	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(GetMesh());
	Arm->SetLeaderPoseComponent(GetMesh());

	Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom"));
	Bottom->SetupAttachment(GetMesh());
	Bottom->SetLeaderPoseComponent(GetMesh());

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(GetMesh());
	Shoes->SetLeaderPoseComponent(GetMesh());

	Equipments = CreateDefaultSubobject<USceneComponent>(TEXT("Equipments"));
	Equipments->SetupAttachment(GetMesh());

	HeadGear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadGear"));
	HeadGear->SetupAttachment(Equipments);
	HeadGear->SetLeaderPoseComponent(GetMesh());

	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest"));
	Vest->SetupAttachment(Equipments);
	Vest->SetLeaderPoseComponent(GetMesh());

	Backpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));
	Backpack->SetupAttachment(Equipments);
	Backpack->SetLeaderPoseComponent(GetMesh());
}

void APRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APRCharacter, CurrentHeldFirearm);
}

void APRCharacter::Multicast_UpdateCostume_Implementation(const TArray<FPRCostume>& Costumes)
{
	UE_LOG(LogClass, Warning, TEXT("APRCharacter::Multicast_UpdateCostume"));

	for (FPRCostume Costume : Costumes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Costume = %s, Category = %d"), *Costume.Name.ToString(), Costume.Category);

		switch (Costume.Category)
		{
		case EPRCostume::Default:
			break;
		case EPRCostume::HeadGear:
			HeadGear->SetSkeletalMeshAsset(Costume.Mesh);
			break;
		case EPRCostume::Top:
			Top->SetSkeletalMeshAsset(Costume.Mesh);
			Arm->SetSkeletalMeshAsset(Costume.SubMesh);
			break;
		case EPRCostume::Bottom:
			Bottom->SetSkeletalMeshAsset(Costume.Mesh);
			break;
		case EPRCostume::Shoes:
			Shoes->SetSkeletalMeshAsset(Costume.Mesh);
			break;
		}
	}
	Client_UpdateLiveCharacter(Costumes);
}

void APRCharacter::Client_UpdateLiveCharacter_Implementation(const TArray<FPRCostume>& Costumes)
{
	if(GetOwner())
	{
		if (APRLobbyPawn* LiveCharacter = Cast<APRPlayerController>(GetOwner())->LiveCharacter)
		{
			LiveCharacter->Client_UpdateCostume(Costumes);
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("APRCharacter::Client_UpdateLiveCharacter : LiveCharacter is invalid."));
		}
	}
}

void APRCharacter::Server_UpdateEquipment_Implementation(EPRCategory Category, FName ID, APRItem* NewEquipmentItem)
{
	UE_LOG(LogTemp, Warning, TEXT("APRCharacter::Server_UpdateEquipment"));

	if(const FName* CurrentEquipmentID = PRInventoryComponent->Equipments.Find(Category))
	{
		FVector SpawnLocation = PRInventoryComponent->GetSpawnLocation();
		FRotator SpawnRotation = GetActorRotation() + FRotator(0.0f, - 90.f, 0.0f);

		if(APRItem* NewItem = GetWorld()->SpawnActor<APRItem>(SpawnLocation, SpawnRotation))
		{
			NewItem->Init(FPRItemData(*CurrentEquipmentID));
		}
	}
	if(NewEquipmentItem)
	{
		NewEquipmentItem->Destroy();
	}
	PRInventoryComponent->Equipments.Add(Category, ID);
	Multicast_UpdateEquipment(Category, ID);
}

void APRCharacter::Multicast_UpdateEquipment_Implementation(EPRCategory Category, FName ID)
{
	UE_LOG(LogTemp, Warning, TEXT("APRCharacter::Multicast_UpdateEquipment"));

	USkeletalMesh* EquipmentMesh = nullptr;

	if(FPREquipmentData* EquipmentData = UPRItemLibrary::GetAdvancedData<FPREquipmentData>(ID))
	{
		EquipmentMesh = EquipmentData->BodyMesh;
	}

	switch (Category)
	{
	case EPRCategory::Equipment_HeadGear:
		HeadGear->SetSkeletalMeshAsset(EquipmentMesh);
		break;

	case EPRCategory::Equipment_Vest:
		Vest->SetSkeletalMeshAsset(EquipmentMesh);
		break;

	case EPRCategory::Equipment_Backpack:
		Backpack->SetSkeletalMeshAsset(EquipmentMesh);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("APRCharacter::Multicast_UpdateEquipment : Category is invalid."))
		break;
	}
	PRInventoryComponent->OnUpdateEquipment.Broadcast(Category, ID);
}

void APRCharacter::Server_UpdateFirearm_Implementation(int32 Index, FPRItemData FirearmItemData)
{
	if (APRFirearm* Firearm = GetWorld()->SpawnActor<APRFirearm>())
	{
		FirearmItemData.Index = Index;
		Firearm->Init(FirearmItemData);
		Firearm->SetOwner(GetOwner());
		
		const FString SocketString = FString::Printf(TEXT("Firearm_Socket_0%d"), Index + 1);
		const FName SocketName = FName(*SocketString);
		Firearm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		PRInventoryComponent->Firearms[Index] = Firearm;
	}
}

void APRCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void APRCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		StaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	HeldObjectRoot->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	HeldObjectRoot->SetRelativeLocation(Offset);
}

void APRCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void APRCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}

ECollisionChannel APRCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform APRCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector APRCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void APRCharacter::OnOverlayStateChanged(EPROverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void APRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
}

void APRCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateHeldObject();

	if(UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		PRInventoryComponent = InventoryComponent;
	}

	if(UPRStatusComponent* StatusComponent = Cast<UPRStatusComponent>(GetComponentByClass(UPRStatusComponent::StaticClass())))
	{
		PRStatusComponent = StatusComponent;
		StatusComponent->OnStaminaExhausted.BindUObject(this, &APRBaseCharacter::SetDesiredGait);
	}
}

void APRCharacter::Server_HoldFirearm_Implementation(int32 Index)
{
	if(!PRInventoryComponent->Firearms[Index])
	{
		return;
	}

	if(CurrentHeldFirearm)
	{
		if (CurrentHeldFirearm == PRInventoryComponent->Firearms[Index])
			return;
	}

	CurrentHeldFirearm = nullptr;

	for(int32 i = 0; i < 3; i++)
	{
		if(PRInventoryComponent->Firearms[i])
		{
			if (i == Index)
			{
				AttachToHand(PRInventoryComponent->Firearms[i]);
			}
			else
			{
				AttachToBack(PRInventoryComponent->Firearms[i]);
			}
		}
	}
}

void APRCharacter::AttachToHand(APRFirearm* Firearm)
{
	Firearm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("VB LHS_ik_hand_gun")));
	CurrentHeldFirearm = Firearm;
}

void APRCharacter::AttachToBack(APRFirearm* Firearm)
{
	const FString SocketString = FString::Printf(TEXT("Firearm_Socket_0%d"), Firearm->ItemData.Index + 1);
	const FName SocketName = FName(*SocketString);
	Firearm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void APRCharacter::OnRep_CurrentHeldFirearm()
{
	if(CurrentHeldFirearm)
	{
		FPRFirearmData FirearmData = CurrentHeldFirearm->FirearmData;
		SetOverlayState(FirearmData.OverlayState);
		OnUpdateBullet.Broadcast(FirearmData.AmmunitionID);
	}
	else
	{
		SetOverlayState(EPROverlayState::Default);
		OnUpdateBullet.Broadcast(NAME_None);
	}
}

void APRCharacter::OnShoot_Implementation(const FInputActionValue& Value)
{
	if(CurrentHeldFirearm)
	{
		if(Value.Get<bool>())
		{
			CurrentHeldFirearm->StartFire();
		}
		else
		{
			CurrentHeldFirearm->StopFire();
		}
	}
}

void APRCharacter::OnReload_Implementation(const FInputActionValue& Value)
{
	if(CurrentHeldFirearm && Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("APRCharacter::OnReload"));
		CurrentHeldFirearm->Reload();
	}
}

float APRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = PRStatusComponent->GetHealthPoint() - DamageAmount;
	PRStatusComponent->UpdateHealthPoint(-DamageAmount);
	return DamageApplied;
}

void APRCharacter::OnAim_Implementation(const FInputActionValue& Value)
{
	if(!bIsOnADS)
	{
		if (Value.Get<bool>())
		{
			// AimAction: Hold "AimAction" to enter the aiming mode, release to revert back the desired rotation mode.
			SetRotationMode(EPRRotationMode::Aiming);
		}
		else
		{
			if (ViewMode == EPRViewMode::ThirdPerson)
			{
				SetRotationMode(DesiredRotationMode);
			}
			else if (ViewMode == EPRViewMode::FirstPerson)
			{
				SetRotationMode(EPRRotationMode::LookingDirection);
			}
		}
	}
}

void APRCharacter::OnADS_Implementation(const FInputActionValue& Value)
{
	if(CurrentHeldFirearm)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
		{
			if (bIsOnADS)
			{
				bIsOnADS = false;
				PlayerController->SetViewTargetWithBlend(this, 0.05f);
				SetRotationMode(EPRRotationMode::LookingDirection);
				LookLeftRightRate = LookUpDownRate = 1.2f;
			}
			else
			{
				bIsOnADS = true;
				PlayerController->SetViewTargetWithBlend(CurrentHeldFirearm, 0.05f);
				SetRotationMode(EPRRotationMode::Aiming);
				LookLeftRightRate = LookUpDownRate = 0.3f;
			}
			GetMesh()->SetVisibility(!bIsOnADS);
			HeadGear->SetVisibility(!bIsOnADS);
			CurrentHeldFirearm->bIsOnADS = bIsOnADS;
		}
	}
}

void APRCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogClass, Warning, TEXT("APRCharacter::OnRep_PlayerState"));

	if(APRPlayerState* PRPlayerState = GetPlayerState<APRPlayerState>())
	{
		PRPlayerState->LoadCostume();
	}
}
