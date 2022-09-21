// Copyright (c) 2021 Nelson Rodrigues

using UnrealBuildTool;
using System.Collections.Generic;

public class JLPTDojoEditorTarget : TargetRules
{
	public JLPTDojoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "JLPTDojo" } );
	}
}
