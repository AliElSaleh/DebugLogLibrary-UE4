// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSuccess, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogFatal, Fatal, All)
DECLARE_LOG_CATEGORY_EXTERN(LogCrash, Fatal, All)
DECLARE_LOG_CATEGORY_EXTERN(LogError, Error, All)
DECLARE_LOG_CATEGORY_EXTERN(LogWarning, Warning, All)
DECLARE_LOG_CATEGORY_EXTERN(LogNumber, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogUnit, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogColor, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogBool, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogVector, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogRotator, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogQuaternion, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogTransform, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogMatrix, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogMessage, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogObjectValidity, Display, All)

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
