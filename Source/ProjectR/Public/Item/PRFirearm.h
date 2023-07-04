// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRItem.h"
#include "Camera/CameraComponent.h"
#include "Library/PRItemLibrary.h"
#include "Item/PRItemDataObject.h"
#include "Item/PRScope.h"
#include "PRFirearm.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFirearmInitialized)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateAttachment, int32, EPRCategory, FName);
DECLARE_DELEGATE_ThreeParams(FOnFire, float, float, float);
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

	void Init(FPRItemData NewItemData);

	void StartFire();

	void StopFire();

	void Fire();

	void Reload();

	FPRItemData* CheckIsBulletsInInventory();

	UFUNCTION(Client, Reliable)
	void Client_SetScope(FName ID);

	UFUNCTION(Server, Reliable)
	void Server_AddAttachment(EPRCategory Category, FName ID, APRItem* NewAttachmentItem);

	UFUNCTION(Server, Reliable)
	void Server_RemoveAttachment(EPRCategory Category, bool bNeedToSpawn);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateAttachment(int32 Index, EPRCategory Category, FName ID);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> BodyMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MagMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BarrelMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> GripMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ScopeMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> ADSCam = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> ScopeCam = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneCaptureComponent2D> SceneCapture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemData)
	FPRItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_FirearmData)
	FPRFirearmData FirearmData;

	UPROPERTY()
	TMap<EPRCategory, FName> Attachments;

	FOnFirearmInitialized OnFirearmInitialized;

	FOnUpdateAttachment OnUpdateAttachment;

	FOnFire OnFire;

	bool bUseGrip;

	bool bIsFiring = false;

	bool bIsOnADS = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 LoadedBullets = 0;

protected:
	UFUNCTION()
	void OnRep_ItemData();

	UFUNCTION()
	void OnRep_FirearmData();

private:

	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(Server, Reliable)
	void Server_Reload();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnEffects();

	FTimerHandle FireTimerHandle;
	//void SetFireMode();
};
