// Copyright (c) 2021 Nelson Rodrigues

using UnrealBuildTool;
using System.Collections.Generic;

public class JLPTDojoTarget : TargetRules
{
	public JLPTDojoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "JLPTDojo" } );
	}
}
