// Fill out your copyright notice in the Description page of Project Settings.


#include "AnswerBoardActor.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "ProjectileActor.h"
#include "Text3DComponent.h"

// Called when the game starts or when spawned
void AAnswerBoardActor::BeginPlay() {
	Super::BeginPlay();

}

int AAnswerBoardActor::GetCurrentIndex() {
	return this->currentIndex;
}

void AAnswerBoardActor::SetCorrect(bool correct) {
	this->MeshComponent->SetMaterial(0, correct ? this->RightMaterial : this->WrongMaterial);
}

void AAnswerBoardActor::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();
	this->IsRightAnswer = false;

	this->DefaultLocation = this->GetActorLocation();
	this->DefaultRotation = this->GetActorRotation();
}

void AAnswerBoardActor::ResetGeometry() {
	this->SetActorLocation(this->DefaultLocation);
	this->SetActorRotation(this->DefaultRotation);

	this->MeshComponent->SetMaterial(0, this->DefaultMaterial);
	this->MeshComponent->SetSimulatePhysics(false);
	this->MeshComponent->SetSimulatePhysics(true);
}

void AAnswerBoardActor::SetRightAnswer(FString Answer, int Index) {
	this->IsRightAnswer = true;

	this->TextComponent->SetText(FText::FromString(Answer));
	this->currentIndex = Index;
}

void AAnswerBoardActor::SetRandomWrongAnswer(FString Answer, int Index) {
	this->IsRightAnswer = false;

	this->TextComponent->SetText(FText::FromString(Answer));
	this->currentIndex = Index;
}