// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRFirearm.h"

#include "Character/PRPlayerController.h"
#include "Library/PRItemStructLibrary.h"
#include "Library/RyanLibrary.h"
#include "Component/PRInventoryComponent.h"
#include "Item/PRBullet.h"
#include "Net/UnrealNetwork.h"

APRFirearm::APRFirearm()
{
	bReplicates = true;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyMesh");
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BodyMesh;

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	MagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MagMesh->SetupAttachment(BodyMesh);

	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ScopeMesh->SetupAttachment(BodyMesh);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BarrelMesh->SetupAttachment(BodyMesh);

	GripMesh = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	GripMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GripMesh->SetupAttachment(BodyMesh);

	ScopeChildActor = CreateDefaultSubobject<UChildActorComponent>("ScopeChildActor");
	ScopeChildActor->SetupAttachment(BodyMesh);

}

void APRFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APRFirearm, ItemData);
	DOREPLIFETIME(APRFirearm, FirearmData);
}

void APRFirearm::BeginPlay()
{
	Super::BeginPlay();
}

void APRFirearm::Init(FPRItemData InItemData)
{
	ItemData = InItemData;
}

void APRFirearm::OnRep_ItemData()
{
	if (const FPRFirearmData* NewFirearmData = ItemData.GetAdvancedData<FPRFirearmData>())
	{
		FirearmData = NewFirearmData;
		BodyMesh->SetSkeletalMeshAsset(FirearmData.BodyMesh);
		MagMesh->SetStaticMesh(FirearmData.MagMesh);
		MagMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("Mag_Socket")));
	}
}

void APRFirearm::StartFire()
{
	bIsFiring = true;
	Fire();
}

void APRFirearm::Fire()
{
	if (bIsFiring)
	{
		if(CurrentAmmo > 0)
		{
			if (const APlayerCameraManager* Camera = Cast<APlayerController>(GetOwner())->PlayerCameraManager)
			{
				FVector CamLoc; // Player Camera Location
				FRotator CamRot; // Player Camera Rotation
				Camera->GetCameraViewPoint(CamLoc, CamRot);

				FHitResult HitResult;

				FVector SpawnLocation = BodyMesh->GetSocketLocation(FName(TEXT("Barrel")));
				FVector Destination = CamLoc + FirearmData.EffectiveRange * CamRot.Vector();

				if (GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, CamLoc + FirearmData.EffectiveRange * CamRot.Vector(), ECollisionChannel::ECC_Visibility))
				{
					Destination = HitResult.ImpactPoint;
				}
				FRotator SpawnRotation = FRotationMatrix::MakeFromX(Destination - SpawnLocation).Rotator();
				Server_Fire(SpawnLocation, SpawnRotation);
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APRFirearm::Fire, FirearmData.FireRate, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Ammo"));
		}
	}
}

void APRFirearm::StopFire()
{
	bIsFiring = false;
}

void APRFirearm::Server_Fire_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APlayerController>(GetOwner())->GetPawn();
	SpawnParameters.Owner = GetOwner();

	DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + SpawnRotation.Vector() * FirearmData.EffectiveRange, FColor::Red, true, 5.0f);

	if(APRBullet* Bullet = GetWorld()->SpawnActor<APRBullet>(SpawnLocation, SpawnRotation, SpawnParameters))
	{
		Bullet->Init(FirearmData.Damage);
	}
}


