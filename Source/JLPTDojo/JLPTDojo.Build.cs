// Copyright (c) 2021 Nelson Rodrigues

using UnrealBuildTool;

public class JLPTDojo : ModuleRules
{
	public JLPTDojo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
