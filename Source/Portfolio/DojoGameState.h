// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DojoGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressChanged, int, progress);

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ADojoGameState : public AGameStateBase
{
	GENERATED_BODY()
		
public:
	UPROPERTY(VisibleAnywhere)
	TArray<class ABlackboardActor*> AnswerBlackboards;

	UPROPERTY(VisibleAnywhere)
	TArray<class ABlackboardActor*> Blackboards;

	UPROPERTY(VisibleAnywhere)
	int JLPTLevel;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnProgressChanged OnProgressChanged;

	UPROPERTY(VisibleAnywhere)
	int Progress;

	UPROPERTY(VisibleAnywhere)
	int RightAnswers;

	UPROPERTY(VisibleAnywhere)
	ABlackboardActor* QuestionBlackboard;

	UPROPERTY(VisibleAnywhere)
	int WrongAnswers;

	virtual void Reset() override;

	UFUNCTION()
	void TriggerOnProgressChanged();
};
