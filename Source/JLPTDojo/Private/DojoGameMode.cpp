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

template<typename ... TFString>
FString ADojoGameMode::DetermineCommonSuffix(TFString... Words) const {
	const auto WordList = { Words... };
	const size_t WordCount = WordList.size();

	if (WordCount == 0) {
		return FString();
	}

	FString FirstWord = *WordList.begin();

	if (WordCount == 1) {
		return FirstWord;
	}

	int SameCharacters = 0;
	bool Finished = false;

	do {
		TCHAR CommonCharacter = FirstWord[FirstWord.Len() - SameCharacters - 1];

		for (auto it = WordList.begin() + 1; it != WordList.end(); ++it) {
			FString Word = *it;
			int WordLength = Word.Len();

			if (SameCharacters >= WordLength) {
				Finished = true;
				break;
			}

			if (CommonCharacter != Word[WordLength - SameCharacters - 1]) {
				Finished = true;
				break;
			}
		}

		if (Finished) {
			break;
		}

		SameCharacters++;
	} while (SameCharacters < FirstWord.Len());

	if (SameCharacters == 0) {
		return FString();
	}

	return FirstWord.Right(SameCharacters);
}

FVocabularyTableRow* ADojoGameMode::GetTableRow(int Index) const {
	if (Index < 0 || Index > this->FilteredRowNames.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Could not find row on Vocabulary table: %d. Total rows: %d."), Index, this->FilteredRowNames.Num());
		return nullptr;
	}

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
	int FilteredRowsIndex = -1;

	for (int i = 0; i < RowNames.Num(); i ++) {
		FName RowName = RowNames[i];
		FVocabularyTableRow* Row = this->GetTableRow(RowName);

		// Filter all the rows that have the same reading as the question
		if (Row->Word == Row->Reading) {
			continue;
		}

		this->FilteredRowNames.Add(RowName);
		FilteredRowsIndex++;

		// Build suffix map
		FString Suffix = this->DetermineCommonSuffix(Row->Word, Row->Reading);

		if (Suffix.Len() > 0) {
			if (! this->ReadingsWithSameEnding.Contains(Suffix)){
				this->ReadingsWithSameEnding.Add(Suffix);
			}

			this->ReadingsWithSameEnding[Suffix].Add(FilteredRowsIndex);
		}
	}
}

void ADojoGameMode::InitialiseState() {
	this->DojoGameState = this->GetGameState<ADojoGameState>();

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

	FVocabularyTableRow* QuestionRow = this->GetTableRow(RightAnswerIndex);
	this->QuestionBoard->Reset();
	this->QuestionBoard->SetCaption(FText::FromString(QuestionRow->Word));

	FString Suffix = this->DetermineCommonSuffix(QuestionRow->Word, QuestionRow->Reading);

	TArray<uint32> Answers { RightAnswerIndex };
	int TotalAnswerBoards = this->AnswerBoards.Num();

	// Prioritise answers that have the same suffix
	if (this->ReadingsWithSameEnding.Contains(Suffix)) {
		TArray<uint32> SameEndingAnswers = this->ReadingsWithSameEnding[Suffix];
		SameEndingAnswers.Remove(RightAnswerIndex);

		while (SameEndingAnswers.Num() > 0 && Answers.Num() < TotalAnswerBoards) {
			int Index = FMath::RandRange(0, SameEndingAnswers.Num() - 1);
			Answers.Add(SameEndingAnswers[Index]);
			SameEndingAnswers.RemoveAt(Index);
		}
	}

	// Fill out the rest of the array with any other answers, regardless of their reading
	while (Answers.Num() < TotalAnswerBoards) {
		int Index = FMath::RandRange(0, this->FilteredRowNames.Num() - 1);

		if (Answers.Contains(Index)) {
			continue;
		}

		Answers.Add(Index);
	}

	for (AAnswerBoard* AnswerBoard : this->AnswerBoards) {
		int AnswerIndex = FMath::RandRange(0, Answers.Num() - 1);
		int RowIndex = Answers[AnswerIndex];
		Answers.RemoveAt(AnswerIndex);

		FVocabularyTableRow* AnswerRow = this->GetTableRow(RowIndex);
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

	this->InitialiseData();
	this->InitialiseActors();
	this->InitialiseState();

	this->SetRandomQuestion();
}
