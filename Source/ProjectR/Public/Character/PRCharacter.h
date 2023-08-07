#pragma once

#include "CoreMinimal.h"
#include "Character/PRBaseCharacter.h"
#include "Component/PRInventoryComponent.h"
#include "Component/PRStatusComponent.h"
#include "Item/PRItemDataObject.h"
#include "Item/PRFirearm.h"
#include "System/PRLiveCharacter.h"
#include "PRCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateBulletType, FName);

UCLASS(Blueprintable, BlueprintType)
class PROJECTR_API APRCharacter : public APRBaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "PR|HeldObject")
	void ClearHeldObject();

	/*
	UFUNCTION(BlueprintCallable, Category = "PR|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);
	*/

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

	virtual void OnRep_PlayerState() override;

	/* Àåºñ(Çï¸ä, ¹æÅºÁ¶³¢, °¡¹æ) ¾÷µ¥ÀÌÆ® ÇÔ¼ö */

	UFUNCTION(Server, Reliable)
	void Server_UpdateEquipment(EPRCategory SubCategory, FName ID, APRItem* NewEquipmentItem = nullptr);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateEquipment(EPRCategory SubCategory, FName ID);

	UFUNCTION(Server, Reliable)
	void Server_UpdateFirearm(int32 Index, FPRItemData FirearmItemData);

	UFUNCTION(Server, Reliable)
	void Server_HoldFirearm(int32 Index);

	UFUNCTION()
	void OnRep_CurrentHeldFirearm();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
	void OnShoot(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
	void OnReload(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
	void OnAim(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
	void OnADS(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PR|Input")
	void OnZoom(const FInputActionValue& Value);

	APRFirearm* GetCurrentHeldFirearm() { return CurrentHeldFirearm;  }

	void SetCurrentHeldFirearm(APRFirearm* NewFirearm);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCostume(const TArray<FPRCostume>& Costumes);

	UFUNCTION(Client, Reliable)
	void Client_UpdateLiveCharacter(const TArray<FPRCostume>& Costumes);

	void AttachToHand(bool bRight = true);

	void AttachToBack(APRFirearm* NewFirearm);

	void Die();

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EPROverlayState PreviousState) override;


	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PR|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USceneComponent> BodyParts = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Top = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Arm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Bottom = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Shoes = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USceneComponent> Equipments = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> HeadGear = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Vest = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USkeletalMeshComponent> Backpack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PR|Component")
	TObjectPtr<USphereComponent> SphereCollision = nullptr;

	UPROPERTY()
	TObjectPtr<UPRInventoryComponent> PRInventoryComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UPRStatusComponent> PRStatusComponent = nullptr;

	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	FOnUpdateBulletType OnUpdateBullet;

private:
	bool bNeedsColorReset = false;

	bool bIsOnADS = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentHeldFirearm, meta=(AllowPrivateAccess=true))
	TObjectPtr<APRFirearm> CurrentHeldFirearm = nullptr;

	
};
