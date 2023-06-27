// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PRInteractInterface.h"
#include "Library/RyanLibrary.h"
#include "Item/PRItemDataObject.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintCallable)
	//void Init(UPRItemDataObject* NewItemDataObject);

	UFUNCTION(BlueprintCallable)
	void Init(FPRItemData NewItemData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_ItemData();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemDataObject)
	//TObjectPtr<UPRItemDataObject> ItemDataObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ItemData)
	FPRItemData ItemData;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Collision = nullptr;

private:
	bool bIsInteractive = true;
};
