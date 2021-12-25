// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackboardActor.generated.h"

UCLASS()
class PORTFOLIO_API ABlackboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackboardActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<FString> questions;

	UPROPERTY()
	TArray<class UText3DComponent*> textComponents;
};
