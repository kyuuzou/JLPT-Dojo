// Copyright (c) 2021 Nelson Rodrigues

#include "AnswerBoard.h"

int AAnswerBoard::GetCurrentIndex() const {
	return this->CurrentIndex;
}

void AAnswerBoard::SetCorrect(bool Correct) const {
	this->SetMaterial(Correct ? this->RightMaterial : this->WrongMaterial);
}

void AAnswerBoard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->IsRightAnswer = false;
}

void AAnswerBoard::SetAnswer(FString Answer, int Index, bool IsRightAnswer_) {
	this->IsRightAnswer = IsRightAnswer_;
	this->CurrentIndex = Index;

	this->SetCaption(FText::FromString(Answer));
}
