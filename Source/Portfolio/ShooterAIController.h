// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	bool IsDead() const;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	/*
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200.0f;
	*/

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
