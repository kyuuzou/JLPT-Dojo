// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DojoGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FScoreChangedSignature, int, CurrentStreak, int, MaximumStreak, int, RightAnswers, int, TotalQuestions);

/**
 * 
 */
UCLASS()
class JLPTDOJO_API ADojoGameState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	FScoreChangedSignature OnScoreChanged;

	void BroadcastScoreChanged();
	int GetNextQuestion();
	void Initialise(int TotalQuestions);
	void ProcessCorrectAnswer(int QuestionIndex);
	void ProcessWrongAnswer();

private:
	int _CurrentStreak;
	int _MaximumStreak;
	int _RightAnswers;
	int _TotalQuestions;

	TArray<int> RemainingQuestions;
};
