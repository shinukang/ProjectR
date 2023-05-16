// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRItem.h"
#include "PRFirearm.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFirearmInitialized)
/**
 * 
 */
UCLASS()
class PROJECTR_API APRFirearm : public AActor
{
	GENERATED_BODY()

public:
	APRFirearm();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	void Init(FPRItemData InItemData);

	void StartFire();

	void StopFire();

	void Fire();

	UFUNCTION()
	void OnRep_ItemData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> BodyMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MagMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ScopeMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BarrelMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> GripMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ScopeChildActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemData)
	FPRItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FPRFirearmData FirearmData;

	FOnFirearmInitialized OnFirearmInitialized;

	bool bUseGrip;

	bool bIsFiring = false;

	int32 Index = INDEX_NONE;

	int32 CurrentAmmo = 0;

private:
	

	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector SpawnLocation, FRotator SpawnRotation);

	void Reload();

	FTimerHandle FireTimerHandle;
	//void SetFireMode();
};
