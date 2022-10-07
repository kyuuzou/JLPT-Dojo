// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "Blackboard.h"
#include "CoreMinimal.h"

#include "JLPTLevelButton.generated.h"

UCLASS()
class JLPTDOJO_API AJLPTLevelButton : public ABlackboard
{
	GENERATED_BODY()

public:
	int GetJLPTLevel() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Difficulty")
	int JLPTLevel = 5;
};
