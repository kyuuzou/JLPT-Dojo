// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Misc/AssertionMacros.h"

#include "KanaTableRow.generated.h"

USTRUCT(BlueprintType)
struct FKanaTableRow : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Hiragana;

	UPROPERTY(EditAnywhere)
	FString Katakana;

	UPROPERTY(EditAnywhere)
	FString Romaji;

	UPROPERTY(EditAnywhere)
	FString Row;
};
