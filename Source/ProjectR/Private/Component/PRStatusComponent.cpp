// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRStatusComponent.h"

#include "Character/PRCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPRStatusComponent::UPRStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...

	
}

void UPRStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPRStatusComponent, HealthPoint);
}

// Called when the game starts
void UPRStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	// ...
	
}


// Called every frame
void UPRStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UPRStatusComponent::OnHealthPointUpdate()
{
	if(GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		FString HealthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetOwner()->GetName(), HealthPoint);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);
	}

	if(APRCharacter* Character = Cast<APRCharacter>(GetOwner()))
	{
		if(Character->IsLocallyControlled())
		{
			OnUpdateHealthPoint.Execute(HealthPoint);
		}

		if (HealthPoint <= 0.0f)
		{
			Character->Die();
		}
	}
}

void UPRStatusComponent::OnRep_HealthPoint()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_HealthPoint"));
	OnHealthPointUpdate();
}


void UPRStatusComponent::SetHealthPoint(float NewValue)
{
	UE_LOG(LogClass, Warning, TEXT("UPRStatusComponent::SetHealthPoint : NewValue = %f"), NewValue);

	HealthPoint = FMath::Clamp(NewValue, 0.0f, 1.0f);

	if(GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		OnHealthPointUpdate();
	}
}

void UPRStatusComponent::SetStamina(float NewValue)
{
	Stamina = FMath::Clamp(NewValue, 0.0f, 1.0f);
	OnUpdateStamina.ExecuteIfBound(Stamina);
}

void UPRStatusComponent::UpdateHealthPoint(float DeltaValue)
{
	SetHealthPoint(HealthPoint + DeltaValue);
}

void UPRStatusComponent::UpdateStamina(float DeltaValue)
{
	SetStamina(Stamina + DeltaValue);
}

void UPRStatusComponent::IncreaseStamina()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if(TimerManager.IsTimerActive(DecreaseStaminaHandle))
	{
		TimerManager.ClearTimer(DecreaseStaminaHandle);
	}

	TimerManager.SetTimer(IncreaseStaminaHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(Stamina >= 1.0f)
		{
			TimerManager.ClearTimer(IncreaseStaminaHandle);
		}
		UpdateStamina(IncrementOfStamina);
	}), IncrementalCycleOfStamina, true, WaitingTimeBeforeIncreasingStamina);
}

void UPRStatusComponent::DecreaseStamina()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(IncreaseStaminaHandle))
	{
		TimerManager.ClearTimer(IncreaseStaminaHandle);
	}

	TimerManager.SetTimer(DecreaseStaminaHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (Stamina <= 0.0f) 
		{
			OnStaminaExhausted.ExecuteIfBound(EPRGait::Running);
			UE_LOG(LogClass, Warning, TEXT("UPRStatusComponent::Stamina exhausted."));
			TimerManager.ClearTimer(DecreaseStaminaHandle);
		}
		UpdateStamina(DecrementOfStamina);
	}), DecrementalCycleOfStamina, true);
}

void UPRStatusComponent::IncreaseHealthPoint(float Amount)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	UpdateHealthPoint(Amount / 10.0f);
	RepeatCount++;

	if(HealthPoint >= 1.0f || RepeatCount >= 10)
	{
		RepeatCount = 0;
	}
	else
	{
		TimerManager.SetTimer(IncreaseHealthPointHandle, FTimerDelegate::CreateUObject(this, &UPRStatusComponent::IncreaseHealthPoint, Amount), 0.5f, false, 0.5f);
	}
	
}

void UPRStatusComponent::Server_UseMedicine_Implementation(FPRItemData ItemData)
{
	if(const FPRMedicineData* MedicineData = UPRItemLibrary::GetAdvancedData<FPRMedicineData>(ItemData.ID))
	{
		switch (ItemData.Category)
		{
		case EPRCategory::Medicine_HealthPoint:
			UE_LOG(LogClass, Warning, TEXT("UPRStatusComponent::UseMedicine : HealthPoint."));
			IncreaseHealthPoint(MedicineData->Efficiency);
			break;

		case EPRCategory::Medicine_Stamina:
			Client_SetStaminaBuffTimer(MedicineData->Efficiency);
			break;
		default:
			UE_LOG(LogClass, Warning, TEXT("UPRStatusComponent::UseMedicine : Default."));

			break;
		}
	}
}

void UPRStatusComponent::Client_SetStaminaBuffTimer_Implementation(float Efficiency)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	IncrementOfStamina *= Efficiency;
	DecrementOfStamina /= Efficiency;

	TimerManager.SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
	{
		IncrementOfStamina /= Efficiency;
		DecrementOfStamina *= Efficiency;
	}), 30.0f, false, 30.0f);
}

void UPRStatusComponent::SetHeadArmor(float NewHeadArmor)
{
	HeadArmor = NewHeadArmor;
}

void UPRStatusComponent::SetBodyArmor(float NewBodyArmor)
{
	BodyArmor = NewBodyArmor;
}
