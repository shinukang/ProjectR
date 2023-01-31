#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Library/PRCharacterStructLibrary.h"

#include "PRCharacterMovementComponent.generated.h"

/**
 * Authoritative networked Character Movement
 */
UCLASS()
class PROJECTR_API UPRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

	class PROJECTR_API FSavedMove_My : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
		                        class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Walk Speed Update
		uint8 bSavedRequestMovementSettingsChange : 1;
		EPRGait SavedAllowedGait = EPRGait::Walking;
	};

	class PROJECTR_API FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;

	// Movement Settings Override
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	// Movement Settings Variables
	UPROPERTY()
	uint8 bRequestMovementSettingsChange = 1;

	UPROPERTY()
	EPRGait AllowedGait = EPRGait::Walking;

	UPROPERTY(BlueprintReadOnly, Category = "PR|Movement System")
	FPRMovementSettings CurrentMovementSettings;

	// Set Movement Curve (Called in every instance)
	float GetMappedSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetMovementSettings(FPRMovementSettings NewMovementSettings);

	// Set Max Walking Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetAllowedGait(EPRGait NewAllowedGait);

	UFUNCTION(Reliable, Server, Category = "Movement Settings")
	void Server_SetAllowedGait(EPRGait NewAllowedGait);
};
