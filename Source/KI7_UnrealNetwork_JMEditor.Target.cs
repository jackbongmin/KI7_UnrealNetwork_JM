// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class KI7_UnrealNetwork_JMEditorTarget : TargetRules
{
	public KI7_UnrealNetwork_JMEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "KI7_UnrealNetwork_JM" } );
	}
}
