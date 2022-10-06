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

	UPROPERTY(EditAnywhere, Category = "Flow")
	float AllowSkipCooldownInSeconds = 1.0f;

	bool AllowSkip;
	TArray<class AAnswerBoard*> AnswerBoards;
	int CurrentDifficultyLevel;
	TMap<int, class DataTableWrapper*> DataPerJLPTLevel;
	class ADojoGameState* DojoGameState;
	bool HandlingAnswer;
	class AMeaningBoard* MeaningBoard;
	AActor* NextButton;
	class AQuestionBoardActor* QuestionBoard;
	class AAnswerBoard* RightAnswerBoard;

	class DataTableWrapper* GetCurrentData();
	void InitialiseActors();
	void InitialiseData();
	void InitialiseState();

	UFUNCTION()
	void OnAnswerBoardBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnLevelButtonBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnNextButtonHit(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void SetRandomQuestion();
};
