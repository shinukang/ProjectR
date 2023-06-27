// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSpawner.generated.h"

USTRUCT(BlueprintType)
struct FPRItemSpawnSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

UCLASS()
class PROJECTR_API APRSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APRSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FPRItemSpawnSetting> SpawnSettings;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float DistanceBetweenItem = 50.0f;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root = nullptr;
};
