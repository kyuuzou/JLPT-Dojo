// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnswerBoardActor.generated.h"

UCLASS()
class JLPTDOJO_API AAnswerBoardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	int GetCurrentIndex();
	virtual void PostInitializeComponents() override;
	void ResetGeometry();
	void SetCorrect(bool correct);
	void SetRandomWrongAnswer(FString Answer, int Index);
	void SetRightAnswer(FString Answer, int Index);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Materials")
	class UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* RightMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* WrongMaterial;

	int currentIndex;
	FVector DefaultLocation;
	FRotator DefaultRotation;
	bool IsRightAnswer;
	class UStaticMeshComponent* MeshComponent;
	class UText3DComponent* TextComponent;
};
