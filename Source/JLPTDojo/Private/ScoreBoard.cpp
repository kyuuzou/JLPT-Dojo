// Copyright (c) 2021 Nelson Rodrigues

#include "ScoreBoard.h"

#include "Engine/DataTable.h"
#include "Delegates/Delegate.h"
#include "DojoGameState.h"
#include "Text3DComponent.h"

AScoreBoard::AScoreBoard() {
	PrimaryActorTick.bCanEverTick = true;
}

void AScoreBoard::BeginPlay() {
	Super::BeginPlay();

	ADojoGameState* GameState = this->GetWorld()->GetGameState<ADojoGameState>();
	GameState->OnScoreChanged.AddUniqueDynamic(this, &AScoreBoard::OnScoreChanged);
}

void AScoreBoard::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ADojoGameState* GameState = this->GetWorld()->GetGameState<ADojoGameState>();
	GameState->OnScoreChanged.RemoveAll(this);
}

void AScoreBoard::OnScoreChanged(int CurrentStreak, int MaximumStreak, int RightAnswers, int TotalQuestions) {
	FString Caption = FString::Format(*this->TextTemplate, { RightAnswers, TotalQuestions, CurrentStreak, MaximumStreak });
	this->TextComponent->SetText(FText::FromString(Caption));
}

void AScoreBoard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
}

void AScoreBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

