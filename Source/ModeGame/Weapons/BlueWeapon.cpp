// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BlueWeapon.h"

bool ABlueWeapon::TryBeginFire()
{
	return (Super::TryBeginFire() && ReceiveTestFire());
}

bool ABlueWeapon::TryEndFire()
{
	return (Super::TryEndFire() && ReceiveEndFire());
}
