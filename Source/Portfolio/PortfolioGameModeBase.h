// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortfolioGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API APortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* PawnKilled);
};
