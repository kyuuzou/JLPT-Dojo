// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NextButton.generated.h"

UCLASS()
class JLPTDOJO_API ANextButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANextButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
