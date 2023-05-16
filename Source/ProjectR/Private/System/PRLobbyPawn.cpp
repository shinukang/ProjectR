// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PRLobbyPawn.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Library/PRItemEnumLibrary.h"
#include "Library/RyanLibrary.h"
#include "Library/PRItemStructLibrary.h"
#include "System/PRGameInstance.h"
#include "System/PRItemObject.h"


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

	Top = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(Head);
	Top->SetLeaderPoseComponent(Head);

	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(Head);
	Arm->SetLeaderPoseComponent(Head);

	Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom"));
	Bottom->SetupAttachment(Head);
	Bottom->SetLeaderPoseComponent(Head);

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(Head);
	Shoes->SetLeaderPoseComponent(Head);

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

// Called to bind functionality to input
void APRLobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APRLobbyPawn::UpdateCostume()
{
	if(UPRGameInstance* GameInstance = Cast<UPRGameInstance>(GetGameInstance()))
	{
		TArray<FPRCostume> Costumes = GameInstance->GetCharacterCostume();

		for(FPRCostume Costume : Costumes)
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

void APRLobbyPawn::UpdateEquipment(FPRItemData ItemData)
{
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

