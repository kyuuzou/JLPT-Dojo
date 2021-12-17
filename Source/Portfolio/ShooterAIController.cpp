// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay() {
	Super::BeginPlay();

	if (this->AIBehavior != nullptr) {
		this->RunBehaviorTree(this->AIBehavior);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
		
		UBlackboardComponent* BlackboardComponent = this->GetBlackboardComponent();
		
		BlackboardComponent->SetValueAsVector(
			TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()
		);

		BlackboardComponent->SetValueAsVector(
			TEXT("StartLocation"), this->GetPawn()->GetActorLocation()
		);
	}
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(this->GetPawn());
	
	if (ControlledCharacter == nullptr) {
		return true;
	}

	return ControlledCharacter->IsDead();
}

void AShooterAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	/*
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	UBlackboardComponent* BlackboardComponent = this->GetBlackboardComponent();

	if (this->LineOfSightTo(PlayerPawn)) {
		BlackboardComponent->SetValueAsVector(
			TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()
		);

		BlackboardComponent->SetValueAsVector(
			TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation()
		);
	} else {
		BlackboardComponent->ClearValue("PlayerLocation");
	}
	*/
}