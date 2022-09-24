// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameMode.h"

#include "AnswerBoard.h"
#include "Delegates/Delegate.h"
#include "DojoGameState.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "GameplayStaticsExtended.h"
#include "MeaningBoard.h"
#include "NextButton.h"
#include "QuestionBoardActor.h"

FVocabularyTableRow* ADojoGameMode::GetTableRow(int Index) const {
	return this->GetTableRow(this->FilteredRowNames[Index]);
}

FVocabularyTableRow* ADojoGameMode::GetTableRow(FName RowName) const {
	return this->DataTable->FindRow<FVocabularyTableRow>(RowName, TEXT("Vocabulary Table"));
}

void ADojoGameMode::InitialiseActors() {
	UGameplayStaticsExtended::GetAllActorsOfSubclass(this->GetWorld(), this->AnswerBoards);

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		AnswerBoard->OnActorBeginOverlap.AddUniqueDynamic(this, &ADojoGameMode::OnAnswerBoardBeginOverlap);
	}

	this->NextButton = UGameplayStatics::GetActorOfClass(this->GetWorld(), ANextButton::StaticClass());
	this->NextButton->OnActorHit.AddUniqueDynamic(this, &ADojoGameMode::OnNextButtonHit);

	this->MeaningBoard = UGameplayStaticsExtended::GetActorOfSubclass<AMeaningBoard>(this->GetWorld());
	this->QuestionBoard = UGameplayStaticsExtended::GetActorOfSubclass<AQuestionBoardActor>(this->GetWorld());
}

void ADojoGameMode::InitialiseData() {
	TArray<FName> RowNames = this->DataTable->GetRowNames();

	// Filter all the rows that have the same reading as the question
	for (FName RowName : RowNames) {
		FVocabularyTableRow* Row = this->GetTableRow(RowName);

		if (Row->Word != Row->Reading) {
			this->FilteredRowNames.Add(RowName);
		}
	}
}

void ADojoGameMode::InitialiseState() {
	this->DojoGameState = this->GetGameState<ADojoGameState>();

	// TODO: investigate whether it would not be better to have all of this code in GameInstance instead
	int TotalQuestions = this->FilteredRowNames.Num();
	this->DojoGameState->Initialise(TotalQuestions);
}

void ADojoGameMode::OnAnswerBoardBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	AAnswerBoard* AnswerBoard = Cast<AAnswerBoard>(OverlappedActor);
	int AnswerIndex = AnswerBoard->GetCurrentIndex();
	FVocabularyTableRow* Row = this->GetTableRow(AnswerIndex);

	this->MeaningBoard->SetCaption(Row->Meaning);

	if (this->HandlingAnswer) {
		return;
	}

	this->HandlingAnswer = true;
	this->RightAnswerBoard->SetCorrect(true);

	bool Correct = OverlappedActor == this->RightAnswerBoard;

	this->QuestionBoard->SetCorrect(Correct);

	if (Correct) {
		this->DojoGameState->ProcessCorrectAnswer(AnswerIndex);
	} else {
		this->DojoGameState->ProcessWrongAnswer();
		AnswerBoard->SetCorrect(false);
	}
}

void ADojoGameMode::OnNextButtonHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	this->SetRandomQuestion();
}

void ADojoGameMode::SetRandomQuestion() {
	int RightAnswerIndex = this->DojoGameState->GetNextQuestion();

	FVocabularyTableRow* Row = this->GetTableRow(RightAnswerIndex);
	this->QuestionBoard->Reset();
	this->QuestionBoard->SetCaption(FText::FromString(Row->Word));

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		// TODO: redo this so there's no possibility of repeated indexes
		int WrongIndex = FMath::FRandRange(0.0f, this->FilteredRowNames.Num() - 1);

		if (WrongIndex == RightAnswerIndex) {
			WrongIndex++;
		}

		FVocabularyTableRow* WrongRow = this->GetTableRow(WrongIndex);
		AnswerBoard->SetAnswer(WrongRow->Reading, WrongIndex, false);
	}

	int RightAnswerBoardIndex = FMath::FRandRange(0.0f, this->AnswerBoards.Num());
	this->AnswerBoards[RightAnswerBoardIndex]->SetAnswer(Row->Reading, RightAnswerIndex, true);
	this->RightAnswerBoard = this->AnswerBoards[RightAnswerBoardIndex];

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		AnswerBoard->Reset();
	}

	this->HandlingAnswer = false;
}

void ADojoGameMode::StartPlay() {
	Super::StartPlay();

	this->HandlingAnswer = false;

	this->InitialiseData();
	this->InitialiseActors();
	this->InitialiseState();

	this->SetRandomQuestion();
}
