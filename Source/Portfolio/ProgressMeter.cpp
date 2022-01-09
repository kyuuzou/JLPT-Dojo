// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressMeter.h"

#include "DojoGameState.h"
#include "Text3DComponent.h"

TArray<FString> ArabicToKanji = {
	TEXT("零"),
	TEXT("一"),
	TEXT("二"),
	TEXT("三"),
	TEXT("四"),
	TEXT("五"),
	TEXT("六"),
	TEXT("七"),
	TEXT("八"),
	TEXT("九"),
	TEXT("十")
};

// Sets default values
AProgressMeter::AProgressMeter() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AProgressMeter::BeginPlay() {
	Super::BeginPlay();

	this->text3dComponent = this->FindComponentByClass<UText3DComponent>();

	ADojoGameState* gameState = Cast<ADojoGameState>(this->GetWorld()->GetGameState());
	gameState->OnProgressChanged.AddDynamic(this, &AProgressMeter::OnProgressChanged);
}

void AProgressMeter::OnProgressChanged(int progress) {
	FString progressText;

	if (progress > 10) {
		progressText = TEXT("Next Level");
	} else if (progress < -3) {
		progressText = TEXT("Game Over");
	} else if (progress == -3) {
		progressText = TEXT("-3");
	} else if (progress == -2) {
		progressText = TEXT("-2");
	} else if (progress == -1) {
		progressText = TEXT("-1");
	} else {
		progressText = ArabicToKanji[progress];
	}

	this->text3dComponent->SetText(FText::FromString(progressText));
}

// Called every frame
void AProgressMeter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

