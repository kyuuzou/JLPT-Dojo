// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FQuestion.generated.h"

USTRUCT(BlueprintType)
struct FQuestion: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    FString Instructions;

    UPROPERTY(EditAnywhere)
    FString Sentence;

    UPROPERTY(EditAnywhere)
    FString RightAnswer;

    UPROPERTY(EditAnywhere)
    TArray<FString> WrongAnswers;
};
