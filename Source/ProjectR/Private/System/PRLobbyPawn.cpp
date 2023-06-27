// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRLobbyPawn.h"

#include "Character/PRCharacter.h"
#include "Character/PRPlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Library/PRItemLibrary.h"
#include "Library/RyanLibrary.h"
#include "System/PRGameInstance.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Item/PRItemDataObject.h"
#include "Kismet/GameplayStatics.h"
#include "System/PRPlayerState.h"


// Sets default values
APRLobbyPawn::APRLobbyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Root);
	Head->SetRelativeRotation(FRotator(0.0f, 25.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadDefaultMesh(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head2.SK_Military_Head2'"));
	if (HeadDefaultMesh.Succeeded())
	{
		Head->SetSkeletalMeshAsset(HeadDefaultMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("AnimBlueprint'/Game/System/Lobby/ABP_LobbyPawn.ABP_LobbyPawn_C'"));
	if (AnimInstance.Succeeded())
	{
		Head->SetAnimClass(AnimInstance.Class);
	}

	Top = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(Head);
	Top->SetLeaderPoseComponent(Head);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopDefaultMesh(TEXT("SkeletalMesh'/Game/Costume/Mesh/Top/SK_Top_01.SK_Top_01'"));
	if (TopDefaultMesh.Succeeded())
	{
		Top->SetSkeletalMeshAsset(TopDefaultMesh.Object);
	}

	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(Head);
	Arm->SetLeaderPoseComponent(Head);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArmDefaultMesh(TEXT("SkeletalMesh'/Game/Costume/Mesh/Top/Arm/SK_Arm.SK_Arm'"));
	if (ArmDefaultMesh.Succeeded())
	{
		Arm->SetSkeletalMeshAsset(ArmDefaultMesh.Object);
	}

	Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom"));
	Bottom->SetupAttachment(Head);
	Bottom->SetLeaderPoseComponent(Head);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomDefaultMesh(TEXT("SkeletalMesh'/Game/Costume/Mesh/Bottom/SK_Bottom_01.SK_Bottom_01'"));
	if (BottomDefaultMesh.Succeeded())
	{
		Bottom->SetSkeletalMeshAsset(BottomDefaultMesh.Object);
	}

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(Head);
	Shoes->SetLeaderPoseComponent(Head);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesDefaultMesh(TEXT("SkeletalMesh'/Game/Costume/Mesh/Shoes/SK_Shoes_01.SK_Shoes_01'"));
	if (ShoesDefaultMesh.Succeeded())
	{
		Shoes->SetSkeletalMeshAsset(ShoesDefaultMesh.Object);
	}

	Equipments = CreateDefaultSubobject<USceneComponent>(TEXT("Equipments"));
	Equipments->SetupAttachment(Head);

	HeadGear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadGear"));
	HeadGear->SetupAttachment(Equipments);
	HeadGear->SetLeaderPoseComponent(Head);

	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest"));
	Vest->SetupAttachment(Equipments);
	Vest->SetLeaderPoseComponent(Head);

	Backpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));
	Backpack->SetupAttachment(Equipments);
	Backpack->SetLeaderPoseComponent(Head);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Root);
	Camera->SetRelativeLocationAndRotation(FVector(-165.0f, 334.0f, 125.0f), FRotator(0.0f, -90.0f, 0.0f));

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(Root);
	SceneCapture->SetRelativeLocationAndRotation(FVector(-32.0f, 249.0f, 92.0f), FRotator(0.0f, -84.33f, 0.0f));
}

// Called when the game starts or when spawned
void APRLobbyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APRLobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APRLobbyPawn::InitLiveCharacter(UPRWidgetBase* HUD)
{
	UE_LOG(LogClass, Warning, TEXT("APRLobbyPawn::InitLiveCharacter"));

	if (UTextureRenderTarget2D* NewTextureRenderTarget = NewObject<UTextureRenderTarget2D>())
	{
		NewTextureRenderTarget->InitAutoFormat(570, 950);
		SceneCapture->TextureTarget = NewTextureRenderTarget;
		SceneCapture->FOVAngle = 30.0f;
		SceneCapture->bConsiderUnrenderedOpaquePixelAsFullyTranslucent = true;

		if (HUD)
		{
			UMaterialInstanceDynamic* MID_LiveCharacter = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), HUD->GetLiveCharacterMaterial());
			MID_LiveCharacter->SetTextureParameterValue(FName(TEXT("LiveCharacter")), NewTextureRenderTarget);
			HUD->SetLiveCharacterMaterial(MID_LiveCharacter);
			UE_LOG(LogClass, Warning, TEXT("MID_LiveCharacter = %s"), *MID_LiveCharacter->GetName());
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("APRLobbyPawn::InitLiveCharacter : HUD is invalid."));
		}
	}
}


// Called to bind functionality to input
void APRLobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APRLobbyPawn::Client_UpdateCostume_Implementation(const TArray<FPRCostume>& Costumes)
{
	for (FPRCostume Costume : Costumes)
	{
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
}

void APRLobbyPawn::UpdateEquipment(EPRCategory Category, FName ID)
{
	USkeletalMesh* EquipmentMesh = nullptr;

	if (FPREquipmentData* EquipmentData = UPRItemLibrary::GetAdvancedData<FPREquipmentData>(ID))
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
} 

