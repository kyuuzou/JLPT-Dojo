// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackboardActor.h"

#include "Components/BoxComponent.h"
#include "DojoGameState.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Text3DComponent.h"

ABlackboardActor::ABlackboardActor() {
	PrimaryActorTick.bCanEverTick = true;
	this->Enabled = true;
}

void ABlackboardActor::BeginPlay() {
	Super::BeginPlay();

	UText3DComponent* textComponent = this->FindComponentByClass<UText3DComponent>();
	UMaterialInterface* MaterialInterface = textComponent->FrontMaterial;
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(MaterialInterface, nullptr);
	textComponent->SetFrontMaterial(Material);

	this->Disable();
}

void ABlackboardActor::BePointedAt(FHitResult HitResult) {

}

void ABlackboardActor::Clear() {

}

void ABlackboardActor::Disable() {
	this->Enabled = false;
}

void ABlackboardActor::Enable() {
	this->Enabled = true;
}

void ABlackboardActor::OnCorrectAnswer() {
	this->SetColor(FLinearColor::Green);
}

void ABlackboardActor::OnWrongAnswer() {
	this->SetColor(FLinearColor::Red);
}

void ABlackboardActor::SetCaption(FString Caption) {
	this->Clear();

	int LastLineBreak = 0;
	int CharactersPerLine = 20;

	TArray<int> LineBreaks;
	
	for (int i = 0; i < Caption.Len(); i ++) {
		if (Caption[i] == '\n') {
			LastLineBreak = 0;
		}

		// 7 = length of [...] + 1 for the zero-based array + 1
		if ((LastLineBreak++ > CharactersPerLine) || (Caption[i] == '[' && i >= CharactersPerLine - 7)) {
			LastLineBreak = 0;
			LineBreaks.Add(i);
		}
	}

	int MinimumCharactersBetweenLineBreaks = 2;
	LastLineBreak = INT_MAX;

	for (int i = LineBreaks.Num() - 1; i >= 0; i--) {
		if (LastLineBreak - LineBreaks[i] <= MinimumCharactersBetweenLineBreaks) {
			continue;
		}

		LastLineBreak = LineBreaks[i];
		Caption.InsertAt(LineBreaks[i], "\n");
	}

	// set max width to width of collision box
	UStaticMeshComponent* board = this->FindComponentByClass<UStaticMeshComponent>();
	FVector localBoxExtent = board->CalcLocalBounds().BoxExtent;

	float margin = 10.0f;
	localBoxExtent.X -= margin;
	localBoxExtent.Z -= margin;

	UText3DComponent* textComponent = this->FindComponentByClass<UText3DComponent>();
	float textScaleX = textComponent->GetRelativeScale3D().X;

	float maxWidth = localBoxExtent.X * 2.0f * (1.0f / textScaleX);
	textComponent->SetMaxWidth(maxWidth);
	
	float lineHeight = localBoxExtent.Z / 3.0f;

	// move to center
	FVector textLocation = textComponent->GetRelativeLocation();
	textLocation.Z = localBoxExtent.Z + lineHeight;
	textComponent->SetRelativeLocation(textLocation);

	textComponent->SetText(FText::FromString(Caption));
}

void ABlackboardActor::SetColor(FLinearColor Color) {
	UText3DComponent* TextComponent = this->FindComponentByClass<UText3DComponent>();

	if (TextComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Could not find Text3DComponent."));
		return;
	}

	UMaterialInterface* Material = TextComponent->FrontMaterial;
	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

	if (DynamicMaterial == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Could not cast to DynamicMaterial."));
		return;
	}

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}

void ABlackboardActor::SetTextSize(FVector Size) {
	UText3DComponent* TextComponent = this->FindComponentByClass<UText3DComponent>();
	USceneComponent* TextRoot = static_cast<USceneComponent*>(TextComponent->GetDefaultSubobjectByName(TEXT("TextRoot")));
	TextRoot->SetRelativeScale3D(Size);
}

