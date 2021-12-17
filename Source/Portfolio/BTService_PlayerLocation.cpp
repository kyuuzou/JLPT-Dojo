// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocation::UBTService_PlayerLocation() {
	this->NodeName = "Update Player Location";
}

void UBTService_PlayerLocation::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds
) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);

	if (PlayerPawn == nullptr) {
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		this->GetSelectedBlackboardKey(),
		PlayerPawn->GetActorLocation()
	);
}
