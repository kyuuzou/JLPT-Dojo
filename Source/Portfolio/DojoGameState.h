// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DojoGameState.generated.h"

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
	ABlackboardActor* TopBlackboard;
};
