// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyTls : ModuleRules
{
	public TinyTls(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"OpenSSL"
			}
		);
	}
}
