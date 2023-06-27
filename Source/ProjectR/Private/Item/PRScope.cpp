// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRScope.h"

#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

// Sets default values
APRScope::APRScope()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScopeMesh"));
	ScopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ScopeMesh;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(Camera);
}

void APRScope::Init(UStaticMesh* NewScopeMesh)
{
	
}

void APRScope::Init(UPRItemDataObject* ItemDataObject)
{
	/*
	Init(InItemData.GetBaseData().Mesh);

	//ScopeMesh->SetStaticMesh(NewScopeMesh);
	Camera->AttachToComponent(ScopeMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Camera")));

	if(FPRScopeData* ScopeData = InItemData.GetAdvancedData<FPRScopeData>())
	{
		if(ScopeData->bNeedRenderTarget)
		{
			if (UTextureRenderTarget2D* NewTextureRenderTarget = NewObject<UTextureRenderTarget2D>())
			{
				NewTextureRenderTarget->InitAutoFormat(1024, 1024);
				SceneCapture->TextureTarget = NewTextureRenderTarget;
				SceneCapture->FOVAngle = InItemData.GetAdvancedData<FPRScopeData>()->FOV;
				UMaterialInstanceDynamic* MID_Scope_Sight = ScopeMesh->CreateDynamicMaterialInstance(1);
				MID_Scope_Sight->SetTextureParameterValue(FName(TEXT("RenderTarget")), NewTextureRenderTarget);
			}
		}
	}
	*/
}


// Called when the game starts or when spawned
void APRScope::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APRScope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

