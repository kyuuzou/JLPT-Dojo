// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "Blackboard.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuestionBoardActor.generated.h"

UCLASS()
class JLPTDOJO_API AQuestionBoardActor : public ABlackboard {
	GENERATED_BODY()

public:
	void SetCorrect(bool Correct) const;

private:
	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* RightMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* WrongMaterial;
};
