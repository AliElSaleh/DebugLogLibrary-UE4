// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Debug.h"

#include "DebugLogLibrarySettings.h"
#include "ISettingsModule.h"
#include "Log.h"

#define LOCTEXT_NAMESPACE "FDebugModule"

void FDebugModule::StartupModule()
{
	RegisterSettings();
}

void FDebugModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnRegisterSettings();
	}
}

bool FDebugModule::SupportsDynamicReloading()
{
	return true;
}

bool FDebugModule::HandleSettingsSaved()
{
	UDebugLogLibrarySettings* Settings = GetMutableDefault<UDebugLogLibrarySettings>();
	//bool ResaveSettings = false;
	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	//if (ResaveSettings)
	//{
		Settings->SaveConfig();
	//}

	return true;
}

void FDebugModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Debug Log Library",
			LOCTEXT("RuntimeSettingsName", "Debug Log Library"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Debug Log Library plugin"),
			GetMutableDefault<UDebugLogLibrarySettings>());
	}
}

void FDebugModule::UnRegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Debug Log Library");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugModule, Debug)

DEFINE_LOG_CATEGORY(LogSuccess)
DEFINE_LOG_CATEGORY(LogError)
DEFINE_LOG_CATEGORY(LogFatal)
DEFINE_LOG_CATEGORY(LogCrash)
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
