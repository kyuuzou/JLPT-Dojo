// Fill out your copyright notice in the Description page of Project Settings.

#include "AnswerBoardComponent.h"
#include "Engine/DataTable.h"
#include "FVocabularyTableRow.h"
#include "Text3DComponent.h"

UAnswerBoardComponent::UAnswerBoardComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UAnswerBoardComponent::BeginPlay() {
	Super::BeginPlay();

	this->TextComponent = this->GetOwner()->FindComponentByClass<UText3DComponent>();
	this->SetRandomAnswer();
}

void UAnswerBoardComponent::SetRandomAnswer() {
	TArray<FName> rowNames = this->DataTable->GetRowNames();
	int index = FMath::FRandRange(0.0f, rowNames.Num());

	FVocabularyTableRow* row = this->DataTable->FindRow<FVocabularyTableRow>(rowNames[index], TEXT("Reading"));
	this->TextComponent->SetText(FText::FromString(row->Reading));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *row->Reading);
 }

void UAnswerBoardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

