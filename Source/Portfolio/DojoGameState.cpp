// Fill out your copyright notice in the Description page of Project Settings.


#include "DojoGameState.h"

void ADojoGameState::Reset() {
	this->Progress = 0;
	this->RightAnswers = 0;
	this->WrongAnswers = 0;
	this->JLPTLevel = 5;
	this->TriggerOnProgressChanged();
}

void ADojoGameState::TriggerOnProgressChanged() {
	this->OnProgressChanged.Broadcast(this->Progress);
}
