// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "GameFramework/Actor.h"
#include "Blackboard.generated.h"

UCLASS(Abstract)
class JLPTDOJO_API ABlackboard : public AActor {
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void Reset() override;
	virtual void SetCaption(FText Caption) const;

protected:
	void SetMaterial(class UMaterialInterface* Material) const;

private:
	FVector DefaultLocation;
	class UMaterialInterface* DefaultMaterial;
	FRotator DefaultRotation;
	class UStaticMeshComponent* MeshComponent;
	class UText3DComponent* TextComponent;
};
