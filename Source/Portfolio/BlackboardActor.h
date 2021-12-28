// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackboardActor.generated.h"

UCLASS()
class PORTFOLIO_API ABlackboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackboardActor();
	class UText3DComponent* BePointedAt(FHitResult HitResult);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

private:
	UPROPERTY(EditAnywhere)
	class UDataTable* DataTable;

	bool handlingAnswer;

	UPROPERTY()
	UText3DComponent* RightAnswerComponent;

	TArray<class UText3DComponent*> AnswerTextComponents;
	TArray<class UText3DComponent*> TextComponents;

	UFUNCTION()
	UText3DComponent* AddLine(
		class UText3DComponent* templateComponent, FString line, FVector& location, float lineHeight
	);

	void Clear();
	static void SetColor(class UText3DComponent* TextComponent, FLinearColor Color);
	void SetRandomQuestion();
	void SetQuestion(int index);
};
