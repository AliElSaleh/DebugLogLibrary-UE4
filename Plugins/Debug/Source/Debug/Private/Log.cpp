// Copyright Ali El Saleh 2020

#include "Log.h"
#include "Debug.h"

#include "Engine/Engine.h"

#include "DrawDebugHelpers.h"
#include "GenericPlatform/GenericPlatformStackWalk.h"

#include "Math/Vector.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Misc/MessageDialog.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "HAL/PlatformFilemanager.h"

#define MAX_HEX_VALUES 16

UDebugLogLibrarySettings* ULog::Settings;
FDebugLogTimer* ULog::Timer;
FDateTime ULog::SavedDateTime;
FString ULog::LogFilename;
bool ULog::bIsShippingBuild;

void ULog::PostInitProperties()
{
	Super::PostInitProperties();

	Settings = GetMutableDefault<UDebugLogLibrarySettings>();
	Settings->AddToRoot();

	SavedDateTime = FDateTime::Now();
	LogFilename = "DebugLog-" + SavedDateTime.ToString() + ".log";
	
#if (!UE_BUILD_SHIPPING || UE_BUILD_TEST)
	bIsShippingBuild = false;
#elif UE_BUILD_SHIPPING
	bIsShippingBuild = true;
#else
	bIsShippingBuild = false;
#endif
}

void ULog::FinishDestroy()
{
	Settings->RemoveFromRoot();

#if !UE_BUILD_SHIPPING
	WriteToLogFile(LogFilename, "Engine shutdown");
	WriteToLogFile(LogFilename, "Log file closed");
#endif	

	Super::FinishDestroy();
}

