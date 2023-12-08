// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/GreyWeapon.h"

bool AGreyWeapon::TryBeginFire()
{
	return (Super::TryBeginFire() && ReceiveTestFire());
}

bool AGreyWeapon::TryEndFire()
{
	return (Super::TryEndFire() && ReceiveEndFire());
}
