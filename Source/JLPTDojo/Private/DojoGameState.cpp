// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameState.h"

void ADojoGameState::BroadcastScoreChanged() {
	this->OnScoreChanged.Broadcast(this->CurrentStreak, this->MaximumStreak, this->RightAnswers, this->TotalQuestions);
}

int ADojoGameState::GetNextQuestion() {
	return FMath::FRandRange(0.0f, this->RemainingQuestions.Num());
}

void ADojoGameState::Initialise(int TotalQuestions_) {
	this->TotalQuestions = TotalQuestions_;

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
