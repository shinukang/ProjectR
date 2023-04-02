// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRWeapon.h"
#include "PRFirearm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API APRFirearm : public APRWeapon
{
	GENERATED_BODY()

public:
	APRFirearm();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Init() override;

	virtual void OnRep_ID() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MagMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ScopeMesh = nullptr;

	//FPRFirearmData FirearmData;

private:
	//void Fire();

	//void Reload();

	//void SetFireMode();
};
