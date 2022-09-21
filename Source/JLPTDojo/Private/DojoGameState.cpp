// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameState.h"

void ADojoGameState::BroadcastScoreChanged() {
	this->OnScoreChanged.Broadcast(_CurrentStreak, _MaximumStreak, _RightAnswers, _TotalQuestions);
}

int ADojoGameState::GetNextQuestion() {
	return FMath::FRandRange(0.0f, this->RemainingQuestions.Num());
}

void ADojoGameState::Initialise(int TotalQuestions) {
	_TotalQuestions = TotalQuestions;

	this->RemainingQuestions.Reset();

	for (int i = 0; i < TotalQuestions; i++) {
		this->RemainingQuestions.Add(i);
	}

	this->BroadcastScoreChanged();
}

void ADojoGameState::ProcessCorrectAnswer(int QuestionIndex) {
	_RightAnswers++;
	_CurrentStreak++;

	if (_CurrentStreak > _MaximumStreak) {
		_MaximumStreak++;
	}

	this->RemainingQuestions.Remove(QuestionIndex);

	this->BroadcastScoreChanged();
}

void ADojoGameState::ProcessWrongAnswer() {
	_CurrentStreak = 0;
	this->BroadcastScoreChanged();
}
