// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HudClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GameOverScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> VictoryScreenClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;

	UPROPERTY()
	UUserWidget* Hud;

	FTimerHandle RestartTimer;
};
