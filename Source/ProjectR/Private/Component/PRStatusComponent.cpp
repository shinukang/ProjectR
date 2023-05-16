// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PRStatusComponent.h"

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

	if(Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		OnUpdateHealthPoint.Execute(HealthPoint);
		if (HealthPoint <= 0.0f)
		{
			FString DeathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);
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

	if(TimerManager.IsTimerActive(DecreaseHandle))
	{
		TimerManager.ClearTimer(DecreaseHandle);
	}

	TimerManager.SetTimer(IncreaseHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(Stamina >= 1.0f)
		{
			TimerManager.ClearTimer(IncreaseHandle);
		}
		UpdateStamina(IncrementOfStamina);
	}), IncrementalCycleOfStamina, true, WaitingTimeBeforeIncreasingStamina);
}

void UPRStatusComponent::DecreaseStamina()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(IncreaseHandle))
	{
		TimerManager.ClearTimer(IncreaseHandle);
	}

	TimerManager.SetTimer(DecreaseHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (Stamina <= 0.0f) 
		{
			TimerManager.ClearTimer(DecreaseHandle);
			OnStaminaExhausted.ExecuteIfBound(EPRGait::Running);
		}
		UpdateStamina(DecrementOfStamina);
	}), DecrementalCycleOfStamina, true);
}



