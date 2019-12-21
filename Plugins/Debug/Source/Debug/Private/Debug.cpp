// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Debug.h"

#define LOCTEXT_NAMESPACE "FDebugModule"

void FDebugModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDebugModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugModule, Debug)

DEFINE_LOG_CATEGORY(LogSuccess)
DEFINE_LOG_CATEGORY(LogError)
DEFINE_LOG_CATEGORY(LogFatal)
DEFINE_LOG_CATEGORY(LogWarning)
DEFINE_LOG_CATEGORY(LogNumber)
DEFINE_LOG_CATEGORY(LogColor)
DEFINE_LOG_CATEGORY(LogBool)
DEFINE_LOG_CATEGORY(LogVector)
DEFINE_LOG_CATEGORY(LogRotator)
DEFINE_LOG_CATEGORY(LogTransform)
DEFINE_LOG_CATEGORY(LogQuaternion)
DEFINE_LOG_CATEGORY(LogMatrix)
DEFINE_LOG_CATEGORY(LogMessage)
DEFINE_LOG_CATEGORY(LogObjectValidity)
