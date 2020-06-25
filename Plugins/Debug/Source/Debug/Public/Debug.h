// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSuccess, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogFatal, Fatal, All)
DECLARE_LOG_CATEGORY_EXTERN(LogCrash, Fatal, All)
DECLARE_LOG_CATEGORY_EXTERN(LogError, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogWarning, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogNumber, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogUnit, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogFColor, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogCapsule, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogBox, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogSphere, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogBool, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogVector, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogRotator, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogQuaternion, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogTransform, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogMatrix, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogDateTime, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogEnum, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogArray, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogMessage, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogLineBreak, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogObjectValidity, Log, All)

class FDebugModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
	bool SupportsDynamicReloading() override;

	bool HandleSettingsSaved();

	void RegisterSettings();
	void UnRegisterSettings();
};
