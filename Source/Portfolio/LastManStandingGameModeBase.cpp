// Fill out your copyright notice in the Description page of Project Settings.

#include "LastManStandingGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void ALastManStandingGameModeBase::EndGame(bool playerIsWinner) {
	for (AController* controller : TActorRange<AController>(this->GetWorld())) {
		controller->GameHasEnded(
			controller->GetPawn(),
			playerIsWinner == controller->IsPlayerController()
		);
	}
}

void ALastManStandingGameModeBase::PawnKilled(APawn* PawnKilled) {
	Super::PawnKilled(PawnKilled);

	APlayerController* playerController = Cast<APlayerController>(PawnKilled->GetController());

	if (playerController == nullptr) {
		for (AShooterAIController* controller : TActorRange<AShooterAIController>(this->GetWorld())) {
			if (!controller->IsDead()) {
				return;
			}
		}

		this->EndGame(true);
	} else {
		this->EndGame(false);
	}
}
