// Copyright (c) 2021 Nelson Rodrigues

#pragma once

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

/**
 * 
 */
class JLPTDOJO_API UGameplayStaticsExtended : UGameplayStatics {

public:
	template <typename ActorClass>
	static ActorClass* GetActorOfSubclass(UWorld* World) {
		TActorIterator<ActorClass> It(World);
		return *It;
	}

	template <typename ActorClass>
	static void GetAllActorsOfSubclass(UWorld* World, TArray<ActorClass*>& Actors) {
		Actors.Reset();

		for (TActorIterator<ActorClass> It(World); It; ++It) {
			Actors.Add(*It);
		}
	}
};
