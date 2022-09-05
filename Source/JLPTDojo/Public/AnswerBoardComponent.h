// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnswerBoardComponent.generated.h"


UCLASS(BluePrintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JLPTDOJO_API UAnswerBoardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
	class UDataTable* DataTable;
	
	UAnswerBoardComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:	
	class UText3DComponent* TextComponent;

	void SetRandomAnswer();
		
};
