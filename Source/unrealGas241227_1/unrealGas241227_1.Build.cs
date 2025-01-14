// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class unrealGas241227_1 : ModuleRules
{
	public unrealGas241227_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		
		PrivateDependencyModuleNames.AddRange(
			new string[] { "GameplayAbilities", "GameplayTags",
			"GameplayTasks"});


	}
}
