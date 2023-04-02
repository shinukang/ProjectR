// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PRInteractInterface.h"
#include "Library/PRItemEnumLibrary.h"
#include "Library/RyanLibrary.h"
#include "Kismet/GameplayStatics.h"


#include "Components/CapsuleComponent.h"
#include "PRItem.generated.h"


UCLASS()
class PROJECTR_API APRItem : public AActor, public IPRInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APRItem();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanInteract_Implementation() override;

	FName GetItemID_Implementation() override;

	int32 GetItemAmount_Implementation() override;

	void OnInteract_Implementation(APlayerController* PlayerController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Init();

	UFUNCTION()
	virtual void OnRep_ID();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UMeshComponent> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Collision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_ID")
	FName ID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EPRItemType Type = EPRItemType::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Amount = 1;

private:
	bool bIsInteractive = true;


};
