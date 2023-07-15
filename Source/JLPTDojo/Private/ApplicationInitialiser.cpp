// Copyright (c) 2023 Nelson Rodrigues

#include "ApplicationInitialiser.h"

#include "Framework/Application/SlateApplication.h"

void UApplicationInitialiser::Run() {
#if WITH_EDITOR
	FSlateApplication::Get().SetApplicationScale(1.25f);
#endif
}
