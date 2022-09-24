// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DojoGameMode.generated.h"

UCLASS()
class JLPTDOJO_API ADojoGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	class UDataTable* DataTable;

	TArray<class AAnswerBoard*> AnswerBoards;
	class ADojoGameState* DojoGameState;
	TArray<FName> FilteredRowNames;
	bool HandlingAnswer;
	class AMeaningBoard* MeaningBoard;
	AActor* NextButton;
	class AQuestionBoardActor* QuestionBoard;
	class AAnswerBoard* RightAnswerBoard;

	struct FVocabularyTableRow* GetTableRow(int Index) const;
	struct FVocabularyTableRow* GetTableRow(FName RowName) const;
	void InitialiseActors();
	void InitialiseData();
	void InitialiseState();

	UFUNCTION()
	void OnAnswerBoardBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnNextButtonHit(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void SetRandomQuestion();
};
