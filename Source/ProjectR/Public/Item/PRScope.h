// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Library/PRItemLibrary.h"
#include "Item/PRItemDataObject.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PRScope.generated.h"

UCLASS()
class PROJECTR_API APRScope : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APRScope();

	void Init(UPRItemDataObject* ItemDataObject);

	void Init(UStaticMesh* NewScopeMesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ScopeMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr <USceneCaptureComponent2D> SceneCapture = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr <UCameraComponent> Camera = nullptr;
};
