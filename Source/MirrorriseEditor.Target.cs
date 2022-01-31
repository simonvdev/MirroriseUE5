// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MirrorriseEditorTarget : TargetRules
{
	public MirrorriseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Mirrorrise");
	}
}