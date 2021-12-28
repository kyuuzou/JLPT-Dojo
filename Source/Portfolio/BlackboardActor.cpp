// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackboardActor.h"

#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "FQuestion.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Text3DComponent.h"

ABlackboardActor::ABlackboardActor() {
	PrimaryActorTick.bCanEverTick = true;
	this->handlingAnswer = false;
}

UText3DComponent* ABlackboardActor::AddLine(
	UText3DComponent* templateComponent, FString line, OUT FVector& location, float lineHeight
) {
	UActorComponent* actorComponent = this->CreateComponentFromTemplate(templateComponent);
	UText3DComponent* textComponent = Cast<UText3DComponent>(actorComponent);
	textComponent->RegisterComponent();
	this->AddInstanceComponent(textComponent);
	this->TextComponents.Add(textComponent);

	textComponent->SetText(FText::FromString(line));
	textComponent->SetWorldLocation(location);

	UMaterialInterface* MaterialInterface = textComponent->FrontMaterial;
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(MaterialInterface, nullptr);
	textComponent->SetFrontMaterial(Material);

	location.Z -= lineHeight;

	return textComponent;
}

void ABlackboardActor::BeginPlay() {
	Super::BeginPlay();

	FGenericPlatformMath::SRandInit(time(0));
	this->SetRandomQuestion();
}

UText3DComponent* ABlackboardActor::BePointedAt(FHitResult HitResult) {
	float MinDistance = FLT_MAX;
	UText3DComponent* ClosestTextComponent = nullptr;

	UBoxComponent* box = this->FindComponentByClass<UBoxComponent>();
	FVector boxExtent = box->Bounds.BoxExtent;
	float lineHeight = boxExtent.Z / 3.0f;
	lineHeight += 10.0f; // TODO: figure out why the aim is off by about 5.0f

	for (UText3DComponent* TextComponent : this->AnswerTextComponents) {
		FVector Location = TextComponent->GetComponentLocation();
		Location.Z -= (lineHeight * 0.5f);

		float Distance = FVector::Dist(HitResult.ImpactPoint, Location);
		float DistanceFromRow = abs(Location.Z - HitResult.ImpactPoint.Z);

		if (Distance < MinDistance && DistanceFromRow < lineHeight) {
			if (!handlingAnswer) {
				ABlackboardActor::SetColor(ClosestTextComponent, FLinearColor::White);
				ABlackboardActor::SetColor(TextComponent, FLinearColor::Yellow);
			}

			ClosestTextComponent = TextComponent;
			MinDistance = Distance;
		}
		else {
			if (!handlingAnswer) {
				ABlackboardActor::SetColor(TextComponent, FLinearColor::White);
			}
		}
	}

	return ClosestTextComponent;
}

void ABlackboardActor::Clear() {
	for (UText3DComponent* textComponent : this->TextComponents) {
		textComponent->UnregisterComponent();
		this->RemoveInstanceComponent(textComponent);
	}

	this->TextComponents.Empty();
	this->AnswerTextComponents.Empty();
	this->RightAnswerComponent = nullptr;
}

void ABlackboardActor::SetColor(UText3DComponent* TextComponent, FLinearColor Color) {
	if (TextComponent == nullptr) {
		return;
	}

	UMaterialInterface* Material = TextComponent->FrontMaterial;
	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}

void ABlackboardActor::SetRandomQuestion() {
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	int index = FMath::FRandRange(0, RowNames.Num());

	this->SetQuestion(index);
}

void ABlackboardActor::SetQuestion(int index) {
	this->Clear();

	// set max width to width of collision box
	UBoxComponent* box = this->FindComponentByClass<UBoxComponent>();
	FVector boxExtent = box->Bounds.BoxExtent;

	UText3DComponent* templateComponent = this->FindComponentByClass<UText3DComponent>();
	float textScaleX = templateComponent->GetRelativeScale3D().X;

	float maxWidth = boxExtent.X * 2.0f * (1.0f / textScaleX);
	templateComponent->SetMaxWidth(maxWidth);

	// move to corner
	FVector location = box->GetComponentLocation();
	location.X -= boxExtent.X;
	location.Y += 2.001f; // just enough so it doesn't Z-fight the chalkboard
	location.Z += boxExtent.Z;
	templateComponent->SetWorldLocation(location);

	// position strings
	TArray<FName> RowNames = this->DataTable->GetRowNames();
	FQuestion* question = this->DataTable->FindRow<FQuestion>(RowNames[index], TEXT("Blackboard"));

	float lineHeight = boxExtent.Z / 3.0f;

	this->AddLine(templateComponent, question->Instructions, location, lineHeight);
	this->AddLine(templateComponent, question->Sentence, location, lineHeight);

	TArray<FString> answers = question->WrongAnswers;
	answers.Add(question->RightAnswer);

	// shuffle answers
	for (int i = answers.Num() - 1; i > 0; i--) {
		int j = FMath::FloorToInt(FMath::SRand() * (i + 1)) % answers.Num();
		FString temp = answers[i];
		answers[i] = answers[j];
		answers[j] = temp;
	}

	for (int i = 0; i < answers.Num(); i++) {
		bool isRightAnswer = answers[i] == question->RightAnswer;

		FString answer = FString::Printf(TEXT("%d %s □"), i + 1, *answers[i]);
		UText3DComponent* textComponent = this->AddLine(templateComponent, answer, location, lineHeight);
		this->AnswerTextComponents.Add(textComponent);

		if (isRightAnswer) {
			this->RightAnswerComponent = textComponent;
		}
	}

	templateComponent->SetVisibility(false, true);
	this->handlingAnswer = false;
}

float ABlackboardActor::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) {
	if (this->handlingAnswer) {
		return;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
		this->handlingAnswer = true;

		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		FHitResult HitResult = PointDamageEvent->HitInfo;
		
		UText3DComponent* textComponent = this->BePointedAt(HitResult);

		if (textComponent != nullptr) {
			FString text = textComponent->Text.ToString();
			textComponent->SetText(FText::FromString(text.Replace(TEXT("□"), TEXT("▣"))));

			if (textComponent == this->RightAnswerComponent) {
				ABlackboardActor::SetColor(textComponent, FLinearColor::Green);
			} else {
				ABlackboardActor::SetColor(textComponent, FLinearColor::Red);
			}

			FTimerHandle timerHandle;
			this->GetWorldTimerManager().SetTimer(
				timerHandle,
				this,
				&ABlackboardActor::SetRandomQuestion,
				3.0f
			);
		} else {
			this->handlingAnswer = false;
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("ABlackboardActor::TakeDamage: Unexpected type of damage!"));
	}

	return 0.0f;
}

// Called every frame
void ABlackboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

