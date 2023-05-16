// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PRBullet.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APRBullet::APRBullet()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(1.5f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Collision;

	if(GetLocalRole() == ROLE_Authority)
	{
		Collision->OnComponentHit.AddDynamic(this, &APRBullet::OnHit);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Item/Consumable/Ammunition/Mesh/SM_Bullet.SM_Bullet"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetRelativeLocationAndRotation(FVector(-1.8f, 0.0f, 0.0f), FRotator(-90.0f, 0.0f, 0.0f));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Mesh.Succeeded())
	{
		BulletMesh->SetStaticMesh(Mesh.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void APRBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APRBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APRBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
	}
	Destroy();
}

void APRBullet::Init(float InDamage)
{
	Damage = InDamage;
}

