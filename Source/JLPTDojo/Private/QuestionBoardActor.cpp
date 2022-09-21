// Copyright (c) 2021 Nelson Rodrigues

#include "QuestionBoardActor.h"

#include "Text3DComponent.h"

void AQuestionBoardActor::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
}

void AQuestionBoardActor::SetCaption(FText Caption) {
	this->TextComponent->SetText(Caption);

	this->MeshComponent->SetMaterial(0, this->DefaultMaterial);
}

void AQuestionBoardActor::SetCorrect(bool Correct) {
	this->MeshComponent->SetMaterial(0, Correct ? this->RightMaterial : this->WrongMaterial);
}

