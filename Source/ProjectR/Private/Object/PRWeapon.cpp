// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PRWeapon.h"

APRWeapon::APRWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	Collision->SetupAttachment(Mesh, FName(TEXT("CollisionSocket")));
}
