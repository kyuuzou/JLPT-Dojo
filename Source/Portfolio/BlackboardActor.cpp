// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackboardActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Text3DComponent.h"

// Sets default values
ABlackboardActor::ABlackboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABlackboardActor::BeginPlay()
{
	Super::BeginPlay();

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
	for (FString question : this->questions) {
		TArray<FString> lines;
		question.ParseIntoArray(OUT lines, TEXT("|"));

		for (FString line : lines) {
			UActorComponent* actorComponent = this->CreateComponentFromTemplate(templateComponent);
			UText3DComponent* textComponent = Cast<UText3DComponent>(actorComponent);
			textComponent->RegisterComponent();
			this->AddInstanceComponent(textComponent);

			textComponent->SetText(FText::FromString(line));
			textComponent->SetWorldLocation(location);
			
			this->textComponents.Add(textComponent);

			location.Z -= boxExtent.Z / 3.0f;
		}
	}

	templateComponent->SetVisibility(false, true);
}

// Called every frame
void ABlackboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

