// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay() {
	this->Hud = CreateWidget(this, this->HudClass);

	if (this->Hud != nullptr) {
		this->Hud->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) {
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	this->Hud->RemoveFromViewport();

	UUserWidget* OutcomeScreen = CreateWidget(
		this,
		bIsWinner ? this->VictoryScreenClass : this->GameOverScreenClass
	);

	if (OutcomeScreen != nullptr) {
		OutcomeScreen->AddToViewport();
	}

	this->GetWorldTimerManager().SetTimer(
		this->RestartTimer, 
		this, 
		&APlayerController::RestartLevel, 
		this->RestartDelay
	);
}