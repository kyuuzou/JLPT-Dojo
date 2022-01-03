// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "BlackboardActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->SetRootComponent(this->Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	this->Mesh = this->FindComponentByClass<USkeletalMeshComponent>();

	if (this->Mesh == nullptr) {
		this->Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
		this->Mesh->SetupAttachment(this->Root);
	}

	this->Mesh->SetSimulatePhysics(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled());
}

AController* AGun::GetOwnerController() const {
	APawn* owner = Cast<APawn>(this->GetOwner());

	if (owner == nullptr) {
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
			// TODO: assign an owner to the gun when the player picks it up
			return this->GetWorld()->GetFirstPlayerController();
		}

		return nullptr;
	}

	return owner->GetController();
}

bool AGun::GunTrace(OUT FHitResult& HitResult, OUT FVector& ShotDirection) {
	AController* OwnerController = this->GetOwnerController();

	if (OwnerController == nullptr) {
		return false;
	}

	FName muzzleName = FName(TEXT("MuzzleLocation"));
	USceneComponent* muzzleLocationComponent = Cast<USceneComponent>(
		this->GetDefaultSubobjectByName(muzzleName)
	);

	FVector location = muzzleLocationComponent->GetComponentLocation();
	FRotator rotation = muzzleLocationComponent->GetComponentRotation();

	FVector shotDirection = -rotation.Vector();
	FVector end = location + rotation.Vector() * this->MaxRange;

	const FName TraceTag("DebugTraceTag");
	this->GetWorld()->DebugDrawTraceTag = TraceTag;
	
	FCollisionQueryParams queryParameters;
	//queryParameters.TraceTag = TraceTag;
	queryParameters.AddIgnoredActor(this);
	queryParameters.AddIgnoredActor(this->GetOwner());

	return this->GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		location,
		end,
		ECollisionChannel::ECC_GameTraceChannel1,
		queryParameters
	);
}

void AGun::PullTrigger(bool makeNoise)
{
	//UE_LOG(LogTemp, Warning, TEXT("AGun::PullTrigger"));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AGun::PullTrigger"));

	if (makeNoise) {
		UGameplayStatics::SpawnEmitterAttached(this->MuzzleFlash, this->Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(this->MuzzleSound, this->Mesh, TEXT("MuzzleFlashSound"));
	}

	FHitResult HitResult;
	FVector ShotDirection;

	if (this->GunTrace(HitResult, ShotDirection)) {
		DrawDebugPoint(this->GetWorld(), HitResult.Location, 20.0f, FColor::Red, true);

		// pulling it a bit back, so it doesn't spawn inside the target
		//FVector ShotLocation = HitResult.Location - rotation.Vector() * 10.0f;
		FVector ShotLocation = HitResult.Location;

		if (makeNoise) {
			UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), this->ImpactSound, ShotLocation);

			UGameplayStatics::SpawnEmitterAtLocation(
				this->GetWorld(),
				this->ImpactEffect,
				ShotLocation,
				ShotDirection.Rotation()
			);
		}

		AActor* victim = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Gun::PullTrigger: before victim"));

		if (victim != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Gun::PullTrigger: %s"), *victim->GetName());

			FPointDamageEvent damageEvent(this->Damage, HitResult, ShotDirection, nullptr);

			AController* OwnerController = this->GetOwnerController();
			victim->TakeDamage(this->Damage, damageEvent, OwnerController, this);
		}
	}
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector ShotDirection;

	if (this->GunTrace(HitResult, ShotDirection)) {
		AActor* target = HitResult.GetActor();

		//UE_LOG(LogTemp, Warning, TEXT("Bsdfjkfsdnkjsdfnjkfdjnk"));

		//TODO: have a better way to determine if the actor needs to know that we're pointing at him
		ABlackboardActor* blackboard = Cast<ABlackboardActor>(target);

		if (blackboard != nullptr) {
			blackboard->BePointedAt(HitResult);
		}
	}
}
