// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestionBlackboard.h"

#include "AnswerBlackboardActor.h"
#include "DojoGameState.h"
#include "Engine/DataTable.h"
#include "FQuestion.h"
#include "ProgressBlackboard.h"
#include "Text3DComponent.h"

AQuestionBlackboard::AQuestionBlackboard() {
	this->RightAnswerBoard = nullptr;
	this->HandlingAnswer = false;
}

void AQuestionBlackboard::BeginPlay() {
	Super::BeginPlay();

	this->DetermineIdealTextSize();

	for (AAnswerBlackboardActor* AnswerBlackboard : AnswerBlackboards) {
		AnswerBlackboard->Initialise(this);
	}

	this->ProgressMeter->Initialise(this);
	this->Disable();
}

void AQuestionBlackboard::DetermineIdealTextSize() {
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	int MaxLength = INT_MIN;
	FQuestion* MaxLengthQuestion = nullptr;
	
	for (int i = 0; i < RowNames.Num(); i++) {
		FQuestion* Question = this->DataTable->FindRow<FQuestion>(RowNames[i], TEXT("Blackboard"));
		int Length = Question->Sentence.Len();

		if (Length > MaxLength) {
			MaxLength = Length;
			MaxLengthQuestion = Question;
		}
	}

	this->SetCaption(FString::Printf(TEXT("%s\n\n«shoot to continue»"), *MaxLengthQuestion->Sentence));

	UText3DComponent* TextComponent = this->FindComponentByClass<UText3DComponent>();
	USceneComponent* TextRoot = static_cast<USceneComponent*>(TextComponent->GetDefaultSubobjectByName(TEXT("TextRoot")));
	this->IdealTextSize = TextRoot->GetRelativeScale3D();
}

void AQuestionBlackboard::Disable() {
	Super::Disable();

	for (AAnswerBlackboardActor* answerBlackboard : this->AnswerBlackboards) {
		answerBlackboard->Disable();
	}

	this->ProgressMeter->Disable();
	this->GetWorldTimerManager().ClearAllTimersForObject(this);
	this->SetCaption(TEXT("JLPT N5\n\n«shoot to start»"));
}

void AQuestionBlackboard::Enable() {
	if (this->Enabled) {
		return;
	}

	Super::Enable();

	for (AAnswerBlackboardActor* answerBlackboard : this->AnswerBlackboards) {
		answerBlackboard->Enable();
	}

	this->ProgressMeter->Enable();
}

void AQuestionBlackboard::OnChosenAnswer(AAnswerBlackboardActor* ChosenBoard) {
	if (this->HandlingAnswer || this->RightAnswerBoard == nullptr) {
		return;
	}

	this->HandlingAnswer = true;

	ADojoGameState* GameState = this->GetWorld()->GetGameState<ADojoGameState>();
	bool IsRightAnswer = ChosenBoard == this->RightAnswerBoard;

	if (IsRightAnswer) {
		ChosenBoard->OnCorrectAnswer();
		GameState->Progress++;
		GameState->RightAnswers++;
	} else {
		ChosenBoard->OnWrongAnswer();
		this->RightAnswerBoard->OnCorrectAnswer();

		GameState->Progress--;
		GameState->WrongAnswers++;
	}

	this->SetFilledCaption(
		FString::Printf(TEXT("%s\n\n«shoot to continue»"), *this->CurrentQuestion->Sentence),
		*FString::Printf(TEXT("[%s]"), *this->RightAnswer)
	);
	this->SetTextSize(this->IdealTextSize);

	GameState->TriggerOnProgressChanged();

	// TODO: make it configurable whether or not we want to move to the next question automatically
	// FTimerHandle timerHandle;
	// this->GetWorldTimerManager().SetTimer(timerHandle, this, &AQuestionBlackboard::SetRandomQuestion, 3.0f);
}

void AQuestionBlackboard::SetFilledCaption(FString Sentence, const TCHAR* Filler) {
	int Index = Sentence.Find(TEXT("_"));
	Sentence = Sentence.Replace(TEXT("_"), TEXT(""));
	Sentence.InsertAt(Index, Filler);
	this->SetCaption(Sentence);
}

void AQuestionBlackboard::SetRandomQuestion() {
	ADojoGameState* GameState = this->GetWorld()->GetGameState<ADojoGameState>();
	
	if (GameState->Progress < -3) {
		GameState->Reset();
	}

	TArray<FName> RowNames = this->DataTable->GetRowNames();
	int index = FMath::FRandRange(0, RowNames.Num());

	this->SetQuestion(index);
}

void AQuestionBlackboard::SetQuestion(int Index) {
	ADojoGameState* GameState = this->GetWorld()->GetGameState<ADojoGameState>();

	// position strings
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	FQuestion* Question = this->DataTable->FindRow<FQuestion>(RowNames[Index], TEXT("Blackboard"));
	this->CurrentQuestion = Question;

	TArray<FString> Answers = Question->Answers;

	int _RightAnswerIndex = FCString::Atoi(*Question->RightAnswer) - 1;
	FString _RightAnswer = Question->Answers[_RightAnswerIndex];
	this->RightAnswer = _RightAnswer;

	// shuffle answers
	for (int i = Answers.Num() - 1; i > 0; i--) {
		int j = FMath::FloorToInt(FMath::SRand() * (i + 1)) % Answers.Num();
		FString temp = Answers[i];
		Answers[i] = Answers[j];
		Answers[j] = temp;
	}

	this->SetColor(FLinearColor::White);
	this->SetFilledCaption(
		FString::Printf(TEXT("%s\n\n«shoot to pass»"), *Question->Sentence),
		TEXT("[...]")
	);

	this->SetTextSize(this->IdealTextSize);

	for (int i = 0; i < Answers.Num() && i < this->AnswerBlackboards.Num(); i++) {
		bool IsRightAnswer = Answers[i] == _RightAnswer;

		AAnswerBlackboardActor* AnswerBlackboard = this->AnswerBlackboards[i];
		AnswerBlackboard->SetCaption(*Answers[i]);
		AnswerBlackboard->SetColor(FLinearColor::White);

		if (IsRightAnswer) {
			this->RightAnswerBoard = AnswerBlackboard;
		}
	}

	this->HandlingAnswer = false;
}

float AQuestionBlackboard::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) {
	this->Enable();
	this->SetRandomQuestion();

	return 0.0f;
}
