// Copyright Ali El Saleh 2020

#include "DebugLogLibrarySettings.h"

UDebugLogLibrarySettings::UDebugLogLibrarySettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bEnableSpecifiers = false;
	
	LogCategorySpecifiers.Empty();
	LogCategorySpecifiers.Add(DL_Info, "[Info]: ");
	LogCategorySpecifiers.Add(DL_Warning, "[Warning]: ");
	LogCategorySpecifiers.Add(DL_Success, "[Success]: ");
	LogCategorySpecifiers.Add(DL_Error, "[Error]: ");
	LogCategorySpecifiers.Add(DL_Fatal, "[Fatal]: ");
}
