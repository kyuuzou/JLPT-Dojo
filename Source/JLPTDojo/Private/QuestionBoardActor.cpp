// Copyright (c) 2021 Nelson Rodrigues

#include "QuestionBoardActor.h"

#include "AnswerBoardActor.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "Text3DComponent.h"

AQuestionBoardActor::AQuestionBoardActor() {
	PrimaryActorTick.bCanEverTick = true;

}

void AQuestionBoardActor::BeginPlay() {
	Super::BeginPlay();

}

void AQuestionBoardActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

}

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

void AQuestionBoardActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

