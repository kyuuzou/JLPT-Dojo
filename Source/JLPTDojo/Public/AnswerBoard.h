// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "Blackboard.h"

#include "AnswerBoard.generated.h"

UCLASS()
class JLPTDOJO_API AAnswerBoard : public ABlackboard {
	GENERATED_BODY()

public:
	int GetCurrentIndex() const;
	virtual void PostInitializeComponents() override;
	void SetCorrect(bool Correct) const;
	void SetAnswer(FString Answer, int Index, bool IsRightAnswer_);

private:
	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* RightMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* WrongMaterial;

	int CurrentIndex;
	bool IsRightAnswer;
};
