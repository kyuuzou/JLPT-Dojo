// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortfolioGameModeBase.h"
#include "LastManStandingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ALastManStandingGameModeBase : public APortfolioGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool playerIsWinner);
};
