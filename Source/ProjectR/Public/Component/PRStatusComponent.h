// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRBaseComponent.h"
#include "Components/ActorComponent.h"
#include "PRStatusComponent.generated.h"


UCLASS(BlueprintType, Blueprintable)
class PROJECTR_API UPRStatusComponent : public UPRBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRStatusComponent();

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

private:
	
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
	float HealthPoint = 1.0f;

	float Stamina = 1.0f;

	FTimerHandle IncreaseHandle;

	FTimerHandle DecreaseHandle;
};
