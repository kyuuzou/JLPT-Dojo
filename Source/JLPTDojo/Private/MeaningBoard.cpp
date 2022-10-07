// Copyright (c) 2021 Nelson Rodrigues

#include "MeaningBoard.h"

#include "Internationalization/Regex.h"
#include "Text3DComponent.h"

void AMeaningBoard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
	this->DefaultCaption = this->TextComponent->Text;
}

void AMeaningBoard::Reset() {
	Super::Reset();

	this->TextComponent->SetText(this->DefaultCaption);
}

void AMeaningBoard::SetCaption(FString Caption) {
	const FRegexPattern Pattern(TEXT("(?<=1. )[^\\[{\\|]*"));
	FRegexMatcher Matcher(Pattern, Caption);

	if (Matcher.FindNext()) {
		Caption = Matcher.GetCaptureGroup(0);

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
	int MinimumSize = 50;

	if (Caption.Len() < MinimumSize) {
		int Difference = MinimumSize - Caption.Len();

		for (int i = 0; i < Difference; i += 2) {
			Caption.InsertAt(0, ' ');
			Caption.InsertAt(Caption.Len(), ' ');
		}
	}

	this->TextComponent->SetText(FText::FromString(Caption));
}
