// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TextToSpeech.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JLPTDOJO_API UTextToSpeech : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTextToSpeech();

	UFUNCTION(BlueprintCallable)
	void PlaySound();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
