// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mirrorrise : ModuleRules
{
	public Mirrorrise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{"Core", "CoreUObject", "Engine", "InputCore", "Niagara", "EnhancedInput"});

		PrivateDependencyModuleNames.AddRange(new[] {"GameplayAbilities", "GameplayTags", "GameplayTasks"});
	}
}