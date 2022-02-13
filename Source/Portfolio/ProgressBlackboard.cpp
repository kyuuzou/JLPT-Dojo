// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressBlackboard.h"

#include "DojoGameState.h"
#include "QuestionBlackboard.h"
#include "Text3DComponent.h"

TArray<FString> ArabicToKanji = {
	TEXT("零"),
	TEXT("一"),
	TEXT("二"),
	TEXT("三"),
	TEXT("四"),
	TEXT("五"),
	TEXT("六"),
	TEXT("七"),
	TEXT("八"),
	TEXT("九"),
	TEXT("十")
};

void AProgressBlackboard::BeginPlay() {
	Super::BeginPlay();

	ADojoGameState* gameState = Cast<ADojoGameState>(this->GetWorld()->GetGameState());
	gameState->OnProgressChanged.AddDynamic(this, &AProgressBlackboard::OnProgressChanged);

	this->Disable();
}

void AProgressBlackboard::Disable() {
	if (!this->Enabled) {
		return;
	}

	Super::Disable();

	FVector location = this->GetActorLocation();
	location.Z -= 250.0f;
	this->SetActorLocation(location);

	this->OnProgressChanged(0);
}

void AProgressBlackboard::Enable() {
	if (this->Enabled) {
		return;
	}

	Super::Enable();

	FVector location = this->GetActorLocation();
	location.Z += 250.0f;
	this->SetActorLocation(location);
}

void AProgressBlackboard::Initialise(class AQuestionBlackboard* _QuestionBlackboard) {
	this->QuestionBlackboard = _QuestionBlackboard;
}

void AProgressBlackboard::OnProgressChanged(int Progress) {
	FString ProgressText;

	if (Progress > 10) {
		ProgressText = TEXT("Next Level");
	} else if (Progress < -3) {
		ProgressText = TEXT("Game Over");
	} else if (Progress == -3) {
		ProgressText = TEXT("-3");
	} else if (Progress == -2) {
		ProgressText = TEXT("-2");
	} else if (Progress == -1) {
		ProgressText = TEXT("-1");
	} else {
		ProgressText = ArabicToKanji[Progress];
	}

	ProgressText = ProgressText.Append(TEXT("\n\n«shoot me to stop»"));

	UText3DComponent* textComponent = this->FindComponentByClass<UText3DComponent>();
	textComponent->SetText(FText::FromString(ProgressText));
}

float AProgressBlackboard::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) {
	this->QuestionBlackboard->Disable();

	return 0.0f;
}
