// Copyright Ali El Saleh 2019

#include "Log.h"
#include "Debug.h"
#include "Engine/Engine.h"
#include "Math/Vector.h"

void ULog::ObjectValidity(UObject* ObjectRef, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		if (ObjectRef)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, NET_MODE + ObjectRef->GetName() + " is valid");
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, NET_MODE + FString("Object is null"));
	}
	else if (LoggingOption == LO_Console)
	{
		if (ObjectRef)
			UE_LOG(LogObjectValidity, Warning, TEXT("%s%s is valid"), NET_MODE, *ObjectRef->GetName())
		else
			UE_LOG(LogObjectValidity, Error, TEXT("%sObject is null"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		if (ObjectRef)
		{
			UE_LOG(LogObjectValidity, Warning, TEXT("%s%s is valid"), NET_MODE, *ObjectRef->GetName())
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, NET_MODE + ObjectRef->GetName() + " is valid");
		}
		else
		{
			UE_LOG(LogObjectValidity, Error, TEXT("%sObject is null"), NET_MODE)	
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, NET_MODE + FString("Object is null"));
		}
	}
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
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
		UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE, *Message)
	break;

	default:
		break;
	}
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FName& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{	
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
}

void ULog::Fatal(const FString& Message)
{
	UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE, *Message)
}

void ULog::Error(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = NET_MODE + FString("Error: ") + Message;
	else
		NewMessage = NET_MODE + Message;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%s%s"), NET_MODE, *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%s%s"), NET_MODE, *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, NewMessage);
	}
}

void ULog::Success(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = NET_MODE + FString("Success: ") + Message;
	else
		NewMessage = NET_MODE + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, NewMessage);
	}
}

void ULog::Warning(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = NET_MODE + FString("Warning: ") + Message;
	else
		NewMessage = NET_MODE + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s%s"), NET_MODE, *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s%s"), NET_MODE, *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, NewMessage);
	}
}

void ULog::Info(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = NET_MODE + FString("Info: ") + Message;
	else
		NewMessage = NET_MODE + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NewMessage);
	}
}

void ULog::Hello(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("Hello"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sHello"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sHello"), NET_MODE)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("Hello"));
	}
}

void ULog::Yes(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("Yes"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sYes"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sYes"), NET_MODE)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("Yes"));
	}
}

void ULog::Yes(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE + Prefix + "Yes" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	}
}

void ULog::No(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("No"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%sNo"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%sNo"), NET_MODE)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, NET_MODE + FString("No"));
	}
}

void ULog::Valid(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, NET_MODE + FString("Valid"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid"), NET_MODE)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, NET_MODE + FString("Valid"));
	}
}

void ULog::Invalid(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NET_MODE + FString("Invalid"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid"), NET_MODE)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid"), NET_MODE)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NET_MODE + FString("Invalid"));
	}
}

void ULog::No(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE + Prefix + ": No" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	}
}

void ULog::Valid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE + Prefix + FString("Valid") + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
	}
}

void ULog::Invalid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE + Prefix + FString("Invalid") + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
	}
}

void ULog::Number(const int8 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int8 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Percent(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	const FString NewMessage = NET_MODE + Prefix + FString::SanitizeFloat(Number) + "%" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NewMessage);
	}
}

void ULog::Percent(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Percent(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Bool(const bool bBoolToTest, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bBoolToTest)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + "True" + Suffix);
		else
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + "False" + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		if (bBoolToTest)
			UE_LOG(LogBool, Warning, TEXT("%s%sTrue%s"), NET_MODE, *Prefix, *Suffix)
		else
			UE_LOG(LogBool, Warning, TEXT("%s%sFalse%s"), NET_MODE, *Prefix, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		if (bBoolToTest)
		{
			UE_LOG(LogBool, Warning, TEXT("%s%sTrue%s"), NET_MODE, *Prefix, *Suffix)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + "True" + Suffix);
		}
		else
		{
			UE_LOG(LogBool, Warning, TEXT("%s%sFalse%s"), NET_MODE, *Prefix, *Suffix)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + "False" + Suffix);
		}
	}
}

void ULog::Bool(const bool bBoolToTest, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Bool(bBoolToTest, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Vector(const FVector& InVector, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InVector.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InVector.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InVector.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InVector.ToString() + Suffix);
	}
}

void ULog::Vector(const FVector& InVector, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Vector(InVector, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Rotator(const FRotator& InRotator, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InRotator.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InRotator.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InRotator.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InRotator.ToString() + Suffix);
	}
}

void ULog::Rotator(const FRotator& InRotator, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Rotator(InRotator, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Transform(const FTransform& InTransform, const FString& Prefix, const bool bFormat, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bFormat)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InTransform.ToString());
		}
	}
	else if (LoggingOption == LO_Console)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE, *InTransform.GetScale3D().ToString())
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE, *Prefix, *InTransform.ToString())
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE, *InTransform.GetScale3D().ToString())

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix);
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE, *Prefix, *InTransform.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InTransform.ToString());
		}
	}
}

void ULog::Transform(const FTransform& InTransform, const ELoggingOptions LoggingOption, const bool bFormat, const float TimeToDisplay)
{
	Transform(InTransform, "", bFormat, LoggingOption, TimeToDisplay);
}

void ULog::Quat(const FQuat& Quaternion, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + Quaternion.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *Quaternion.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *Quaternion.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + Quaternion.ToString() + Suffix);
	}
}

void ULog::Quat(const FQuat& Quaternion, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Quat(Quaternion, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Matrix(const FMatrix& InMatrix, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InMatrix.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InMatrix.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InMatrix.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InMatrix.ToString() + Suffix);
	}
}

void ULog::Matrix(const FMatrix& InMatrix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Matrix(InMatrix, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Color(const FLinearColor& InColor, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InColor.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InColor.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s%s"), NET_MODE, *Prefix, *InColor.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + InColor.ToString() + Suffix);
	}
}

void ULog::Color(const FLinearColor& InColor, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Color(InColor, "", "", LoggingOption, TimeToDisplay);	
}

void ULog::Number_Int_Blueprint(const int32 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number_Float_Blueprint(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::LogInt(const int64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::LogUInt(const uint64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::LogFloat(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%f%s"), NET_MODE, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%f%s"), NET_MODE, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
}

void ULog::LogLongInt(const long Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%d%s"), NET_MODE, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%d%s"), NET_MODE, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, NET_MODE + Prefix + FString::FromInt(Number) + Suffix);
	}
}
