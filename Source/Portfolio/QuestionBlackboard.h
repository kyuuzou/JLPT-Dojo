// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackboardActor.h"
#include "QuestionBlackboard.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AQuestionBlackboard : public ABlackboardActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<class AAnswerBlackboardActor*> AnswerBlackboards;

	UPROPERTY(EditAnywhere)
	class UDataTable* DataTable;

	UPROPERTY(EditAnywhere)
	class AProgressBlackboard* ProgressMeter;

	AQuestionBlackboard();
	virtual void Disable() override;
	virtual void Enable() override;
	void OnChosenAnswer(class AAnswerBlackboardActor* ChosenBoard);

protected:
	virtual void BeginPlay() override;
	void SetFilledCaption(FString Sentence, const TCHAR* Filler);
	void SetRandomQuestion();
	void SetQuestion(int index);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

private:
	struct FQuestion* CurrentQuestion;
	FVector IdealTextSize;
	FString RightAnswer;
	bool HandlingAnswer;
	class AAnswerBlackboardActor* RightAnswerBoard;

	void DetermineIdealTextSize();
};
