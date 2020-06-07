// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DebugLogLibraryTarget : TargetRules
{
	public DebugLogLibraryTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new[] { "DebugLogLibrary" } );
	}
}
