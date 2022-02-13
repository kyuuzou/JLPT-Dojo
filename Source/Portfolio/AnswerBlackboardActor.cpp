// Fill out your copyright notice in the Description page of Project Settings.


#include "AnswerBlackboardActor.h"

#include "QuestionBlackboard.h"

void AAnswerBlackboardActor::Disable() {
	if (!this->Enabled) {
		return;
	}

	Super::Disable();

	FVector location = this->GetActorLocation();
	location.Z -= 250.0f;
	this->SetActorLocation(location);
}

void AAnswerBlackboardActor::Enable() {
	if (this->Enabled) {
		return;
	}

	Super::Enable();

	FVector location = this->GetActorLocation();
	location.Z += 250.0f;
	this->SetActorLocation(location);
}

void AAnswerBlackboardActor::Initialise(class AQuestionBlackboard* _QuestionBlackboard) {
	this->QuestionBlackboard = _QuestionBlackboard;
}

float AAnswerBlackboardActor::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) {
	this->QuestionBlackboard->OnChosenAnswer(this);

	return 0.0f;
}
