// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen() {
	this->NodeName = "Update Player Location If Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds
) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);

	if (PlayerPawn == nullptr) {
		return;
	}

	if (OwnerComp.GetAIOwner() == nullptr) {
		return;
	}

	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(
			this->GetSelectedBlackboardKey(),
			PlayerPawn
		);
	} else {
		OwnerComp.GetBlackboardComponent()->ClearValue(this->GetSelectedBlackboardKey());
	}
}