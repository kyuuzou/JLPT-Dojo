// Fill out your copyright notice in the Description page of Project Settings.


#include "DojoGameMode.h"

#include "BlackboardActor.h"
#include "DojoGameState.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "FQuestion.h"
#include "Kismet/GameplayStatics.h"

ADojoGameMode::ADojoGameMode() {
	this->HandlingAnswer = true;
}

void ADojoGameMode::BeginPlay() {
    this->GameState = this->GetGameState<ADojoGameState>();
	this->InitializeBoards();

	FGenericPlatformMath::SRandInit(time(0));
	this->SetRandomQuestion();
}

void ADojoGameMode::Clear() {

}

void ADojoGameMode::InitializeBoards() {
	const FName ignoreTag("Ignore");
	const FName questionTag("Question");

	for (TActorIterator<ABlackboardActor> blackboardIterator(this->GetWorld()); blackboardIterator; ++blackboardIterator) {
		if (blackboardIterator->ActorHasTag(ignoreTag)) {
			continue;
		}

		if (blackboardIterator->ActorHasTag(questionTag)) {
			this->GameState->QuestionBlackboard = *blackboardIterator;
		} else {
			this->GameState->AnswerBlackboards.Add(*blackboardIterator);
			blackboardIterator->OnBlackboardHit.AddDynamic(this, &ADojoGameMode::OnBlackboardHit);
		}

		this->GameState->Blackboards.Add(*blackboardIterator);
	}
}

void ADojoGameMode::OnBlackboardHit(ABlackboardActor* blackboard) {
	if (this->HandlingAnswer || blackboard == this->GameState->QuestionBlackboard) {
		return;
	}

	this->HandlingAnswer = true;

	bool correct = blackboard == this->RightAnswerBoard;

	if (correct) {
		blackboard->OnCorrectAnswer();

		this->GameState->Progress++;
		this->GameState->RightAnswers++;
	} else {
		blackboard->OnWrongAnswer();
		this->RightAnswerBoard->OnCorrectAnswer();

		this->GameState->Progress--;
		this->GameState->WrongAnswers++;
	}

	this->GameState->TriggerOnProgressChanged();

	FTimerHandle timerHandle;
	this->GetWorldTimerManager().SetTimer(
		timerHandle,
		this,
		&ADojoGameMode::SetRandomQuestion,
		3.0f
	);
}

void ADojoGameMode::SetRandomQuestion() {
	if (this->GameState->Progress < -3) {
		this->GameState->Reset();
	}

	TArray<FName> RowNames = this->DataTable->GetRowNames();
	int index = FMath::FRandRange(0, RowNames.Num());

	this->SetQuestion(index);
}

void ADojoGameMode::SetQuestion(int index) {
	this->Clear();

	// position strings
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	FQuestion* question = this->DataTable->FindRow<FQuestion>(RowNames[index], TEXT("Blackboard"));

	TArray<FString> answers = question->Answers;

	int rightAnswerIndex = FCString::Atoi(*question->RightAnswer) - 1;
	FString rightAnswer = question->Answers[rightAnswerIndex];

	// shuffle answers
	for (int i = answers.Num() - 1; i > 0; i--) {
		int j = FMath::FloorToInt(FMath::SRand() * (i + 1)) % answers.Num();
		FString temp = answers[i];
		answers[i] = answers[j];
		answers[j] = temp;
	}

	this->GameState->QuestionBlackboard->SetCaption(question->Sentence);

	for (int i = 0; i < answers.Num(); i++) {
		bool isRightAnswer = answers[i] == rightAnswer;

		ABlackboardActor* answerBlackboard = this->GameState->AnswerBlackboards[i];
		answerBlackboard->SetCaption(*answers[i]);

		if (isRightAnswer) {
			this->RightAnswerBoard = answerBlackboard;
		}
	}

	this->HandlingAnswer = false;
}


