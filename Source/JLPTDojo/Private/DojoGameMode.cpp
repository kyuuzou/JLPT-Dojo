// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameMode.h"

#include "AnswerBoardActor.h"
#include "Delegates/Delegate.h"
#include "DojoGameState.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "GameplayStaticsExtended.h"
#include "MeaningBoard.h"
#include "NextButton.h"
#include "QuestionBoardActor.h"

FVocabularyTableRow* ADojoGameMode::GetTableRow(int Index) {
	return this->GetTableRow(this->FilteredRowNames[Index]);
}

FVocabularyTableRow* ADojoGameMode::GetTableRow(FName RowName) {
	return this->DataTable->FindRow<FVocabularyTableRow>(RowName, TEXT("Vocabulary Table"));
}

void ADojoGameMode::InitialiseActors() {
	UGameplayStaticsExtended::GetAllActorsOfSubclass(this->GetWorld(), this->AnswerBoards);

	for (AAnswerBoardActor* answerBoard : this->AnswerBoards) {
		answerBoard->OnActorBeginOverlap.AddUniqueDynamic(this, &ADojoGameMode::OnAnswerBoardBeginOverlap);
	}

	this->NextButton = UGameplayStatics::GetActorOfClass(this->GetWorld(), ANextButton::StaticClass());
	this->NextButton->OnActorHit.AddUniqueDynamic(this, &ADojoGameMode::OnNextButtonHit);

	this->MeaningBoard = UGameplayStaticsExtended::GetActorOfSubclass<AMeaningBoard>(this->GetWorld());
	this->QuestionBoard = UGameplayStaticsExtended::GetActorOfSubclass<AQuestionBoardActor>(this->GetWorld());
}

void ADojoGameMode::InitialiseData() {
	TArray<FName> rowNames = this->DataTable->GetRowNames();

	// Filter all the rows that have the same reading as the question
	for (FName rowName : rowNames) {
		FVocabularyTableRow* row = this->GetTableRow(rowName);
		
		if (row->Word != row->Reading) {
			this->FilteredRowNames.Add(rowName);
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
	AAnswerBoardActor* AnswerBoard = Cast<AAnswerBoardActor>(OverlappedActor);
	int answerIndex = AnswerBoard->GetCurrentIndex();
	FVocabularyTableRow* row = this->GetTableRow(answerIndex);

	this->MeaningBoard->SetCaption(row->Meaning);

	if (this->HandlingAnswer) {
		return;
	}

	this->HandlingAnswer = true;
	this->RightAnswerBoard->SetCorrect(true);

	bool Correct = OverlappedActor == this->RightAnswerBoard;

	this->QuestionBoard->SetCorrect(Correct);

	if (Correct) {
		this->DojoGameState->ProcessCorrectAnswer(answerIndex);
	} else {
		this->DojoGameState->ProcessWrongAnswer();
		AnswerBoard->SetCorrect(false);
	}
}

void ADojoGameMode::OnNextButtonHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	this->SetRandomQuestion();
}

void ADojoGameMode::SetRandomQuestion() {
	int rightAnswerIndex = this->DojoGameState->GetNextQuestion();

	FVocabularyTableRow* row = this->GetTableRow(rightAnswerIndex);
	this->QuestionBoard->SetCaption(FText::FromString(row->Word));

	for (AAnswerBoardActor* answerBoard : this->AnswerBoards) {
		// TODO: redo this so there's no possibility of repeated indexes
		int wrongIndex = FMath::FRandRange(0.0f, this->FilteredRowNames.Num() - 1);

		if (wrongIndex == rightAnswerIndex) {
			wrongIndex++;
		}

		FVocabularyTableRow* wrongRow = this->GetTableRow(wrongIndex);
		answerBoard->SetRandomWrongAnswer(wrongRow->Reading, wrongIndex);
	}

	int rightAnswerBoardIndex = FMath::FRandRange(0.0f, this->AnswerBoards.Num());
	this->AnswerBoards[rightAnswerBoardIndex]->SetRightAnswer(row->Reading, rightAnswerIndex);
	this->RightAnswerBoard = this->AnswerBoards[rightAnswerBoardIndex];

	for (AAnswerBoardActor* answerBoard : this->AnswerBoards) {
		answerBoard->ResetGeometry();
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