void ULog::ObjectValidity(UObject* InObject, const bool bSilenceOnError, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (InObject)
		LogMessage_Internal(InObject->GetName() + " is valid", DL_Success, "", "", Settings->SuccessColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogObjectValidity.GetCategoryName().ToString());
	else if (!bSilenceOnError)
		LogMessage_Internal("None (Object is null)", DL_Error, "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogObjectValidity.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::ObjectName(UObject* InObject, const bool bSilenceOnError, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (InObject)
		LogMessage_Internal(InObject->GetName(), DL_Info, "", "", Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogMessage.GetCategoryName().ToString());
	else if (!bSilenceOnError)
		LogMessage_Internal("None (Object is null)", DL_Error, "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Success:
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Warning:
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Error:
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Fatal:
		Fatal(Message);
	break;

	default:
	break;
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FName& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, FName ViewportKeyName)
{	
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Success:
		Success(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Warning:
		Warning(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Error:
		Error(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
	break;

	case DL_Fatal:
		Fatal(Message.ToString());
	break;

	default:
	break;
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::DebugMessage_WithCondition(const EDebugLogType LogSeverity, const bool bCondition, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		DebugMessage(LogSeverity, Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Crash(const FString& Message, const FString& FromFunction)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogCrash, Fatal, TEXT("%s%s | %s"), NET_MODE_PREFIX, *FromFunction, *Message)
#elif (UE_BUILD_SHIPPING)
	FString ErrorMessage = "";
	FString Line1 = "A function from the DebugLogLibrary plugin was called from C++ or Blueprint!\n";
	FString Line2 = FromFunction.IsEmpty() ? "" : FString("Function name:\n") + FromFunction + "\n";
	FString Line3 = FString("\nDebugLogLibrary plugin does not work in a Shipping build.");
	FString Line4 = FString(" \n\nFor C++ users: \nRemove all ULog:: calls or wrap them with a #if guard!");
	FString Line5 = FString(" \n\nFor Blueprint users: \nRemove or use the Disabled option for all nodes that use the DebugLogLibrary plugin from all blueprint graphs.");
	FString Line6 = FString("\n\nAlternatively, you can disable this feature by going to Project Settings -> Debug Log Library and uncheck 'CrashGameInShippingBuildConfiguration'");

	if (Settings->bCrashGameInShippingBuildConfiguration && Message.IsEmpty())
	{
		ErrorMessage = Line1 + Line2 + Line3 + Line4 + Line5 + Line6;
	}
	else
	{
		if (!FromFunction.IsEmpty())
			ErrorMessage = FromFunction + " | " + Message;
		else
			ErrorMessage = Message;
	}
	
	FMessageDialog ErrorDialogBox;
	FText Title = FText::FromString("Runtime Error");
	ErrorDialogBox.Open(EAppMsgType::Ok, FText::FromString(ErrorMessage), &Title);

	QuitApplication_Internal();
#endif
}

void ULog::Fatal(const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Fatal_WithCondition(const FString& Message, const bool bCondition)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Fatal(Message);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Error(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = FString("Error: ") + Message;
	else
		NewMessage = Message;

	LogMessage_Internal(NewMessage, DL_Error, "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogError.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Error_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Success(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = FString("Success: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, DL_Success, "", "", Settings->SuccessColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogSuccess.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Success_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Warning(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = FString("Warning: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, DL_Warning, "", "", Settings->WarningColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogWarning.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Warning_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Info(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix)
		NewMessage = FString("Info: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, DL_Info, "", "", Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Info_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Hello(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Hello", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Hey(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Hey", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Bye(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Bye", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Goodbye(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Goodbye", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Cya(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Cya", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Wassup(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Wassup", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Yo(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yo", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Yes(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yes", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Yes(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yes", DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::No(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("No", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LineBreak(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(" ", DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, NAME_None, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LineBreak_Symbol(const FString& Symbol, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(Symbol.IsEmpty() ? " " : Symbol, DL_Info, "", "", Settings->InfoColor, LoggingOption, 5.0f, NAME_None, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::StartDebugTimer(const FString& Description)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Timer)
	{
		delete Timer;
		Timer = nullptr;
	}
	
	Timer = new FDebugLogTimer();

	Timer->Start(Description);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::StopDebugTimer(const bool bAutoDetermineTimeUnit, const EDebugLogTimeUnit DisplayIn, const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!Timer)
	{
		Error("Failed to stop the timer! Call StartDebugTimer to begin the timer and call StopDebugTimer when you're finished.", LO_Both);
		return;
	}

	Timer->Stop();

	if (bAutoDetermineTimeUnit)
	{
		// < 1,000 microseconds
		if (Timer->GetDurationInMicroseconds() < 1000)
			Time(Timer->GetDurationInMicroseconds(), DLTU_Microseconds, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption);
		// > 1,000 microseconds AND < 1,000,000 microseconds
		else if (Timer->GetDurationInMicroseconds() > 1000 && Timer->GetDurationInMicroseconds() < 1000000)
			Time(Timer->GetDurationInMilliseconds(), DLTU_Milliseconds, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption);
		else
			Time(Timer->GetDurationInSeconds(), DLTU_Seconds, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption);
	}
	else
	{
		float Duration = Timer->GetDuration();
		switch (DisplayIn)
		{
			case DLTU_Nanoseconds:
			Duration = Timer->GetDuration();
			break;

			case DLTU_Microseconds:
			Duration = Timer->GetDurationInMicroseconds();
			break;

			case DLTU_Milliseconds:
			Duration = Timer->GetDurationInMilliseconds();
			break;
			
			case DLTU_Seconds:
			Duration = Timer->GetDurationInSeconds();
			break;
			
			case DLTU_Minutes:
			Duration = Timer->GetDurationInSeconds() / 60;
			break;
			
			case DLTU_Hours:
			Duration = Timer->GetDurationInSeconds() / 3600;
			break;
			
			case DLTU_Days:
			Duration = Timer->GetDurationInSeconds() / 86400;
			break;
			
			case DLTU_Weeks:
			Duration = Timer->GetDurationInSeconds() / 604800;
			break;
			
			case DLTU_Months:
			Duration = Timer->GetDurationInSeconds() / 2.628e+6;
			break;
			
			case DLTU_Years:
			Duration = Timer->GetDurationInSeconds() / 3.154e+7;
			break;

			case DLTU_Decades:
			Duration = Timer->GetDurationInSeconds() / 3.154e+8;
			break;
			
			case DLTU_Centuries:
			Duration = Timer->GetDurationInSeconds() / 3.154e+9;
			break;

			case DLTU_Millennium:
			Duration = Timer->GetDurationInSeconds() / 3.154e+10;
			break;

			default:
			break;
		}

		Time(Duration, DisplayIn, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption, 5.0f, ViewportKeyName);
	}

	delete Timer;
	Timer = nullptr;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

FString ULog::InBrackets(const FString& String)
{
	return "(" + String + ")";
}

FName ULog::InBrackets(const FName& Name)
{
	return *("(" + Name.ToString() + ")");
}

FText ULog::InBrackets(const FText& Text)
{
	return FText::FromString("(" + Text.ToString() + ")");
}

void ULog::No(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("No", DL_Error, Prefix, Suffix, Settings->ErrorColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Valid(const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Valid", DL_Success, "", "", Settings->SuccessColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Valid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Valid", DL_Success, Prefix, Suffix, Settings->SuccessColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Invalid(const ELoggingOptions LoggingOption, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Invalid", DL_Error, "", "", Settings->ErrorColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Invalid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Invalid", DL_Error, Prefix, Suffix, Settings->ErrorColor, LoggingOption, 5.0f, ViewportKeyName, LogMessage.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Number(const int8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const double Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const long Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogLongInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const int64 Number, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const uint64 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const double Number, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Number(const long Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	LogLongInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::LogArray_Internal(const TArray<FString>& InArray, const FString& Prefix, const FString& Suffix, const ELoggingOptions& LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (LoggingOption)
	{
		case LO_Viewport:
		if (!Suffix.IsEmpty())
			LogMessage_Internal("", DL_Info, "", Suffix, Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());

		for (int32 i = InArray.Num() - 1; i >= 0; i--)
		{
			LogMessage_Internal(FString::FromInt(i) + ": [" + InArray[i] + "]", DL_Info, "", "", Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		}

		if (!Prefix.IsEmpty())
			LogMessage_Internal("", DL_Info, Prefix, "", Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		break;

		case LO_Console:
		if (!Suffix.IsEmpty())
			LogMessage_Internal("", DL_Info, "", Suffix, Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());

		for (int32 i = 0; i < InArray.Num(); i++)
		{
			LogMessage_Internal(FString::FromInt(i) + ": [" + InArray[i] + "]", DL_Info, "", "", Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		}

		if (!Prefix.IsEmpty())
			LogMessage_Internal("", DL_Info, Prefix, "", Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		break;

		case LO_Both:
		LogMessage_Internal("", DL_Info, "", Suffix, Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString(), false);

		for (int32 i = InArray.Num() - 1; i >= 0; i--)
		{
			LogMessage_Internal(FString::FromInt(i) + ": [" + InArray[i] + "]", DL_Info, "", "", Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString(), false);
		}
		
		LogMessage_Internal("", DL_Info, Prefix, "", Settings->InfoColor, LO_Viewport, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString(), false);

		if (!Prefix.IsEmpty())
			LogMessage_Internal("", DL_Info, Prefix, "", Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());

		for (int32 i = 0; i < InArray.Num(); i++)
		{
			LogMessage_Internal(FString::FromInt(i) + ": [" + InArray[i] + "]", DL_Info, "", "", Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		}
		
		if (!Suffix.IsEmpty())
			LogMessage_Internal("", DL_Info, "", Suffix, Settings->InfoColor, LO_Console, TimeToDisplay, NAME_None, LogArray.GetCategoryName().ToString());
		break;

		case LO_NoLog:
		break;

		default:
		break;
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Int32(TArray<int32> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const int32& Element : InArray)
	{
		InStringArray.Add(FString::FromInt(Element));
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Int64(TArray<int64> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const int64& Element : InArray)
	{
		InStringArray.Add(FString::FromInt(Element));
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Float(TArray<float> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const int32& Element : InArray)
	{
		InStringArray.Add(FString::SanitizeFloat(Element));
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Double(TArray<double> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const int32& Element : InArray)
	{
		InStringArray.Add(FString::SanitizeFloat(Element));
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Bool(TArray<bool> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const bool& Element : InArray)
	{
		if (Element)
			InStringArray.Add(FString("True"));
		else
			InStringArray.Add(FString("False"));
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Vector(TArray<FVector> InArray, const bool bCompact, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FVector& Element : InArray)
	{
		if (bCompact)
		{
			FString VectorAsCompactString = FString::SanitizeFloat(Element.X) + ", " + FString::SanitizeFloat(Element.Y) + ", " + FString::SanitizeFloat(Element.Z);
			InStringArray.Add(VectorAsCompactString);
		}
		else
		{
			InStringArray.Add(Element.ToString());
		}
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Vector2D(TArray<FVector2D> InArray, const bool bCompact, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FVector2D& Element : InArray)
	{
		if (bCompact)
		{
			FString VectorAsCompactString = FString::SanitizeFloat(Element.X) + ", " + FString::SanitizeFloat(Element.Y);
			InStringArray.Add(VectorAsCompactString);
		}
		else
		{
			InStringArray.Add(Element.ToString());
		}
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Rotator(TArray<FRotator> InArray, const bool bCompact, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FRotator& Element : InArray)
	{
		if (bCompact)
		{
			FString RotatorAsCompactString = FString::SanitizeFloat(Element.Roll) + ", " + FString::SanitizeFloat(Element.Pitch) + ", " + FString::SanitizeFloat(Element.Yaw);
			InStringArray.Add(RotatorAsCompactString);
		}
		else
		{
			InStringArray.Add(Element.ToString());
		}
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Transform(TArray<FTransform> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FTransform& Element : InArray)
	{
		InStringArray.Add(Element.ToString());
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Quat(TArray<FQuat> InArray, const bool bCompact, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FQuat& Element : InArray)
	{
		if (bCompact)
		{
			FString QuatAsCompactString = FString::SanitizeFloat(Element.X) + ", " + FString::SanitizeFloat(Element.Y) + ", " + FString::SanitizeFloat(Element.Z) + ", " + FString::SanitizeFloat(Element.W);
			InStringArray.Add(QuatAsCompactString);
		}
		else
		{
			InStringArray.Add(Element.ToString());
		}
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Matrix(TArray<FMatrix> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FMatrix& Element : InArray)
	{
		InStringArray.Add(Element.ToString());
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_String(const TArray<FString> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogArray_Internal(InArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Name(TArray<FName> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FName& Element : InArray)
	{
		InStringArray.Add(Element.ToString());
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Text(TArray<FText> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FText& Element : InArray)
	{
		InStringArray.Add(Element.ToString());
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_DateTime(TArray<FDateTime> InArray, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FDateTime& Element : InArray)
	{
		InStringArray.Add(Element.ToString());
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Array_Color(TArray<FLinearColor> InArray, const bool bCompact, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FString> InStringArray;
	InStringArray.Reserve(InArray.Num());
	for (const FLinearColor& Element : InArray)
	{
		if (bCompact)
		{
			FString ColorAsCompactString = FString::SanitizeFloat(Element.R) + ", " + FString::SanitizeFloat(Element.G) + ", " + FString::SanitizeFloat(Element.B) + ", " + FString::SanitizeFloat(Element.A);
			InStringArray.Add(ColorAsCompactString);
		}
		else
		{
			InStringArray.Add(Element.ToString());
		}
	}

	LogArray_Internal(InStringArray, Prefix, Suffix, LoggingOption, TimeToDisplay);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Percent(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(FString::SanitizeFloat(Number) + "%", DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Percent(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Percent(Number, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Bool(const bool bBoolToTest, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bBoolToTest)
		LogMessage_Internal("True", DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogBool.GetCategoryName().ToString());
	else
		LogMessage_Internal("False", DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogBool.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Bool(const bool bBoolToTest, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Bool(bBoolToTest, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Vector(const FVector& InVector, const bool bCompact, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalVectorString;
	if (bCompact)
	{
		FinalVectorString = "[" + FString::SanitizeFloat(InVector.X) + ", " + FString::SanitizeFloat(InVector.Y) + ", " + FString::SanitizeFloat(InVector.Z) + "]";
	}
	else
	{
		FinalVectorString = InVector.ToString();
	}
	
	LogMessage_Internal(FinalVectorString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogVector.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Vector(const FVector& InVector, const bool bCompact, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Vector(InVector, bCompact, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Vector2D(const FVector2D& InVector2D, const bool bCompact, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalVectorString;
	if (bCompact)
	{
		FinalVectorString = "[" + FString::SanitizeFloat(InVector2D.X) + ", " + FString::SanitizeFloat(InVector2D.Y) + "]";
	}
	else
	{
		FinalVectorString = InVector2D.ToString();
	}
	
	LogMessage_Internal(FinalVectorString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogVector.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Vector2D(const FVector2D& InVector2D, const bool bCompact, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Vector2D(InVector2D, bCompact, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Rotator(const FRotator& InRotator, const bool bCompact, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalRotatorString;
	if (bCompact)
	{
		FinalRotatorString = "[" + FString::SanitizeFloat(InRotator.Roll) + ", " + FString::SanitizeFloat(InRotator.Pitch) + ", " + FString::SanitizeFloat(InRotator.Yaw) + "]";
	}
	else
	{
		FinalRotatorString = InRotator.ToString();
	}
	
	LogMessage_Internal(FinalRotatorString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogRotator.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Rotator(const FRotator& InRotator, const bool bCompact, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Rotator(InRotator, bCompact, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Transform(const FTransform& InTransform, const FString& Prefix, const bool bFormat, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		if (bFormat)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
			{
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
				
				WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + NET_MODE_PREFIX + Prefix);
			}

			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Location: ") + InTransform.GetLocation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Rotation: ") + InTransform.GetRotation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Scale: ") + InTransform.GetScale3D().ToString());
		}
		else
		{
			LogMessage_Internal(InTransform.ToString(), DL_Info, Prefix, "", Settings->InfoColor, LO_Viewport, TimeToDisplay, ViewportKeyName, LogTransform.GetCategoryName().ToString());
		}
	}
	else if (LoggingOption == LO_Console)
	{
		if (bFormat)
		{
			if (!Prefix.IsEmpty())
				UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
			
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())

			if (!Prefix.IsEmpty())
				WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + NET_MODE_PREFIX + Prefix);

			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Location: ") + InTransform.GetLocation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Rotation: ") + InTransform.GetRotation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Scale: ") + InTransform.GetScale3D().ToString());
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE_PREFIX, *Prefix, *InTransform.ToString())
			
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + NET_MODE_PREFIX + Prefix + InTransform.ToString());
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (bFormat)
		{
			if (!Prefix.IsEmpty())
				UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)

			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
			{
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);

				WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + NET_MODE_PREFIX + Prefix);
			}

			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Location: ") + InTransform.GetLocation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Rotation: ") + InTransform.GetRotation().ToString());
			WriteToLogFile(LogFilename, LogTransform.GetCategoryName().ToString() + ": Info: " + FString("Scale: ") + InTransform.GetScale3D().ToString());
		}
		else
		{
			LogMessage_Internal(InTransform.ToString(), DL_Info, Prefix, "", Settings->InfoColor, LO_Both, TimeToDisplay, ViewportKeyName, LogTransform.GetCategoryName().ToString());
		}
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Transform(const FTransform& InTransform, const ELoggingOptions LoggingOption, const bool bFormat, const float TimeToDisplay, const FName ViewportKeyName)
{
	Transform(InTransform, "", bFormat, LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Quat(const FQuat& Quaternion, const bool bCompact, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalQuatString;
	if (bCompact)
	{
		FinalQuatString = "[" + FString::SanitizeFloat(Quaternion.X) + ", " + FString::SanitizeFloat(Quaternion.Y) + ", " + FString::SanitizeFloat(Quaternion.Z) + ", " + FString::SanitizeFloat(Quaternion.W) + "]";
	}
	else
	{
		FinalQuatString = Quaternion.ToString();
	}
	
	LogMessage_Internal(FinalQuatString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogQuaternion.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Quat(const FQuat& Quaternion, const bool bCompact, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Quat(Quaternion, bCompact, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Matrix(const FMatrix& InMatrix, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InMatrix.ToString(), DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogMatrix.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Matrix(const FMatrix& InMatrix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Matrix(InMatrix, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Color(const FLinearColor& InColor, const bool bCompact, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalColorString;
	if (bCompact)
	{
		FinalColorString = "[" + FString::SanitizeFloat(InColor.R) + ", " + FString::SanitizeFloat(InColor.G) + ", " + FString::SanitizeFloat(InColor.B) + ", " + FString::SanitizeFloat(InColor.A) + "]";
	}
	else
	{
		FinalColorString = InColor.ToString();
	}
	
	LogMessage_Internal(FinalColorString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogFColor.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Color(const FLinearColor& InColor, const bool bCompact, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Color(InColor, bCompact, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);	
}

void ULog::DateTime(const FDateTime& InDateTime, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InDateTime.ToString(), DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogDateTime.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::DateTime(const FDateTime& InDateTime, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	DateTime(InDateTime, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::MessageInWorld(const FString& Message, const FVector& WorldLocation, const float FontScale, const FString& Prefix, const FString& Suffix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (GWorld)
		DrawDebugString(GWorld, WorldLocation, Prefix + Message + Suffix, nullptr, FColor::White, TimeToDisplay, true, FontScale);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::MessageInWorld(const FString& Message, const FVector& WorldLocation, const float FontScale, const float TimeToDisplay)
{
	MessageInWorld(Message, WorldLocation, FontScale, "", "", TimeToDisplay);
}

void ULog::Temperature(const float InTemperatureValue, const EDebugLogTemperatureUnit TemperatureUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";

	switch (TemperatureUnit)
	{
	case DLTU_Celsius:
		UnitSymbol = "C";
	break;

	case DLTU_Fahrenheit:
		UnitSymbol = "F";
	break;

	case DLTU_Kelvin:
		UnitSymbol = "K";
	break;
	}

	LogUnitSystem(InTemperatureValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Temperature(const float InTemperatureValue, const EDebugLogTemperatureUnit TemperatureUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Temperature(InTemperatureValue, TemperatureUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Volume(const float InVolumeValue, const EDebugLogVolumeUnit VolumeUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (VolumeUnit)
	{
	case DLVU_Litre:
		UnitSymbol = "L";
	break;
	case DLVU_Millilitre:
		UnitSymbol = "ml";
	break;
	case DLVU_Gallon:
		UnitSymbol = "GAL";
	break;
	case DLVU_Pint:
		UnitSymbol = "pt";
	break;
	case DLVU_Quart:
		UnitSymbol = "qt";
	break;
	}
	
	LogUnitSystem(InVolumeValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Volume(const float InVolumeValue, const EDebugLogVolumeUnit VolumeUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Volume(InVolumeValue, VolumeUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Data(const float InDataValue, const EDebugLogDataUnit DataUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (DataUnit)
	{
	case DLDU_Bit:
		UnitSymbol = "bit";
	break;
	case DLDU_Byte:
		UnitSymbol = "byte";
	break;
	case DLDU_Kilobyte:
		UnitSymbol = "KB";
	break;
	case DLDU_Megabyte:
		UnitSymbol = "MB";
	break;
	case DLDU_Gigabyte:
		UnitSymbol = "GB";
	break;
	case DLDU_Terabyte:
		UnitSymbol = "TB";
	break;
	case DLDU_Petabyte:
		UnitSymbol = "PB";
	break;
	}

	if (InDataValue > 1 && (DataUnit == DLDU_Bit || DataUnit == DLDU_Byte))
	{
		UnitSymbol.Append("s");
	}
	
	LogUnitSystem(InDataValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Data(const float InDataValue, const EDebugLogDataUnit DataUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Data(InDataValue, DataUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Length(const float InLengthValue, const EDebugLogLengthUnit LengthUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (LengthUnit)
	{
	case DLLU_Centimeter:
		UnitSymbol = "cm";
	break;
	case DLLU_Micrometer:
		UnitSymbol = "um";
	break;
	case DLLU_Millimeter:
		UnitSymbol = "mm";
	break;
	case DLLU_Meter:
		UnitSymbol = "m";
	break;
	case DLLU_Kilometer:
		UnitSymbol = "km";
	break;
	case DLLU_Inch:
		UnitSymbol = "in";
	break;
	case DLLU_Feet:
		UnitSymbol = "ft";
	break;
	case DLLU_Mile:
		UnitSymbol = "mi";
	break;
	case DLLU_NauticalMile:
		UnitSymbol = "nm";
	break;
	case DLLU_Yard:
		UnitSymbol = "yd";
	break;
	case DLLU_Lightyears:
        UnitSymbol = "ly";
	break;
	}

	LogUnitSystem(InLengthValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Length(const float InLengthValue, const EDebugLogLengthUnit LengthUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Length(InLengthValue, LengthUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Mass(const float InMassValue, const EDebugLogMassUnit MassUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (MassUnit)
	{
	case DLMU_Microgram:
		UnitSymbol = "ug";
	break;
	case DLMU_Milligram:
		UnitSymbol = "mg";
	break;
	case DLMU_Gram:
		UnitSymbol = "g";
	break;
	case DLMU_Kilogram:
		UnitSymbol = "kg";
	break;
	case DLMU_Ounce:
		UnitSymbol = "oz";
	break;
	case DLMU_Stone:
		UnitSymbol = "st";
	break;
	case DLMU_Pound:
		UnitSymbol = "lb";
	break;
	case DLMU_Tonne:
		UnitSymbol = "t";
	break;
	}

	LogUnitSystem(InMassValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Mass(const float InMassValue, const EDebugLogMassUnit MassUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Mass(InMassValue, MassUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Speed(const float InSpeedValue, const EDebugLogSpeedUnit SpeedUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (SpeedUnit)
	{
	case DLSU_CentimetersPerSec:
		UnitSymbol = "cm/s";
	break;
	case DLSU_CentimetersPerHour:
		UnitSymbol = "cm/h";
	break;
	case DLSU_MicrometersPerSec:
		UnitSymbol = "um/s";
	break;
	case DLSU_MicrometersPerHour:
		UnitSymbol = "um/h";
	break;
	case DLSU_MillimetersPerSec:
		UnitSymbol = "mm/s";
	break;
	case DLSU_MillimetersPerHour:
		UnitSymbol = "mm/h";
	break;
	case DLSU_MetersPerSec:
		UnitSymbol = "m/s";
	break;
	case DLSU_MetersPerHour:
		UnitSymbol = "m/h";
	break;
	case DLSU_KilometersPerSec:
		UnitSymbol = "km/s";
	break;
	case DLSU_KilometersPerHour:
		UnitSymbol = "km/h";
	break;
	case DLSU_InchesPerSec:
		UnitSymbol = "in/s";
	break;
	case DLSU_InchesPerHour:
		UnitSymbol = "in/h";
	break;
	case DLSU_FeetPerSec:
		UnitSymbol = "ft/s";
	break;
	case DLSU_FeetPerHour:
		UnitSymbol = "ft/h";
	break;
	case DLSU_MilesPerSec:
		UnitSymbol = "mi/s";
	break;
	case DLSU_MilesPerHour:
		UnitSymbol = "mi/h";
	break;
	case DLSU_YardsPerSec:
		UnitSymbol = "yd/s";
	break;
	case DLSU_YardsPerHour:
		UnitSymbol = "yd/h";
	break;
	case DLSU_Knots:
		UnitSymbol = "kn";
	break;
	}

	LogUnitSystem(InSpeedValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Speed(const float InSpeedValue, const EDebugLogSpeedUnit SpeedUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Speed(InSpeedValue, SpeedUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Time(const float InTimeValue, const EDebugLogTimeUnit TimeUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (TimeUnit)
	{
	case DLTU_Nanoseconds:
		UnitSymbol = "ns";
	break;
	case DLTU_Microseconds:
		UnitSymbol = "us";
	break;
	case DLTU_Milliseconds:
		UnitSymbol = "ms";
	break;
	case DLTU_Seconds:
		UnitSymbol = "sec";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Minutes:
		UnitSymbol = "min";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Hours:
		UnitSymbol = "hr";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Days:
		UnitSymbol = " day";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Weeks:
		UnitSymbol = " wk";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Months:
		UnitSymbol = " mth";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Years:
		UnitSymbol = " yr";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Decades:
		UnitSymbol = " decade";

		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	case DLTU_Centuries:
		UnitSymbol = " century";
		
		if (InTimeValue > 1)
			UnitSymbol = " centuries";
	break;
	case DLTU_Millennium:
		UnitSymbol = " millennium";
		
		if (InTimeValue > 1)
			UnitSymbol.Append("s");
	break;
	}
	
	LogUnitSystem(InTimeValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Time(const float InTimeValue, const EDebugLogTimeUnit TimeUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Time(InTimeValue, TimeUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Angle(const float InAngleValue, const EDebugLogAngularUnit AngleUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (AngleUnit)
	{
		case DLAU_Degrees:
            UnitSymbol = "deg";
		break;
		
		case DLAU_Radians:
            UnitSymbol = "rad";
		break;
	}
	
	LogUnitSystem(InAngleValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Angle(const float InAngleValue, const EDebugLogAngularUnit AngleUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Angle(InAngleValue, AngleUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Force(const float InForceValue, const EDebugLogForceUnit ForceUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (ForceUnit)
	{
		case DLFU_Newtons:
		UnitSymbol = "N";
		break;

		case DLFU_PoundsForce:
		UnitSymbol = "lbf";
		break;

		case DLFU_KilogramsForce:
		UnitSymbol = "kgf";
		break;

		default:
		break;
	}
	
	LogUnitSystem(InForceValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Force(const float InForceValue, const EDebugLogForceUnit ForceUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Force(InForceValue, ForceUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Frequency(const float InFrequencyValue, const EDebugLogFrequencyUnit FrequencyUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (FrequencyUnit)
	{
		case DLFU_Hertz:
		UnitSymbol = "Hz";
		break;

		case DLFU_Kilohertz:
		UnitSymbol = "kHz";
		break;

		case DLFU_Megahertz:
		UnitSymbol = "MHz";
		break;

		case DLFU_Gigahertz:
		UnitSymbol = "GHz";
		break;

		case DLFU_RevolutionsPerMinute:
		UnitSymbol = "rpm";
		break;

		default:
        break;
	}
	
	LogUnitSystem(InFrequencyValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Frequency(const float InFrequencyValue, const EDebugLogFrequencyUnit FrequencyUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Frequency(InFrequencyValue, FrequencyUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Light(const float InLightValue, const EDebugLogLightUnit LightUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (LightUnit)
	{
		case DLLU_Lumens:
		UnitSymbol = "lm";
		break;

		case DLLU_Candela:
		UnitSymbol = "cd";
		break;

		case DLLU_Lux:
		UnitSymbol = "lx";
		break;

		case DLLU_CandelaPerMeter2:
		UnitSymbol = "cd/m2";
		break;

		default:
        break;
	}
	
	LogUnitSystem(InLightValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Light(const float InLightValue, const EDebugLogLightUnit LightUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Light(InLightValue, LightUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Unit(float InUnitValue, EUnit InUnit, bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (InUnit)
	{
		case EUnit::Micrometers:
		UnitSymbol = "um";
		break;
		case EUnit::Millimeters:
		UnitSymbol = "mm";
		break;
		case EUnit::Centimeters:
		UnitSymbol = "cm";
		break;
		case EUnit::Meters:
		UnitSymbol = "m";
		break;
		case EUnit::Kilometers:
		UnitSymbol = "km";
		break;
		case EUnit::Inches:
		UnitSymbol = "in";
		break;
		case EUnit::Feet:
		UnitSymbol = "ft";
		break;
		case EUnit::Yards:
		UnitSymbol = "yd";
		break;
		case EUnit::Miles:
		UnitSymbol = "mi";
		break;
		case EUnit::Lightyears:
		UnitSymbol = "ly";
		break;
		case EUnit::Degrees:
		UnitSymbol = "deg";
		break;
		case EUnit::Radians:
		UnitSymbol = "rad";
		break;
		case EUnit::MetersPerSecond:
		UnitSymbol = "m/s";
		break;
		case EUnit::KilometersPerHour:
		UnitSymbol = "km/h";
		break;
		case EUnit::MilesPerHour:
		UnitSymbol = "mi/h";
		break;
		case EUnit::Celsius:
		UnitSymbol = "C";
		break;
		case EUnit::Farenheit:
		UnitSymbol = "F";
		break;
		case EUnit::Kelvin:
		UnitSymbol = "K";
		break;
		case EUnit::Micrograms:
		UnitSymbol = "ug";
		break;
		case EUnit::Milligrams:
		UnitSymbol = "mg";
		break;
		case EUnit::Grams:
		UnitSymbol = "g";
		break;
		case EUnit::Kilograms:
		UnitSymbol = "kg";
		break;
		case EUnit::MetricTons:
		UnitSymbol = "t";
		break;
		case EUnit::Ounces:
		UnitSymbol = "oz";
		break;
		case EUnit::Pounds:
		UnitSymbol = "lb";
		break;
		case EUnit::Stones:
		UnitSymbol = "st";
		break;
		case EUnit::Newtons:
		UnitSymbol = "N";
		break;
		case EUnit::PoundsForce:
		UnitSymbol = "lbf";
		break;
		case EUnit::KilogramsForce:
		UnitSymbol = "kgf";
		break;
		case EUnit::Hertz:
		UnitSymbol = "Hz";
		break;
		case EUnit::Kilohertz:
		UnitSymbol = "kHz";
		break;
		case EUnit::Megahertz:
		UnitSymbol = "MHz";
		break;
		case EUnit::Gigahertz:
		UnitSymbol = "GHz";
		break;
		case EUnit::RevolutionsPerMinute:
		UnitSymbol = "rpm";
		break;
		case EUnit::Bytes:
		UnitSymbol = "byte";
		break;
		case EUnit::Kilobytes:
		UnitSymbol = "KB";
		break;
		case EUnit::Megabytes:
		UnitSymbol = "MB";
		break;
		case EUnit::Gigabytes:
		UnitSymbol = "GB";
		break;
		case EUnit::Terabytes:
		UnitSymbol = "TB";
		break;
		case EUnit::Lumens:
		UnitSymbol = "lm";
		break;
		case EUnit::Candela:
		UnitSymbol = "cd";
		break;
		case EUnit::Lux:
		UnitSymbol = "lx";
		break;
		case EUnit::CandelaPerMeter2:
		UnitSymbol = "cd/m2";
		break;
		case EUnit::Milliseconds:
		UnitSymbol = "ms";
		break;
		case EUnit::Seconds:
		UnitSymbol = "sec";
		break;
		case EUnit::Minutes:
		UnitSymbol = "min";
		break;
		case EUnit::Hours:
		UnitSymbol = "hr";
		break;
		case EUnit::Days:
		UnitSymbol = "day";
		break;
		case EUnit::Months:
		UnitSymbol = "mth";
		break;
		case EUnit::Years:
		UnitSymbol = "yr";
		break;
		case EUnit::PixelsPerInch:
		UnitSymbol = "ppi";
		break;
		case EUnit::Percentage:
		UnitSymbol = "%";
		break;
		case EUnit::Multiplier:
		UnitSymbol = "x";
		break;
		case EUnit::Unspecified:
		UnitSymbol = "unspecified";
		break;
		
		default:
		UnitSymbol = "unspecified";
		break;
	}

	if (InUnit == EUnit::Bytes || InUnit == EUnit::Seconds || InUnit == EUnit::Minutes || InUnit == EUnit::Hours || InUnit == EUnit::Days || InUnit == EUnit::Months || InUnit == EUnit::Years)
	{
		UnitSymbol.Append("s");
	}
	
	LogUnitSystem(InUnitValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Unit(const float InUnitValue, const EUnit InUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Unit(InUnitValue, InUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Dollar(const float InDollarValue, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogCurrencyUnitSystem(InDollarValue, "$", bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Dollar(const float InDollarValue, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	Dollar(InDollarValue, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay, ViewportKeyName);
}

void ULog::Sphere(const FSphere& Sphere, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Sphere.W));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Sphere.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSphere, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogSphere, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogSphere, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogSphere, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSphere, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogSphere, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogSphere, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogSphere, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Sphere.W));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Sphere.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Box(const FBox& Box, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Max: ") + Box.Max.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Min: ") + Box.Min.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogBox, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogBox, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogBox, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogBox, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogBox, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogBox, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogBox, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogBox, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Max: ") + Box.Max.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Min: ") + Box.Min.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Capsule(const FCapsuleShape& Capsule, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Orientation: ") + Capsule.Orientation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Length: ") + FString::SanitizeFloat(Capsule.Length));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Capsule.Radius));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Capsule.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogMessage, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogMessage, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogMessage, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogMessage, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogMessage, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogCapsule, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogCapsule, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogCapsule, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogCapsule, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogCapsule, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogCapsule, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Orientation: ") + Capsule.Orientation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Length: ") + FString::SanitizeFloat(Capsule.Length));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Capsule.Radius));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Capsule.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

bool ULog::AssertEqual_Bool(const bool bActual, const bool bExpected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<bool>(bActual, bExpected, "AssertEqual (Bool)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Integer(const int32 Actual, const int32 Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<int32>(Actual, Expected, "AssertEqual (Integer)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertValue_Integer(const int32 Actual, const int32 Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertValue<int32>(Actual, Expected, ShouldBe, "AssertValue (Integer)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Float(const float Actual, const float Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<float>(Actual, Expected, "AssertEqual (Float)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertValue_Float(const float Actual, const float Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertValue<float>(Actual, Expected, ShouldBe, "AssertValue (Float)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_String(const FString Actual, const FString Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FString>(Actual, Expected, "AssertEqual (String)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_String(const FString Actual, const FString Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FString>(Actual, Expected, "AssertNotEqual (String)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Name(const FName Actual, const FName Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FName>(Actual, Expected, "AssertEqual (Name)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Name(const FName Actual, const FName Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FName>(Actual, Expected, "AssertNotEqual (Name)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Object(UObject* Actual, UObject* Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<UObject>(Actual, Expected, "AssertEqual (Object)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Object(UObject* Actual, UObject* Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<UObject>(Actual, Expected, "AssertNotEqual (Object)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Vector(const FVector Actual, const FVector Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FVector>(Actual, Expected, "AssertEqual (Vector)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Vector2D(const FVector2D Actual, const FVector2D Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FVector2D>(Actual, Expected, "AssertEqual (Vector2D)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Vector(const FVector Actual, const FVector Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FVector>(Actual, Expected, "AssertNotEqual (Vector)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Vector2D(const FVector2D Actual, const FVector2D Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FVector2D>(Actual, Expected, "AssertNotEqual (Vector2D)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Rotator(const FRotator Actual, const FRotator Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FRotator>(Actual, Expected, "AssertEqual (Rotator)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Rotator(const FRotator Actual, const FRotator Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FRotator>(Actual, Expected, "AssertNotEqual (Rotator)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Quat(const FQuat Actual, const FQuat Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FQuat>(Actual, Expected, "AssertEqual (Quat)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Quat(const FQuat Actual, const FQuat Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FQuat>(Actual, Expected, "AssertNotEqual (Quat)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_Transform(const FTransform Actual, const FTransform Expected, const FString Message, const bool bNoScale, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bNoScale && !Actual.EqualsNoScale(Expected))
	{
		AssertFailed(Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);

		return false;
	}

	if (!Actual.Equals(Expected))
	{
		AssertFailed(Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);

		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

bool ULog::AssertNotEqual_Transform(const FTransform Actual, const FTransform Expected, const FString Message, const bool bNoScale, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bNoScale && Actual.EqualsNoScale(Expected))
	{
		AssertFailed(Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);

		return false;
	}

	if (Actual.Equals(Expected))
	{
		AssertFailed(Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);

		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

bool ULog::AssertEqual_Color(const FColor Actual, const FColor Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FColor>(Actual, Expected, "AssertEqual (Color)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertNotEqual_Color(const FColor Actual, const FColor Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertNotEqual<FColor>(Actual, Expected, "AssertNotEqual (Color)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertEqual_DateTime(const FDateTime Actual, const FDateTime Expected, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertEqual<FDateTime>(Actual, Expected, "AssertEqual (DateTime)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::AssertValue_DateTime(const FDateTime Actual, const FDateTime Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
	return AssertValue<FDateTime>(Actual, Expected, ShouldBe, "AssertValue (DateTime)", Message, bCrashOnFailure, LoggingOption, TimeToDisplay, ViewportKeyName);
}

bool ULog::Assert_True(const bool bCondition, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!bCondition)
	{
		if (bCrashOnFailure)
			Crash("AssertTrue | Assert Failed: " + Message);
		else
			Error("AssertTrue | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName);

		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

bool ULog::Assert_False(const bool bCondition, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
	{
		if (bCrashOnFailure)
			Crash("AssertFalse | Assert Failed: " + Message);
		else
			Error("AssertFalse | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName);

		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

bool ULog::Assert_IsValid(UObject* Object, const FString Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!IsValid(Object))
	{
		if (bCrashOnFailure)
			Crash("AssertIsValid | Assert Failed: " + Message);
		else
			Error("AssertIsValid | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName);

		return false;		
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

void ULog::Number_Int_Blueprint(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::Number_Float_Blueprint(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

FString ULog::InBrackets_String_Blueprint(const FString& String)
{
	return "(" + String + ")";
}

FName ULog::InBrackets_Name_Blueprint(const FName Name)
{
	return *("(" + Name.ToString() + ")");
}

FText ULog::InBrackets_Text_Blueprint(const FText Text)
{
	return FText::FromString("(" + Text.ToString() + ")");
}

void ULog::CheckObject(UObject* Object, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!Object)
		ASSERT(Object, Message);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::CheckCondition(const bool bCondition, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!bCondition)
		ASSERT(bCondition, Message);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::CheckNoEntry()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoEntry();
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::CheckNoReEntry()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoReentry();
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::CheckNoRecursion()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoRecursion();
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::UnImplemented()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	unimplemented();
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::EnsureObject(UObject* Object, const bool bAlwaysEnsure, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Message.IsEmpty())
	{
		if (bAlwaysEnsure)
		{
			ensureAlways(Object != nullptr);
		}
		else
		{
			ensure(Object != nullptr);
		}
	}
	else
	{
		if (bAlwaysEnsure)
		{
			ensureAlwaysMsgf(Object != nullptr, TEXT("Ensure (Object): %s"), *Message);
		}
		else
		{
			ensureMsgf(Object != nullptr, TEXT("Ensure (Object): %s"), *Message);
		}
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::EnsureCondition(const bool bCondition, const bool bAlwaysEnsure, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Message.IsEmpty())
	{
		if (bAlwaysEnsure)
		{
			ensureAlways(bCondition);
		}
		else
		{
			ensure(bCondition);
		}
	}
	else
	{
		if (bAlwaysEnsure)
		{
			ensureAlwaysMsgf(bCondition, TEXT("Ensure (Bool): %s"), *Message);
		}
		else
		{
			ensureMsgf(bCondition, TEXT("Ensure (Bool): %s"), *Message);
		}
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogMessage_Internal(const FString& Message, const EDebugLogType LogSeverity, const FString& Prefix, const FString& Suffix, const FColor& InLogColor, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName, const FString& LogCategory, const bool bWriteToLog)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_NoLog)
		return;

	const FString TagSpecifier = Settings->bEnableSpecifiers ? Settings->LogCategorySpecifiers[LogSeverity] : "";
	const FString FinalMessage = NET_MODE_PREFIX + Prefix + Message + Suffix;
	const FString FinalMessage_WithTagSpecifier = NET_MODE_PREFIX + TagSpecifier + Prefix + Message + Suffix;
	
	FString LogVerbosity = Settings->LogCategorySpecifiers[DL_Info];
	if (InLogColor == Settings->WarningColor)
	{
		LogVerbosity = Settings->LogCategorySpecifiers[DL_Warning];
	}
	else if (InLogColor == Settings->ErrorColor)
	{
		LogVerbosity = Settings->LogCategorySpecifiers[DL_Error];
	}
	else if (InLogColor == Settings->SuccessColor)
	{
		LogVerbosity = Settings->LogCategorySpecifiers[DL_Success];
	}
	
	if (LoggingOption == LO_Viewport)
	{
		int32* Key = nullptr;
		
		if (!ViewportKeyName.IsNone())
			Key = Settings->ViewportLogKeys.Find(ViewportKeyName);

		if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Viewport || Settings->ShowSpecifiersIn == LO_Both))
			GEngine->AddOnScreenDebugMessage(Key ? *Key : -1, TimeToDisplay, InLogColor, FinalMessage_WithTagSpecifier);
		else
			GEngine->AddOnScreenDebugMessage(Key ? *Key : -1, TimeToDisplay, InLogColor, FinalMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		if (InLogColor == Settings->ErrorColor)
		{
			if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Console || Settings->ShowSpecifiersIn == LO_Both))
				UE_LOG(LogMessage, Error, TEXT("%s"), *FinalMessage_WithTagSpecifier)
			else
				UE_LOG(LogMessage, Error, TEXT("%s"), *FinalMessage)
		}
		else
		{
			if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Console || Settings->ShowSpecifiersIn == LO_Both))
				UE_LOG(LogMessage, Warning, TEXT("%s"), *FinalMessage_WithTagSpecifier)
			else
				UE_LOG(LogMessage, Warning, TEXT("%s"), *FinalMessage)
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (InLogColor == Settings->ErrorColor)
		{
			if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Console || Settings->ShowSpecifiersIn == LO_Both))
				UE_LOG(LogMessage, Error, TEXT("%s"), *FinalMessage_WithTagSpecifier)
            else
            	UE_LOG(LogMessage, Error, TEXT("%s"), *FinalMessage)
        }
		else
		{
			if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Console || Settings->ShowSpecifiersIn == LO_Both))
				UE_LOG(LogMessage, Warning, TEXT("%s"), *FinalMessage_WithTagSpecifier)
            else
            	UE_LOG(LogMessage, Warning, TEXT("%s"), *FinalMessage)
        }

		int32* Key = nullptr;
		
		if (!ViewportKeyName.IsNone())
			Key = Settings->ViewportLogKeys.Find(ViewportKeyName);

		if (Settings->bEnableSpecifiers && (Settings->ShowSpecifiersIn == LO_Viewport || Settings->ShowSpecifiersIn == LO_Both))
			GEngine->AddOnScreenDebugMessage(Key ? *Key : -1, TimeToDisplay, InLogColor, *FinalMessage_WithTagSpecifier);
		else
			GEngine->AddOnScreenDebugMessage(Key ? *Key : -1, TimeToDisplay, InLogColor, *FinalMessage);
	}

	if (bWriteToLog)
		WriteToLogFile(LogFilename, LogCategory + ": " + LogVerbosity + FinalMessage);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::QuitApplication_Internal()
{
	UKismetSystemLibrary::QuitGame(GWorld, UGameplayStatics::GetPlayerController(GWorld, 0), EQuitPreference::Quit, true);
}

void ULog::LogInt(const platform_int Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalNumber;
	switch (NumberSystem)
	{
		case DLNS_Decimal:
			FinalNumber = FString::FromInt(Number);
		break;

		case DLNS_Hex:
			FinalNumber = DecimalToHex(Number);
		break;

		case DLNS_Binary:
			FinalNumber = DecimalToBinary(Number);
		break;

		case DLNS_Octal:
			FinalNumber = DecimalToOctal(Number);
		break;

		case DLNS_Roman:
			FinalNumber = DecimalToRomanNumeral(Number);
		break;
	}
	
	LogMessage_Internal(FinalNumber, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogUInt(const platform_uint Number, const FString& Prefix, const FString& Suffix, EDebugLogNumberSystems const NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalNumber;
	switch (NumberSystem)
	{
		case DLNS_Decimal:
			FinalNumber = FString::FromInt(Number);
		break;

		case DLNS_Hex:
			FinalNumber = DecimalToHex(Number);
		break;

		case DLNS_Binary:
			FinalNumber = DecimalToBinary(Number);
		break;

		case DLNS_Octal:
			FinalNumber = DecimalToOctal(Number);
		break;

		case DLNS_Roman:
			FinalNumber = DecimalToRomanNumeral(Number);
		break;
	}

	LogMessage_Internal(FinalNumber, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogFloat(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(FString::SanitizeFloat(Number), DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogUnitSystem(const float Value, const FString& UnitSymbol, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString ValueInString = FString::SanitizeFloat(Value);

	if (bConvertValueToInt)
		ValueInString = FString::FromInt(Value);

	LogMessage_Internal(ValueInString + UnitSymbol, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogCurrencyUnitSystem(const float Value, const FString& UnitSymbol, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString ValueInString = FString::SanitizeFloat(Value);

	if (bConvertValueToInt)
		ValueInString = FString::FromInt(Value);

	LogMessage_Internal(UnitSymbol + ValueInString, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

void ULog::LogLongInt(const long Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay, FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString FinalNumber;
	switch (NumberSystem)
	{
		case DLNS_Decimal:
			FinalNumber = FString::FromInt(Number);
		break;

		case DLNS_Hex:
			FinalNumber = DecimalToHex(Number);
		break;

		case DLNS_Binary:
			FinalNumber = DecimalToBinary(Number);
		break;

		case DLNS_Octal:
			FinalNumber = DecimalToOctal(Number);
		break;

		case DLNS_Roman:
			FinalNumber = DecimalToRomanNumeral(Number);
		break;
	}

	LogMessage_Internal(FinalNumber, DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, LogNumber.GetCategoryName().ToString());
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration && LoggingOption != LO_NoLog)
		Crash("", FString(__FUNCTION__));
#endif
}

FString ULog::DecimalToHex(const platform_int DecimalNumber)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DecimalNumber < 10 && DecimalNumber >= 0)
		return FString::FromInt(DecimalNumber);

	platform_int Number = DecimalNumber;

	FString HexValues[MAX_HEX_VALUES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
	FString HexadecimalAsString = "";

	bool bIsNegative = false;
	
	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}

	while (Number > 0)
	{
		const platform_int Remainder = Number % 16;
		HexadecimalAsString = HexValues[Remainder] + HexadecimalAsString;
		Number /= 16;
	}

	if (bIsNegative)
	{
		if (Settings->bUseTwosComplimentForDecimalToHexConversionOnNegativeNumbers)
		{
			// This means the decimal number is -16. So exit early, since it will return 'E0' which is incorrect
			if (HexadecimalAsString == "10")
				return FString("0");

			FString HexadecimalTwosComplimentAsString = "";

			TArray<platform_int> ConvertedHexValues;
			for (platform_int i = 0; i < HexadecimalAsString.GetCharArray().Num()-1; i++)
			{
				ConvertedHexValues.Add(HexDigitToDecimal(FString::Chr(HexadecimalAsString[i])));
			}

			TArray<platform_int> HexTwosComplimentResult;
			for (platform_int i = 0; i < ConvertedHexValues.Num(); i++)
			{
				HexTwosComplimentResult.Add((MAX_HEX_VALUES-1) - ConvertedHexValues[i]);
			}

			HexTwosComplimentResult[HexTwosComplimentResult.Num()-1] = HexTwosComplimentResult.Last() + 1;
			
			for (platform_int i = 0; i < HexTwosComplimentResult.Num(); i++)
			{
				if (HexTwosComplimentResult[i] > 9)
					HexadecimalTwosComplimentAsString.Append(DecimalToHexDigit(HexTwosComplimentResult[i]));
				else
					HexadecimalTwosComplimentAsString.AppendInt(HexTwosComplimentResult[i]);
			}
			
			return HexadecimalTwosComplimentAsString;
		}

		HexadecimalAsString.InsertAt(0, '-');
	}

	return HexadecimalAsString;
#endif

	return FString("");
}

FString ULog::DecimalToBinary(const platform_int DecimalNumber)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DecimalNumber == 0)
		return "0";

	if (DecimalNumber == 1)
		return "1";
	
	// Array to store binary numbers
	platform_int BinaryArray[64];
	platform_int Number = DecimalNumber;
	platform_int i;
	
	bool bIsNegative = false;

	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}

	for (i = 0; Number != 0; i++)
	{
		// Store the remainder in binary array
		BinaryArray[i] = Number % 2;
		Number /= 2;
	}

	FString Result;

	// Adding each digit into Result, in reverse order
	for (platform_int j = i - 1; j >= 0; j--)
		Result.AppendInt(BinaryArray[j]);

	if (bIsNegative)
		Result.InsertAt(0, '-');

	return Result;
#endif

	return "";
}

FString ULog::DecimalToOctal(const platform_int DecimalNumber)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DecimalNumber == 0)
		return "0";

	platform_int OctalArray[64];
	platform_int Number = DecimalNumber;
	platform_int i;

	bool bIsNegative = false;

	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}
	
	for (i = 0; Number != 0; i++)
	{
		OctalArray[i] = Number % 8;
		Number /= 8;
	}

	FString Result;

	// Adding each digit into Result, in reverse order
	for (platform_int j = i - 1; j >= 0; j--)
		Result.AppendInt(OctalArray[j]);

	if (bIsNegative)
		Result.InsertAt(0, '-');

	return Result;
#endif

	return FString("");
}

FString ULog::DecimalToRomanNumeral(platform_int DecimalNumber)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DecimalNumber == 0)
		return "N";

	bool bIsNegative = false;
	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		DecimalNumber = -DecimalNumber;
	}

	TArray<int32> RomanNumeral_Integers = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
	TArray<FString> RomanNumeral_Symbols = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};

	int32 i = RomanNumeral_Symbols.Num() - 1;

	FString Result;
	while (DecimalNumber > 0)
	{	
		platform_int Quotient = DecimalNumber / RomanNumeral_Integers[i];
		DecimalNumber %= RomanNumeral_Integers[i];

		while (Quotient--)
		{
			Result.Append(RomanNumeral_Symbols[i]);
		}

		i--;
	}

	if (bIsNegative)
		Result.InsertAt(0, '-');
	
	return Result;
#endif

	return FString("");
}

platform_int ULog::HexDigitToDecimal(FString HexDigit)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (HexDigit.GetCharArray().Num()-1 > 1)
		return platform_int(0);

	FString HexValues[MAX_HEX_VALUES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
	FString HexValues_Integers[MAX_HEX_VALUES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

	for (int32 i = 0; i < MAX_HEX_VALUES; i++)
	{
		if (HexDigit == HexValues[i])
		{
		#if PLATFORM_64BITS
			return FCString::Atoi64(*HexValues_Integers[i]);
		#else
			return FCString::Atoi(*HexValues_Integers[i]);
		#endif
		}
	}

	return platform_int(0);
#endif

	return -1;
}

FString ULog::DecimalToHexDigit(platform_int DecimalNumber)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DecimalNumber < 10)
		return FString::FromInt(DecimalNumber);

	FString HexValues[MAX_HEX_VALUES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
	FString HexValues_Integers[MAX_HEX_VALUES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

	for (int32 i = 0; i < MAX_HEX_VALUES; i++)
	{
		#if PLATFORM_64BITS
		if (DecimalNumber == FCString::Atoi64(*HexValues_Integers[i]))
			return HexValues[i];
		#else
		if (DecimalNumber == FCString::Atoi(*HexValues_Integers[i]))
			return HexValues[i];
		#endif
	}

	return FString("0");
#endif

	return FString("-1");
}

void ULog::AssertFailed(const FString& Message, const bool bCrashOnFailure, const ELoggingOptions LoggingOption, const float TimeToDisplay, const FName ViewportKeyName)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCrashOnFailure)
		Crash("Assert Failed: " + Message);
	else
		Error("Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName);
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif
}

bool ULog::WriteToLogFile(const FString& FileName, const FString& Text, const bool bAllowOverwriting)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	const FString Directory = GetSavedLogsDirectory() + FileName;

	const FString FinalText = "[" + FDateTime::Now().ToString() + "] " + Text + LINE_TERMINATOR;
	if (bAllowOverwriting)
	{
		return FFileHelper::SaveStringToFile(FinalText, *Directory);
	}

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Directory))
	{
		FString InitialText = "[" + FDateTime::Now().ToString() + "] Log file open" + LINE_TERMINATOR;
		InitialText += FinalText;
		
		FFileHelper::SaveStringToFile(InitialText, *Directory);
		return false;
	}

	IFileHandle* FileHandle = FPlatformFileManager::Get().GetPlatformFile().OpenWrite(*Directory, true);
	if (FileHandle)
	{
		FileHandle->Write(reinterpret_cast<const uint8*>(TCHAR_TO_ANSI(*FinalText)), FinalText.Len());
		delete FileHandle;
		
		return true;
	}
	
	return false;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));

	return false;
#endif
}

FString ULog::GetSavedLogsDirectory()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	return FPaths::ProjectSavedDir() + "Logs/Debug Logs/";
#endif
	
	return "";
}
