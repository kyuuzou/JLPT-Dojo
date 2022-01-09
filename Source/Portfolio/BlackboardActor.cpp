// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackboardActor.h"

#include "Components/BoxComponent.h"
#include "DojoGameState.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Text3DComponent.h"

ABlackboardActor::ABlackboardActor() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABlackboardActor::BeginPlay() {
	Super::BeginPlay();

	UText3DComponent* textComponent = this->FindComponentByClass<UText3DComponent>();
	UMaterialInterface* MaterialInterface = textComponent->FrontMaterial;
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(MaterialInterface, nullptr);
	textComponent->SetFrontMaterial(Material);
}

void ABlackboardActor::BePointedAt(FHitResult HitResult) {

}

void ABlackboardActor::Clear() {
	this->SetColor(FLinearColor::White);
}

void ABlackboardActor::OnCorrectAnswer() {
	this->SetColor(FLinearColor::Green);
}

void ABlackboardActor::OnWrongAnswer() {
	this->SetColor(FLinearColor::Red);
}

void ABlackboardActor::SetCaption(FString caption) {
	this->Clear();

	for (int i = 15; i < caption.Len(); i += 15) {
		caption.InsertAt(i, "\n");
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

	textComponent->SetText(FText::FromString(caption));
}

void ABlackboardActor::SetColor(FLinearColor Color) {
	UText3DComponent* TextComponent = this->FindComponentByClass<UText3DComponent>();

	if (TextComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Could not find Text3DComponent."));
		return;
	}

	UMaterialInterface* Material = TextComponent->FrontMaterial;
	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}

float ABlackboardActor::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) {
	this->OnBlackboardHit.Broadcast(this);

	return 0.0f;
}

// Called every frame
void ABlackboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

