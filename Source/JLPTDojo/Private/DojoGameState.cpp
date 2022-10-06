// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameState.h"

void ADojoGameState::BroadcastScoreChanged() const {
	this->OnScoreChanged.Broadcast(this->CurrentStreak, this->MaximumStreak, this->RightAnswers, this->TotalQuestions);
}

uint32 ADojoGameState::GetNextQuestion() const {
	return FMath::RandRange(0, this->RemainingQuestions.Num() - 1);
}

void ADojoGameState::Initialise(int TotalQuestions_) {
	this->TotalQuestions = TotalQuestions_;
	this->CurrentStreak = this->RightAnswers = 0;

	this->RemainingQuestions.Reset();

	for (int i = 0; i < this->TotalQuestions; i++) {
		this->RemainingQuestions.Add(i);
	}

	this->BroadcastScoreChanged();
}

void ADojoGameState::ProcessCorrectAnswer(int QuestionIndex) {
	this->RightAnswers++;
	this->CurrentStreak++;

	if (this->CurrentStreak > this->MaximumStreak) {
		this->MaximumStreak++;
	}

	this->RemainingQuestions.Remove(QuestionIndex);

	this->BroadcastScoreChanged();
}

void ADojoGameState::ProcessWrongAnswer() {
	this->CurrentStreak = 0;
	this->BroadcastScoreChanged();
}
