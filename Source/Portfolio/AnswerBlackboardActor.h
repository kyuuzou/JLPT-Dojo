// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackboardActor.h"
#include "AnswerBlackboardActor.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AAnswerBlackboardActor : public ABlackboardActor
{
	GENERATED_BODY()
	
public:
	virtual void Disable() override;
	virtual void Enable() override;
	void Initialise(class AQuestionBlackboard* questionBlackboard);

protected:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

private:
	class AQuestionBlackboard* QuestionBlackboard;
};
