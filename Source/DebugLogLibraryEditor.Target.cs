// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DebugLogLibraryEditorTarget : TargetRules
{
	public DebugLogLibraryEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new[] { "DebugLogLibrary" } );
		DefaultBuildSettings = BuildSettingsVersion.Latest;
	}
}
