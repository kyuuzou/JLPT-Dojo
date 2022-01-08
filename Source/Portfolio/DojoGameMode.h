// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DojoGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ADojoGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UDataTable* DataTable;

	class ADojoGameState* GameState;
	bool HandlingAnswer;

	UPROPERTY(VisibleAnywhere)
	class ABlackboardActor* RightAnswerBoard;

	ADojoGameMode();
	void Clear();
	void InitializeBoards();

	UFUNCTION()
	void OnBlackboardHit(class ABlackboardActor* blackboard);

	void SetRandomQuestion();
	void SetQuestion(int index);
};
