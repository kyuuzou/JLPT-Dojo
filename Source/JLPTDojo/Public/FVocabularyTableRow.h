// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FVocabularyTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FVocabularyTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Level;

	UPROPERTY(EditAnywhere)
	FString Word;

	UPROPERTY(EditAnywhere)
	FString Reading;

	UPROPERTY(EditAnywhere)
	FString Meaning;
};
