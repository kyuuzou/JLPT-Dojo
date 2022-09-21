// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreBoard.generated.h"

UCLASS()
class JLPTDOJO_API AScoreBoard : public AActor {
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
	FString TextTemplate;

	class UText3DComponent* TextComponent;

	UFUNCTION()
	void OnScoreChanged(int CurrentStreak, int MaximumStreak, int RightAnswers, int TotalQuestions);
};
