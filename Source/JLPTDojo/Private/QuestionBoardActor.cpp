// Copyright (c) 2021 Nelson Rodrigues

#include "QuestionBoardActor.h"

void AQuestionBoardActor::SetCorrect(bool Correct) const {
	this->SetMaterial(Correct ? this->RightMaterial : this->WrongMaterial);
}

