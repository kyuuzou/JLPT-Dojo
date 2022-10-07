// Copyright (c) 2021 Nelson Rodrigues

#include "JLPTLevelButton.h"

void AJLPTLevelButton::BeginPlay(){
	Super::BeginPlay();

	FString Caption = FString::Printf(TEXT(" N%d "), this->JLPTLevel);
	this->SetCaption(FText::FromString(Caption));
}

int AJLPTLevelButton::GetJLPTLevel() const {
	return this->JLPTLevel;
}
