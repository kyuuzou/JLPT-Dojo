// Fill out your copyright notice in the Description page of Project Settings.


#include "MeaningBoard.h"

#include "Internationalization/Regex.h"
#include "Text3DComponent.h"

AMeaningBoard::AMeaningBoard() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMeaningBoard::BeginPlay() {
	Super::BeginPlay();
}

void AMeaningBoard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
}

void AMeaningBoard::SetCaption(FString Caption) {
	const FRegexPattern myPattern(TEXT("(?<=1. )[^\\[{\\|]*"));
	FRegexMatcher myMatcher(myPattern, Caption);

	if (myMatcher.FindNext()) {
		Caption = myMatcher.GetCaptureGroup(0);
		
		// TODO: revisit the regex, so it also removes the second meaning
		int secondMeaningIndex = Caption.Find("2. ");

		if (secondMeaningIndex > 0) {
			Caption.RemoveAt(secondMeaningIndex, Caption.Len() - secondMeaningIndex);
		}

		Caption = Caption.TrimStartAndEnd();
	} else {
		Caption = FString("Error: could not parse meaning.");
	}

	// TODO: Implement proper scaling so the text is always the same size, and remove this
	int minimumSize = 40;

	if (Caption.Len() < minimumSize) {
		int difference = Caption.Len() - minimumSize;

		for (int i = 0; i < difference; i += 2) {
			Caption.InsertAt(0, ' ');
			Caption.InsertAt(Caption.Len(), ' ');
		}
	}

	this->TextComponent->SetText(FText::FromString(Caption));
}

void AMeaningBoard::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

