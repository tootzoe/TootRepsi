// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TootRepsiClientTarget : TargetRules
{
    public TootRepsiClientTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("TootRepsi");
	}
}
