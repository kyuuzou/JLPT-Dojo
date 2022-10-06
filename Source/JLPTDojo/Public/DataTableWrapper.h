// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class JLPTDOJO_API DataTableWrapper
{
public:
	DataTableWrapper(class UDataTable* DataTable, int JLPTLevel);
	~DataTableWrapper();

	TArray<uint32> GetAnswers(uint32 QuestionIndex, int TotalNeededAnswers) const;
	struct FVocabularyTableRow* GetTableRow(int Index) const;
	int GetTotalQuestions() const;
	void Initialise();

private:
	class UDataTable* DataTable;
	TArray<FName> FilteredRowNames;
	int JLPTLevel;
	TMap<FString, TArray<uint32>> ReadingsWithSameEnding;

	template<typename ... TFString>
	FString DetermineCommonSuffix(TFString... Words) const;

	struct FVocabularyTableRow* GetTableRow(FName RowName) const;
};
