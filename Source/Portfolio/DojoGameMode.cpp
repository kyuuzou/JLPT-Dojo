// Fill out your copyright notice in the Description page of Project Settings.


#include "DojoGameMode.h"

void ADojoGameMode::BeginPlay() {
	FGenericPlatformMath::SRandInit(time(0));
}


