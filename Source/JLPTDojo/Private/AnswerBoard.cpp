// Copyright (c) 2021 Nelson Rodrigues

#include "AnswerBoard.h"
#include "Text3DComponent.h"

int AAnswerBoard::GetCurrentIndex() {
	return this->currentIndex;
}

void AAnswerBoard::SetCorrect(bool correct) {
	this->MeshComponent->SetMaterial(0, correct ? this->RightMaterial : this->WrongMaterial);
}

void AAnswerBoard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
	this->IsRightAnswer = false;

	this->DefaultLocation = this->GetActorLocation();
	this->DefaultRotation = this->GetActorRotation();
}

void AAnswerBoard::ResetGeometry() {
	this->SetActorLocation(this->DefaultLocation);
	this->SetActorRotation(this->DefaultRotation);

	this->MeshComponent->SetMaterial(0, this->DefaultMaterial);
	this->MeshComponent->SetSimulatePhysics(false);
	this->MeshComponent->SetSimulatePhysics(true);
}

void AAnswerBoard::SetRightAnswer(FString Answer, int Index) {
	this->IsRightAnswer = true;

	this->TextComponent->SetText(FText::FromString(Answer));
	this->currentIndex = Index;
}

void AAnswerBoard::SetRandomWrongAnswer(FString Answer, int Index) {
	this->IsRightAnswer = false;

	this->TextComponent->SetText(FText::FromString(Answer));
	this->currentIndex = Index;
}
