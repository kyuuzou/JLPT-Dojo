// Fill out your copyright notice in the Description page of Project Settings.


#include "DojoGameState.h"

void ADojoGameState::TriggerOnProgressChanged() {
	this->OnProgressChanged.Broadcast(this->Progress);
}