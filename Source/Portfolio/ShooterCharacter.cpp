// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Gun.h"
#include "PortfolioGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	this->Health = this->MaxHealth;
	this->SetFirstPerson(true);
	this->Gun = this->GetWorld()->SpawnActor<AGun>(this->GunClass);
	
	USkeletalMeshComponent* mesh = this->GetMesh();
	mesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	
	this->Gun->AttachToComponent(
		mesh,
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("WeaponSocket")
	);

	this->Gun->SetOwner(this);
}

float AShooterCharacter::GetHealthPercent() const {
	return this->Health / this->MaxHealth;
}

bool AShooterCharacter::IsDead() const {
	return this->Health <= 0.0f;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	this->AddMovementInput(this->GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	this->AddMovementInput(this->GetActorRightVector() * AxisValue);
}

void AShooterCharacter::SetFirstPerson(bool FirstPerson) {
	this->firstPerson = FirstPerson;
	this->FirstPersonCamera->SetActive(FirstPerson);
	this->ThirdPersonCamera->SetActive(!FirstPerson);
	
	if (FirstPerson) {
		this->GetMesh()->HideBoneByName("neck_01", EPhysBodyOp::PBO_MAX);
	} else {
		this->GetMesh()->UnHideBoneByName("neck_01");
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Move_Y"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Move_X"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(
		TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot
	);

	PlayerInputComponent->BindAction(
		TEXT("Toggle First-Person"),
		EInputEvent::IE_Pressed,
		this,
		&AShooterCharacter::ToggleFirstPerson
	);
}

void AShooterCharacter::Shoot()
{
	this->Gun->PullTrigger();
}

float AShooterCharacter::TakeDamage(
	float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser
) {
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(damageToApply, this->Health);

	this->Health -= damageToApply;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), this->Health);

	if (this->IsDead()) {
		APortfolioGameModeBase* gameMode = this->GetWorld()->GetAuthGameMode<APortfolioGameModeBase>();

		if (gameMode != nullptr) {
			gameMode->PawnKilled(this);
		}

		this->DetachFromControllerPendingDestroy();
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
 
	return damageToApply;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterCharacter::ToggleFirstPerson()
{
	this->SetFirstPerson(!this->firstPerson);
}

