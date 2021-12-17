// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class PORTFOLIO_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Shoot();

	virtual float TakeDamage(
		float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* ThirdPersonCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void SetFirstPerson(bool FirstPerson);
	void ToggleFirstPerson();

	bool firstPerson;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float Health;
};
