// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MirrorriseTarget : TargetRules
{
	public MirrorriseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Mirrorrise");
	}
}