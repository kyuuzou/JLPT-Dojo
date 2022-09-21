// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestionBoardActor.generated.h"

UCLASS()
class JLPTDOJO_API AQuestionBoardActor : public AActor
{
	GENERATED_BODY()
	
public:
	AQuestionBoardActor();

	virtual void PostInitializeComponents() override;
	void SetCaption(FText Caption);
	void SetCorrect(bool Correct);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	class UDataTable* DataTable;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* RightMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	class UMaterialInterface* WrongMaterial;

	class UStaticMeshComponent* MeshComponent;
	class UText3DComponent* TextComponent;
};
