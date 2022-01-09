// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProgressMeter.generated.h"

UCLASS()
class PORTFOLIO_API AProgressMeter : public AActor
{
	GENERATED_BODY()
	
public:	
	AProgressMeter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UText3DComponent* text3dComponent;

	UFUNCTION()
	void OnProgressChanged(int progress);
};
