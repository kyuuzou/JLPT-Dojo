// Copyright (c) 2021 Nelson Rodrigues

#include "Blackboard.h"

#include "Text3DComponent.h"

void ABlackboard::PostInitializeComponents() {
	Super::PostInitializeComponents();

	this->MeshComponent = this->FindComponentByClass<UStaticMeshComponent>();
	this->TextComponent = this->FindComponentByClass<UText3DComponent>();

	this->DefaultLocation = this->GetActorLocation();
	this->DefaultRotation = this->GetActorRotation();
	this->DefaultMaterial = this->MeshComponent->GetMaterial(0);
}

void ABlackboard::Reset() {
	this->SetActorLocation(this->DefaultLocation);
	this->SetActorRotation(this->DefaultRotation);

	this->MeshComponent->SetMaterial(0, this->DefaultMaterial);

	if (this->MeshComponent->IsSimulatingPhysics()) {
		this->MeshComponent->SetSimulatePhysics(false);
		this->MeshComponent->SetSimulatePhysics(true);
	}
}

void ABlackboard::SetCaption(FText Caption) const{
	this->TextComponent->SetText(Caption);
}

void ABlackboard::SetMaterial(UMaterialInterface* Material) const {
	this->MeshComponent->SetMaterial(0, Material);
}