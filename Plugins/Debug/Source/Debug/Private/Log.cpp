// Copyright Ali El Saleh 2020

#include "Log.h"
#include "Debug.h"

#include "Engine/Engine.h"

#include "DrawDebugHelpers.h"

#include "Math/Vector.h"

#include "DebugLogLibrarySettings.h"

#define MAX_HEX_VALUES 16

const UDebugLogLibrarySettings* ULog::Settings;
FDebugLogTimer* ULog::Timer;

void ULog::PostInitProperties()
{
	Super::PostInitProperties();

	Settings = GetDefault<UDebugLogLibrarySettings>();
}

void ULog::ObjectValidity(UObject* InObject, const bool bSilenceOnError, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (InObject)
		LogMessage_Internal(InObject->GetName() + " is valid", "", "", Settings->SuccessColor, LoggingOption, TimeToDisplay);
	else if (!bSilenceOnError)
		LogMessage_Internal("None (Object is null)", "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::ObjectName(UObject* InObject, const bool bSilenceOnError, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (InObject)
		LogMessage_Internal(InObject->GetName(), "", "", Settings->InfoColor, LoggingOption, TimeToDisplay);
	else if (!bSilenceOnError)
		LogMessage_Internal("None (Object is null)", "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Success:
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Warning:
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Error:
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Fatal:
		UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
	break;

	default:
		break;
	}
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FName& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{	
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Success:
		Success(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Warning:
		Warning(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Error:
		Error(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Fatal:
		UE_LOG(LogFatal, Fatal, TEXT("%s"), *Message.ToString())
	break;

	default:
	break;
	}
#endif
}

void ULog::DebugMessage_WithCondition(const EDebugLogType LogSeverity, const bool bCondition, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		DebugMessage(LogSeverity, Message, LoggingOption, bAddPrefix, TimeToDisplay);
#endif
}

void ULog::Crash(const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogCrash, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
#endif
}

void ULog::Fatal(const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
#endif
}

void ULog::Fatal_WithCondition(const FString& Message, const bool bCondition)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Fatal(Message);
#endif
}

void ULog::Error(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = FString("Error: ") + Message;
	else
		NewMessage = Message;

	LogMessage_Internal(NewMessage, "", "", Settings->ErrorColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Error_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay);
#endif
}

void ULog::Success(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = FString("Success: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, "", "", Settings->SuccessColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Success_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay);
#endif
}

void ULog::Warning(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = FString("Warning: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, "", "", Settings->WarningColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Warning_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay);
#endif
}

void ULog::Info(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString NewMessage;

	if (bAddPrefix)
		NewMessage = FString("Info: ") + Message;
	else
		NewMessage = Message;
	
	LogMessage_Internal(NewMessage, "", "", Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Info_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay);
#endif
}

void ULog::Hello(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Hello", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Hey(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Hey", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Bye(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Bye", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Goodbye(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Goodbye", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Cya(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Cya", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Wassup(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Wassup", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Yo(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yo", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Yes(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yes", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::Yes(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Yes", Prefix, Suffix, Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::No(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("No", "", "", Settings->InfoColor, LoggingOption, 5.0f);
#endif
}

void ULog::StartDebugTimer(const FString& Description)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Timer = new FDebugLogTimer();

	Timer->Start(Description);
#endif
}

void ULog::StopDebugTimer(const bool bAutoDetermineTimeUnit, const EDebugLogTimeUnit DisplayIn, const ELoggingOptions LoggingOption)
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
		// < 1000 microseconds
		if (Timer->GetDurationInMicroseconds() < 1000)
			Time(Timer->GetDurationInMicroseconds(), DLTU_Microseconds, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption);
		// > 1000 microseconds AND < 1000000 microseconds
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

		Time(Duration, DisplayIn, false, Timer->GetDescription() + " | Operation took: ", "", LoggingOption);
	}

	delete Timer;
	Timer = nullptr;
#endif
}

void ULog::No(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("No", Prefix, Suffix, Settings->ErrorColor, LoggingOption, 5.0f);
#endif
}

void ULog::Valid(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Valid", "", "", Settings->SuccessColor, LoggingOption, 5.0f);
#endif
}

void ULog::Valid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Valid", Prefix, Suffix, Settings->SuccessColor, LoggingOption, 5.0f);
#endif
}

void ULog::Invalid(const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Invalid", "", "", Settings->ErrorColor, LoggingOption, 5.0f);
#endif
}

void ULog::Invalid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal("Invalid", Prefix, Suffix, Settings->ErrorColor, LoggingOption, 5.0f);
#endif
}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
void ULog::Number(const int8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}
#endif

void ULog::Percent(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(FString::SanitizeFloat(Number) + "%", Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Percent(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Percent(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Bool(const bool bBoolToTest, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bBoolToTest)
		LogMessage_Internal("True", Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
	else
		LogMessage_Internal("False", Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Bool(const bool bBoolToTest, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Bool(bBoolToTest, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Vector(const FVector& InVector, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InVector.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Vector(const FVector& InVector, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Vector(InVector, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Vector2D(const FVector2D& InVector2D, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InVector2D.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Vector2D(const FVector2D& InVector2D, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Vector2D(InVector2D, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Rotator(const FRotator& InRotator, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InRotator.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Rotator(const FRotator& InRotator, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Rotator(InRotator, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Transform(const FTransform& InTransform, const FString& Prefix, const bool bFormat, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InTransform.ToString());
		}
	}
	else if (LoggingOption == LO_Console)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE_PREFIX, *Prefix, *InTransform.ToString())
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE_PREFIX, *Prefix, *InTransform.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InTransform.ToString());
		}
	}
#endif
}

void ULog::Transform(const FTransform& InTransform, const ELoggingOptions LoggingOption, const bool bFormat, const float TimeToDisplay)
{
	Transform(InTransform, "", bFormat, LoggingOption, TimeToDisplay);
}

void ULog::Quat(const FQuat& Quaternion, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(Quaternion.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Quat(const FQuat& Quaternion, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Quat(Quaternion, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Matrix(const FMatrix& InMatrix, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InMatrix.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Matrix(const FMatrix& InMatrix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Matrix(InMatrix, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Color(const FLinearColor& InColor, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InColor.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Color(const FLinearColor& InColor, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Color(InColor, "", "", LoggingOption, TimeToDisplay);	
}

void ULog::DateTime(const FDateTime& InDateTime, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(InDateTime.ToString(), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::DateTime(const FDateTime& InDateTime, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	DateTime(InDateTime, "", "", LoggingOption, TimeToDisplay);
}

void ULog::MessageInWorld(const FString& Message, const FVector& WorldLocation, const float FontScale, const FString& Prefix, const FString& Suffix, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (GWorld)
		DrawDebugString(GWorld, WorldLocation, Prefix + Message + Suffix, nullptr, FColor::White, TimeToDisplay, true, FontScale);
#endif
}

void ULog::MessageInWorld(const FString& Message, const FVector& WorldLocation, const float FontScale, const float TimeToDisplay)
{
	MessageInWorld(Message, WorldLocation, FontScale, "", "", TimeToDisplay);
}

void ULog::Temperature(const float InTemperatureValue, const EDebugLogTemperatureUnit TemperatureUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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

	LogUnitSystem(InTemperatureValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Temperature(const float InTemperatureValue, const EDebugLogTemperatureUnit TemperatureUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Temperature(InTemperatureValue, TemperatureUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Volume(const float InVolumeValue, const EDebugLogVolumeUnit VolumeUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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
	
	LogUnitSystem(InVolumeValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Volume(const float InVolumeValue, const EDebugLogVolumeUnit VolumeUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Volume(InVolumeValue, VolumeUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Data(const float InDataValue, const EDebugLogDataUnit DataUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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
	
	LogUnitSystem(InDataValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Data(const float InDataValue, const EDebugLogDataUnit DataUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Data(InDataValue, DataUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Length(const float InLengthValue, const EDebugLogLengthUnit LengthUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString UnitSymbol = "";
	switch (LengthUnit)
	{
	case DLLU_Centimeter:
		UnitSymbol = "cm";
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
	}

	LogUnitSystem(InLengthValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Length(const float InLengthValue, const EDebugLogLengthUnit LengthUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Length(InLengthValue, LengthUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Mass(const float InMassValue, const EDebugLogMassUnit MassUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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

	LogUnitSystem(InMassValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Mass(const float InMassValue, const EDebugLogMassUnit MassUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Mass(InMassValue, MassUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Speed(const float InSpeedValue, const EDebugLogSpeedUnit SpeedUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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

	LogUnitSystem(InSpeedValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Speed(const float InSpeedValue, const EDebugLogSpeedUnit SpeedUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Speed(InSpeedValue, SpeedUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Time(const float InTimeValue, const EDebugLogTimeUnit TimeUnit, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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
	
	LogUnitSystem(InTimeValue, UnitSymbol, bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Time(const float InTimeValue, const EDebugLogTimeUnit TimeUnit, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Time(InTimeValue, TimeUnit, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Dollar(const float InDollarValue, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogCurrencyUnitSystem(InDollarValue, "$", bConvertValueToInt, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Dollar(const float InDollarValue, const bool bConvertValueToInt, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Dollar(InDollarValue, bConvertValueToInt, "", "", LoggingOption, TimeToDisplay);
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
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Sphere.W));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Sphere.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
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
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Max: ") + Box.Max.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Min: ") + Box.Min.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
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
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogColor, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogColor, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogColor, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogColor, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Orientation: ") + Capsule.Orientation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Length: ") + FString::SanitizeFloat(Capsule.Length));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Capsule.Radius));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Capsule.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
#endif
}

bool ULog::AssertEqual_Bool(const bool bActual, const bool bExpected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<bool>(bActual, bExpected, "AssertEqual (Bool)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Integer(const int32 Actual, const int32 Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<int32>(Actual, Expected, "AssertEqual (Integer)", Message, bCrashOnFailure);
}

bool ULog::AssertValue_Integer(const int32 Actual, const int32 Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure)
{
	return AssertValue<int32>(Actual, Expected, ShouldBe, "AssertValue (Integer)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Float(const float Actual, const float Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<float>(Actual, Expected, "AssertEqual (Float)", Message, bCrashOnFailure);
}

bool ULog::AssertValue_Float(const float Actual, const float Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure)
{
	return AssertValue<float>(Actual, Expected, ShouldBe, "AssertValue (Float)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_String(const FString Actual, const FString Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FString>(Actual, Expected, "AssertEqual (String)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_String(const FString Actual, const FString Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FString>(Actual, Expected, "AssertNotEqual (String)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Name(const FName Actual, const FName Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FName>(Actual, Expected, "AssertEqual (Name)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Name(const FName Actual, const FName Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FName>(Actual, Expected, "AssertNotEqual (Name)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Object(UObject* Actual, UObject* Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<UObject>(Actual, Expected, "AssertEqual (Object)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Object(UObject* Actual, UObject* Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<UObject>(Actual, Expected, "AssertNotEqual (Object)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Vector(const FVector Actual, const FVector Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FVector>(Actual, Expected, "AssertEqual (Vector)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Vector2D(const FVector2D Actual, const FVector2D Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FVector2D>(Actual, Expected, "AssertEqual (Vector2D)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Vector(const FVector Actual, const FVector Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FVector>(Actual, Expected, "AssertNotEqual (Vector)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Vector2D(const FVector2D Actual, const FVector2D Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FVector2D>(Actual, Expected, "AssertNotEqual (Vector2D)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Rotator(const FRotator Actual, const FRotator Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FRotator>(Actual, Expected, "AssertEqual (Rotator)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Rotator(const FRotator Actual, const FRotator Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FRotator>(Actual, Expected, "AssertNotEqual (Rotator)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Quat(const FQuat Actual, const FQuat Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FQuat>(Actual, Expected, "AssertEqual (Quat)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Quat(const FQuat Actual, const FQuat Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FQuat>(Actual, Expected, "AssertNotEqual (Quat)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_Transform(const FTransform Actual, const FTransform Expected, const FString Message, const bool bNoScale, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bNoScale && !Actual.EqualsNoScale(Expected))
	{
		AssertFailed(Message, bCrashOnFailure);

		return false;
	}

	if (!Actual.Equals(Expected))
	{
		AssertFailed(Message, bCrashOnFailure);

		return false;
	}

	return true;
#endif

	return false;
}

bool ULog::AssertNotEqual_Transform(const FTransform Actual, const FTransform Expected, const FString Message, const bool bNoScale, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bNoScale && Actual.EqualsNoScale(Expected))
	{
		AssertFailed(Message, bCrashOnFailure);

		return false;
	}

	if (Actual.Equals(Expected))
	{
		AssertFailed(Message, bCrashOnFailure);

		return false;
	}

	return true;
#endif

	return false;
}

bool ULog::AssertEqual_Color(const FColor Actual, const FColor Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FColor>(Actual, Expected, "AssertEqual (Color)", Message, bCrashOnFailure);
}

bool ULog::AssertNotEqual_Color(const FColor Actual, const FColor Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertNotEqual<FColor>(Actual, Expected, "AssertNotEqual (Color)", Message, bCrashOnFailure);
}

bool ULog::AssertEqual_DateTime(const FDateTime Actual, const FDateTime Expected, const FString Message, const bool bCrashOnFailure)
{
	return AssertEqual<FDateTime>(Actual, Expected, "AssertEqual (DateTime)", Message, bCrashOnFailure);
}

bool ULog::AssertValue_DateTime(const FDateTime Actual, const FDateTime Expected, const EDebugLogComparisonMethod ShouldBe, const FString Message, const bool bCrashOnFailure)
{
	return AssertValue<FDateTime>(Actual, Expected, ShouldBe, "AssertValue (DateTime)", Message, bCrashOnFailure);
}

bool ULog::Assert_True(const bool bCondition, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!bCondition)
	{
		if (bCrashOnFailure)
			Crash("AssertTrue | Assert Failed: " + Message);
		else
			Error("AssertTrue | Assert Failed: " + Message, LO_Both, false);

		return false;
	}

	return true;
#endif

	return false;
}

bool ULog::Assert_False(const bool bCondition, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bCondition)
	{
		if (bCrashOnFailure)
			Crash("AssertFalse | Assert Failed: " + Message);
		else
			Error("AssertFalse | Assert Failed: " + Message, LO_Both, false);

		return false;
	}

	return true;
#endif

	return false;
}

bool ULog::Assert_IsValid(UObject* Object, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!IsValid(Object))
	{
		if (bCrashOnFailure)
			Crash("AssertIsValid | Assert Failed: " + Message);
		else
			Error("AssertIsValid | Assert Failed: " + Message, LO_Both, false);

		return false;		
	}

	return true;
#endif

	return false;
}

void ULog::Number_Int_Blueprint(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
#endif
}

void ULog::Number_Float_Blueprint(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
#endif
}

void ULog::CheckObject(UObject* Object, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!Object)
		ASSERT(Object, Message);
#endif
}

void ULog::CheckCondition(const bool bCondition, const FString& Message)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!bCondition)
		ASSERT(bCondition, Message);
#endif
}

void ULog::CheckNoEntry()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoEntry();
#endif
}

void ULog::CheckNoReEntry()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoReentry();
#endif
}

void ULog::CheckNoRecursion()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	checkNoRecursion();
#endif
}

void ULog::UnImplemented()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	unimplemented();
#endif
}

void ULog::LogMessage_Internal(const FString& Message, const FString& Prefix, const FString& Suffix, const FColor& LogColor, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, LogColor, NET_MODE_PREFIX + Prefix + Message + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *Message, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *Message, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, LogColor, NET_MODE_PREFIX + Prefix + Message + Suffix);
	}
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingConfiguration)
		Crash("DebugLogLibrary plugin does not work in a Shipping build. Disable the plugin, remove all ULog:: calls or wrap them with a #if guard!");
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
#endif
}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
void ULog::LogInt(const platform_int Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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
	
	LogMessage_Internal(FinalNumber, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::LogUInt(const platform_uint Number, const FString& Prefix, const FString& Suffix, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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

	LogMessage_Internal(FinalNumber, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::LogFloat(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	LogMessage_Internal(FString::SanitizeFloat(Number), Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::LogUnitSystem(const float Value, const FString& UnitSymbol, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString ValueInString = FString::SanitizeFloat(Value);

	if (bConvertValueToInt)
		ValueInString = FString::FromInt(Value);

	LogMessage_Internal(ValueInString + UnitSymbol, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::LogCurrencyUnitSystem(const float Value, const FString& UnitSymbol, const bool bConvertValueToInt, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FString ValueInString = FString::SanitizeFloat(Value);

	if (bConvertValueToInt)
		ValueInString = FString::FromInt(Value);

	LogMessage_Internal(UnitSymbol + ValueInString, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
#endif
}

void ULog::LogLongInt(const long Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
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

	LogMessage_Internal(FinalNumber, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay);
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
}

void ULog::AssertFailed(const FString& Message, const bool bCrashOnFailure)
{
	if (bCrashOnFailure)
		Crash("Assert Failed: " + Message);
	else
		Error("Assert Failed: " + Message, LO_Both, false);
}
#endif
