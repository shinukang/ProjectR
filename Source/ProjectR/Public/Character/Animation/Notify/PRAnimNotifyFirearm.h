// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PRAnimNotifyFirearm.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPRFirearmState : uint8
{
	Equip_Start,
	Equip_Detach,
	Equip_Attach,
	Equip_End,

	Reload_Start,
	Reload_End,
};

UCLASS()
class PROJECTR_API UPRAnimNotifyFirearm : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPRFirearmState FirearmState;
};
