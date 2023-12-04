// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenWeapon.h"

AGreenWeapon::AGreenWeapon()
{
}

void AGreenWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGreenWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGreenWeapon::TryBeginFire()
{
	ReceiveTestFire();
	return Super::TryBeginFire() && false;
}


bool AGreenWeapon::TryEndFire()
{
	return Super::TryEndFire() && false;
}

