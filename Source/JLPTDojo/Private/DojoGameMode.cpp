// Copyright (c) 2021 Nelson Rodrigues

#include "DojoGameMode.h"

#include "AnswerBoard.h"
#include "DataTableWrapper.h"
#include "Delegates/Delegate.h"
#include "DojoGameState.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "GameplayStaticsExtended.h"
#include "JLPTLevelButton.h"
#include "MeaningBoard.h"
#include "NextButton.h"
#include "QuestionBoardActor.h"

DataTableWrapper* ADojoGameMode::GetCurrentData() {
	return this->DataPerJLPTLevel[this->CurrentDifficultyLevel];
}

void ADojoGameMode::InitialiseActors() {
	UGameplayStaticsExtended::GetAllActorsOfSubclass(this->GetWorld(), this->AnswerBoards);

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		AnswerBoard->OnActorBeginOverlap.AddUniqueDynamic(this, &ADojoGameMode::OnAnswerBoardBeginOverlap);
	}

	TArray<AJLPTLevelButton*> LevelButtons;
	UGameplayStaticsExtended::GetAllActorsOfSubclass(this->GetWorld(), LevelButtons);

	for (AJLPTLevelButton* LevelButton : LevelButtons) {
		LevelButton->OnActorBeginOverlap.AddUniqueDynamic(this, &ADojoGameMode::OnLevelButtonBeginOverlap);
	}

	this->NextButton = UGameplayStatics::GetActorOfClass(this->GetWorld(), ANextButton::StaticClass());
	this->NextButton->OnActorHit.AddUniqueDynamic(this, &ADojoGameMode::OnNextButtonHit);

	this->MeaningBoard = UGameplayStaticsExtended::GetActorOfSubclass<AMeaningBoard>(this->GetWorld());
	this->QuestionBoard = UGameplayStaticsExtended::GetActorOfSubclass<AQuestionBoardActor>(this->GetWorld());
}

void ADojoGameMode::InitialiseData() {
	for (int i = 1; i <= 5; i++) {
		DataTableWrapper* Wrapper = new DataTableWrapper(this->DataTable, i);
		Wrapper->Initialise();
		this->DataPerJLPTLevel.Add(i, Wrapper);
	}
}

void ADojoGameMode::InitialiseState() {
	this->DojoGameState = this->GetGameState<ADojoGameState>();
	this->DojoGameState->Initialise(this->GetCurrentData()->GetTotalQuestions());
}

void ADojoGameMode::OnAnswerBoardBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	AAnswerBoard* AnswerBoard = Cast<AAnswerBoard>(OverlappedActor);
	int AnswerIndex = AnswerBoard->GetCurrentIndex();
	FVocabularyTableRow* Row = this->GetCurrentData()->GetTableRow(AnswerIndex);
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

void ADojoGameMode::OnLevelButtonBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	AJLPTLevelButton* LevelButton = Cast<AJLPTLevelButton>(OverlappedActor);

	this->CurrentDifficultyLevel = LevelButton->GetLevel();
	this->DojoGameState->Initialise(this->GetCurrentData()->GetTotalQuestions());
	this->SetRandomQuestion();
}

void ADojoGameMode::OnNextButtonHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	if (! this->AllowSkip) {
		return;
	}

	this->AllowSkip = false;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, 
		FTimerDelegate::CreateLambda([this] { this->AllowSkip = true; }),
		this->AllowSkipCooldownInSeconds,
		false
	);

	this->SetRandomQuestion();
}

void ADojoGameMode::SetRandomQuestion() {
	uint32 RightAnswerIndex = this->DojoGameState->GetNextQuestion();
	DataTableWrapper* Data = this->GetCurrentData();

	FVocabularyTableRow* QuestionRow = Data->GetTableRow(RightAnswerIndex);
	this->QuestionBoard->Reset();
	this->QuestionBoard->SetCaption(FText::FromString(QuestionRow->Word));

	TArray<uint32> Answers = Data->GetAnswers(RightAnswerIndex, this->AnswerBoards.Num());

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		int AnswerIndex = FMath::RandRange(0, Answers.Num() - 1);
		int RowIndex = Answers[AnswerIndex];
		Answers.RemoveAt(AnswerIndex);

		FVocabularyTableRow* AnswerRow = Data->GetTableRow(RowIndex);
		AnswerBoard->SetAnswer(AnswerRow->Reading, RowIndex, RowIndex == RightAnswerIndex);
		AnswerBoard->Reset();

		if (RightAnswerIndex == RowIndex) {
			this->RightAnswerBoard = AnswerBoard;
		}
	}

	this->MeaningBoard->Reset();
	this->HandlingAnswer = false;
}

void ADojoGameMode::StartPlay() {
	Super::StartPlay();

	this->HandlingAnswer = false;
	this->AllowSkip = true;
	this->CurrentDifficultyLevel = 5;

	this->InitialiseData();
	this->InitialiseActors();
	this->InitialiseState();

	this->SetRandomQuestion();
}
