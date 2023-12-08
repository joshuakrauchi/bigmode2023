// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RedWeapon.h"

bool ARedWeapon::TryBeginFire()
{
	return (Super::TryBeginFire() && ReceiveTestFire());
}

bool ARedWeapon::TryEndFire()
{
	return (Super::TryEndFire() && ReceiveEndFire());
}
