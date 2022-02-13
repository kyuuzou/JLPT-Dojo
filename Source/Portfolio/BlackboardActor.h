// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackboardActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlackboardHit, ABlackboardActor*, Board);

UCLASS(Abstract)
class PORTFOLIO_API ABlackboardActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnBlackboardHit OnBlackboardHit;
	
	ABlackboardActor();
	void BePointedAt(FHitResult HitResult);
	virtual void Disable();
	virtual void Enable();
	void OnCorrectAnswer();
	void OnWrongAnswer();
	void SetCaption(FString caption);
	void SetColor(FLinearColor Color);
	void SetTextSize(FVector Size);

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override PURE_VIRTUAL(ABlackboardActor::BeginPlay, return 0.0f;);

	bool Enabled;

private:
	void Clear();
};
