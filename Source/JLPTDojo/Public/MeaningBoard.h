// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeaningBoard.generated.h"

UCLASS()
class JLPTDOJO_API AMeaningBoard : public AActor {
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void Reset() override;
	void SetCaption(FString Caption);

private:
	FText DefaultCaption;
	class UStaticMeshComponent* MeshComponent;
	class UText3DComponent* TextComponent;
};
