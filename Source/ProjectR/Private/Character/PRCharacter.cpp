#include "Character/PRCharacter.h"

#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Library/PRItemEnumLibrary.h"
#include "Library/PRItemStructLibrary.h"
#include "Library/RyanLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/PRGameInstance.h"
#include "System/PRLobbyPawn.h"

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

void APRCharacter::UpdateCostume()
{
	if (UPRGameInstance* GameInstance = Cast<UPRGameInstance>(GetGameInstance()))
	{
		TArray<FPRCostume> Costumes = GameInstance->GetCharacterCostume();

		for (FPRCostume Costume : Costumes)
		{
			switch (Costume.Category)
			{
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

			case EPRCostume::Default:
				break;
			}
		}
	}
}

void APRCharacter::UpdateEquipment(FPRItemData ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("APRCharacter::UpdateEquipment"))
	const FPREquipmentData* EquipmentData = ItemData.GetAdvancedData<FPREquipmentData>();

	switch (ItemData.SubCategory)
	{
	case EPRSubCategory::Equipment_HeadGear:
		HeadGear->SetSkeletalMeshAsset(EquipmentData->BodyMesh);
		break;

	case EPRSubCategory::Equipment_Vest:
		Vest->SetSkeletalMeshAsset(EquipmentData->BodyMesh);
		break;

	case EPRSubCategory::Equipment_Backpack:
		Backpack->SetSkeletalMeshAsset(EquipmentData->BodyMesh);
		break;

	default:
		break;
	}
}

void APRCharacter::Server_UpdateEquipment_Implementation(FPRItemData ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("APRCharacter::Server_UpdateEquipment"));
	UpdateEquipment(ItemData);
	Multicast_UpdateEquipment(ItemData);
}

void APRCharacter::Multicast_UpdateEquipment_Implementation(FPRItemData ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("APRCharacter::Client_UpdateEquipment"))
	UpdateEquipment(ItemData);
}

void APRCharacter::Server_UpdateFirearm_Implementation(int32 Index, FPRItemData ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_UpdateFirearm"));

	if(HasAuthority())
	{
		if (APRFirearm* Firearm = GetWorld()->SpawnActor<APRFirearm>())
		{
			Firearm->Init(ItemData);
			Firearm->SetOwner(GetOwner());
			Firearm->Index = Index;
			const FString SocketString = FString::Printf(TEXT("Firearm_Socket_0%d"), Index + 1);
			const FName SocketName = FName(*SocketString);
			Firearm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Warning, TEXT("Owner = %s"), *GetOwner()->GetName());
		}
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
	UpdateCostume();

	if(UPRInventoryComponent* InventoryComponent = Cast<UPRInventoryComponent>(GetComponentByClass(UPRInventoryComponent::StaticClass())))
	{
		PRInventoryComponent = InventoryComponent;
		InventoryComponent->OnUpdateFirearm.AddUObject(this, &APRCharacter::Server_UpdateFirearm);
		InventoryComponent->OnUpdateEquipment.AddUObject(this, &APRCharacter::Server_UpdateEquipment);
	}

	if(UPRStatusComponent* StatusComponent = Cast<UPRStatusComponent>(GetComponentByClass(UPRStatusComponent::StaticClass())))
	{
		PRStatusComponent = StatusComponent;
		StatusComponent->OnStaminaExhausted.BindUObject(this, &APRBaseCharacter::SetDesiredGait);
	}
}

void APRCharacter::Server_HoldFirearm_Implementation(int32 Index)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for(AActor* AttachedActor : AttachedActors)
	{
		if(APRFirearm* Firearm = Cast<APRFirearm>(AttachedActor))
		{
			if(Firearm->Index == Index)
			{
				AttachToHand(Firearm);
			}
			else
			{
				AttachToBack(Firearm);
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
	const FString SocketString = FString::Printf(TEXT("Firearm_Socket_0%d"), Firearm->Index + 1);
	const FName SocketName = FName(*SocketString);
	Firearm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void APRCharacter::OnRep_CurrentHeldFirearm()
{
	SetOverlayState(CurrentHeldFirearm->ItemData.GetAdvancedData<FPRFirearmData>()->OverlayState);
}

void APRCharacter::OnShoot(const FInputActionValue& Value)
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

float APRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = PRStatusComponent->GetHealthPoint() - DamageAmount;
	PRStatusComponent->UpdateHealthPoint(-DamageAmount);
	//PRStatusComponent->SetHealthPoint(DamageApplied);
	return DamageApplied;
}
