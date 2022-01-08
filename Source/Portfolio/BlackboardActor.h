// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackboardActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlackboardHit, ABlackboardActor*, Board);

UCLASS()
class PORTFOLIO_API ABlackboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnBlackboardHit OnBlackboardHit;
	
	ABlackboardActor();
	void BePointedAt(FHitResult HitResult);
	void OnCorrectAnswer();
	void OnWrongAnswer();
	void SetCaption(FString caption);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

private:
	void Clear();
	void SetColor(FLinearColor Color);
};
