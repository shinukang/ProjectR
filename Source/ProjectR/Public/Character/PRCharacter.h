#pragma once

#include "CoreMinimal.h"
#include "Character/PRBaseCharacter.h"
#include "Component/PRInventoryComponent.h"
#include "Component/PRStatusComponent.h"
#include "System/PRItemObject.h"
#include "Item/PRFirearm.h"
#include "PRCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */

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

	UFUNCTION(BlueprintCallable, Category = "PR|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

	void UpdateEquipment(FPRItemData ItemData);

	UFUNCTION(Server, Reliable)
	void Server_UpdateEquipment(FPRItemData ItemData);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateEquipment(FPRItemData ItemData);

	UFUNCTION(Server, Reliable)
	void Server_UpdateFirearm(int32 Index, FPRItemData ItemData);

	UFUNCTION(Server, Reliable)
	void Server_HoldFirearm(int32 Index);

	UFUNCTION()
	void OnRep_CurrentHeldFirearm();

	virtual void OnShoot(const FInputActionValue& Value) override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EPROverlayState PreviousState) override;

	void UpdateCostume();

	void AttachToHand(APRFirearm* Firearm);

	void AttachToBack(APRFirearm* Firearm);


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

	UPROPERTY()
	TObjectPtr<UPRInventoryComponent> PRInventoryComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UPRStatusComponent> PRStatusComponent = nullptr;

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	bool bNeedsColorReset = false;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHeldFirearm)
	TObjectPtr<APRFirearm> CurrentHeldFirearm = nullptr;


};
