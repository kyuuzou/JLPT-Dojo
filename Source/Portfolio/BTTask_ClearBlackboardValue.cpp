// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue() {
	this->NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FName Key = this->GetSelectedBlackboardKey();
	OwnerComp.GetBlackboardComponent()->ClearValue(Key);

	return EBTNodeResult::Succeeded;
}