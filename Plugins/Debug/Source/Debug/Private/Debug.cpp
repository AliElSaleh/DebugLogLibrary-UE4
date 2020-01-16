// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Debug.h"

#include "DebugLogLibrarySettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
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
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("Debug Log Library",
			LOCTEXT("RuntimeWDCategoryName", "Debug Log Library"),
			LOCTEXT("RuntimeWDCategoryDescription", "Configure the Debug Log Library plugin"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Debug Log Library", "General",
			LOCTEXT("RuntimeGeneralSettingsName", "General"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Configure the Debug Log Library plugin"),
			GetMutableDefault<UDebugLogLibrarySettings>()
			);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FDebugModule::HandleSettingsSaved);
		}
	}
}

void FDebugModule::UnRegisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Debug Log Library", "General");
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
