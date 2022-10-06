// Copyright (c) 2021 Nelson Rodrigues

#include "DataTableWrapper.h"

#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"

DataTableWrapper::DataTableWrapper(UDataTable* DataTable, int JLPTLevel) : DataTable(DataTable), JLPTLevel(JLPTLevel) {

}

DataTableWrapper::~DataTableWrapper() {
}

template<typename ... TFString>
FString DataTableWrapper::DetermineCommonSuffix(TFString... Words) const {
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

TArray<uint32> DataTableWrapper::GetAnswers(uint32 QuestionIndex, int TotalNeededAnswers) const {
	FVocabularyTableRow* QuestionRow = this->GetTableRow(QuestionIndex);
	FString Suffix = this->DetermineCommonSuffix(QuestionRow->Word, QuestionRow->Reading);

	TArray<uint32> Answers{ QuestionIndex };

	// Prioritise answers that have the same suffix
	if (this->ReadingsWithSameEnding.Contains(Suffix)) {
		TArray<uint32> SameEndingAnswers = this->ReadingsWithSameEnding[Suffix];
		SameEndingAnswers.Remove(QuestionIndex);

		while (SameEndingAnswers.Num() > 0 && Answers.Num() < TotalNeededAnswers) {
			int Index = FMath::RandRange(0, SameEndingAnswers.Num() - 1);
			Answers.Add(SameEndingAnswers[Index]);
			SameEndingAnswers.RemoveAt(Index);
		}
	}

	// Fill out the rest of the array with any other answers, regardless of their reading
	while (Answers.Num() < TotalNeededAnswers) {
		int Index = FMath::RandRange(0, this->FilteredRowNames.Num() - 1);

		if (Answers.Contains(Index)) {
			continue;
		}

		Answers.Add(Index);
	}

	return Answers;
}

FVocabularyTableRow* DataTableWrapper::GetTableRow(int Index) const {
	if (Index < 0 || Index > this->FilteredRowNames.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Could not find row on Vocabulary table: %d. Total rows: %d."), Index, this->FilteredRowNames.Num());
		return nullptr;
	}

	return this->GetTableRow(this->FilteredRowNames[Index]);
}

FVocabularyTableRow* DataTableWrapper::GetTableRow(FName RowName) const {
	return this->DataTable->FindRow<FVocabularyTableRow>(RowName, TEXT("Vocabulary Table"));
}

int DataTableWrapper::GetTotalQuestions() const {
	return this->FilteredRowNames.Num();
}

void DataTableWrapper::Initialise() {
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	int FilteredRowsIndex = -1;

	for (int i = 0; i < RowNames.Num(); i++) {
		FName RowName = RowNames[i];
		FVocabularyTableRow* Row = this->GetTableRow(RowName);

		// No point testing words written exclusively in hiragana or katakana, at least not in this question format
		if (Row->Level != this->JLPTLevel || Row->Word == Row->Reading) {
			continue;
		}

		this->FilteredRowNames.Add(RowName);
		FilteredRowsIndex++;

		// Build suffix map
		FString Suffix = this->DetermineCommonSuffix(Row->Word, Row->Reading);

		if (Suffix.Len() > 0) {
			if (!this->ReadingsWithSameEnding.Contains(Suffix)) {
				this->ReadingsWithSameEnding.Add(Suffix);
			}

			this->ReadingsWithSameEnding[Suffix].Add(FilteredRowsIndex);
		}
	}
}
