// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeaningBoard.generated.h"

UCLASS()
class JLPTDOJO_API AMeaningBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeaningBoard();

	virtual void PostInitializeComponents() override;
	void SetCaption(FString Caption);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	class UStaticMeshComponent* MeshComponent;
	class UText3DComponent* TextComponent;
};
