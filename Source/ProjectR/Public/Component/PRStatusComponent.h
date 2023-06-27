// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"
#include "Library/PRCharacterEnumLibrary.h"
#include "PRStatusComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnStaminaExhausted, EPRGait);
DECLARE_DELEGATE_OneParam(FOnUpdateStamina, float);
DECLARE_DELEGATE_OneParam(FOnUpdateHealthPoint, float);

UCLASS(BlueprintType, Blueprintable)
class PROJECTR_API UPRStatusComponent : public UPRBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRStatusComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetHealthPoint(float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetStamina(float NewValue);

	UFUNCTION(BlueprintCallable)
	void UpdateHealthPoint(float DeltaValue);

	UFUNCTION(BlueprintCallable)
	void UpdateStamina(float DeltaValue);

	UFUNCTION(Server, Reliable)
	void Server_UseMedicine(FPRItemData ItemData);

	UFUNCTION(Client, Reliable)
	void Client_SetStaminaBuffTimer(float Efficiency);

	void IncreaseHealthPoint(float Amount);

	void IncreaseStamina();

	void DecreaseStamina();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPoint() { return HealthPoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStamina() { return Stamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDead() { return HealthPoint <= 0.0f; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnHealthPointUpdate();

private:
	UFUNCTION()
	void OnRep_HealthPoint();

public:
	FOnStaminaExhausted OnStaminaExhausted;

	FOnUpdateHealthPoint OnUpdateHealthPoint;

	FOnUpdateStamina OnUpdateStamina;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setting|Stamina")
	float IncrementOfStamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Stamina")
	float IncrementalCycleOfStamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Stamina")
	float WaitingTimeBeforeIncreasingStamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Stamina")
	float DecrementOfStamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Stamina")
	float DecrementalCycleOfStamina = 0.0f;



private:
	UPROPERTY(ReplicatedUsing =OnRep_HealthPoint)
	float HealthPoint = 1.0f;

	float Stamina = 1.0f;

	FTimerHandle IncreaseStaminaHandle;

	FTimerHandle DecreaseStaminaHandle;

	FTimerHandle IncreaseHealthPointHandle;

	FTimerHandle TimerHandle;

	int32 RepeatCount = 0;
};
