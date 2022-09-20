// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DojoGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JLPTDOJO_API ADojoGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	class UDataTable* DataTable;

	TArray<class AAnswerBoardActor*> AnswerBoards;
	class ADojoGameState* DojoGameState;
	TArray<FName> FilteredRowNames;
	bool HandlingAnswer;
	class AMeaningBoard* MeaningBoard;
	AActor* NextButton;
	class AQuestionBoardActor* QuestionBoard;
	class AAnswerBoardActor* RightAnswerBoard;

	struct FVocabularyTableRow* GetTableRow(int Index);
	struct FVocabularyTableRow* GetTableRow(FName RowName);
	void InitialiseActors();
	void InitialiseData();
	void InitialiseState();

	UFUNCTION()
	void OnAnswerBoardBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnNextButtonHit(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void SetRandomQuestion();
};
