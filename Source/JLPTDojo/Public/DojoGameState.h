// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DojoGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FScoreChangedSignature, int, CurrentStreak, int, MaximumStreak, int, RightAnswers, int, TotalQuestions);

UCLASS()
class JLPTDOJO_API ADojoGameState : public AGameStateBase {
	GENERATED_BODY()

public:
	FScoreChangedSignature OnScoreChanged;

	void BroadcastScoreChanged() const;
	uint32 GetNextQuestion() const;
	void Initialise(int TotalQuestions_);
	void ProcessCorrectAnswer(int QuestionIndex);
	void ProcessWrongAnswer();

private:
	int CurrentStreak;
	int MaximumStreak;
	int RightAnswers;
	int TotalQuestions;

	TArray<uint32> RemainingQuestions;
};
