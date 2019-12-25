// Copyright Ali El Saleh 2019

#include "Log.h"
#include "Engine/Engine.h"
#include "Debug.h"
#include "Math/Vector.h"

void ULog::ObjectValidity(UObject* ObjectRef, const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		if (ObjectRef)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, ObjectRef->GetName() + " is valid");
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Object is null");
	}
	else if (LoggingOption == LO_Console)
	{
		if (ObjectRef)
			UE_LOG(LogObjectValidity, Warning, TEXT("%s is valid"), *ObjectRef->GetName())
		else
			UE_LOG(LogObjectValidity, Error, TEXT("Object is null"))
	}
	else if (LoggingOption == LO_Both)
	{
		if (ObjectRef)
		{
			UE_LOG(LogObjectValidity, Warning, TEXT("%s is valid"), *ObjectRef->GetName())
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, ObjectRef->GetName() + " is valid");
		}
		else
		{
			UE_LOG(LogObjectValidity, Error, TEXT("Object is null"))	
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Object is null");
		}
	}
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FString& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	switch (LogSeverity)
	{
	case DL_Info:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
		}
	break;

	case DL_Success:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
		}
	break;

	case DL_Warning:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
		}
	break;

	case DL_Error:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogError, Error, TEXT("%s"), *Message)
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogError, Error, TEXT("%s"), *Message)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
		}
	break;

	case DL_Fatal:
		UE_LOG(LogFatal, Fatal, TEXT("%s"), *Message)
	break;

	default:
		break;
	}
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FName& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	switch (LogSeverity)
	{
	case DL_Info:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message.ToString());
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message.ToString())
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message.ToString());
		}
	break;

	case DL_Success:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message.ToString());
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message.ToString())
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message.ToString());
		}
	break;

	case DL_Warning:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message.ToString());
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogWarning, Warning, TEXT("%s"), *Message.ToString())
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogWarning, Warning, TEXT("%s"), *Message.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message.ToString());
		}
	break;

	case DL_Error:
		if (LoggingOption == LO_Viewport)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message.ToString());
		}
		else if (LoggingOption == LO_Console)
		{
			UE_LOG(LogError, Error, TEXT("%s"), *Message.ToString())
		}
		else if (LoggingOption == LO_Both)
		{
			UE_LOG(LogError, Error, TEXT("%s"), *Message.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message.ToString());
		}
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
	UE_LOG(LogFatal, Fatal, TEXT("%s"), *Message)
}

void ULog::Error(const FString& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
	}
}

void ULog::Success(const FString& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
	}
}

void ULog::Warning(const FString& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
	}
}

void ULog::Info(const FString& Message, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
	}
}

void ULog::Hello(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Hello"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("Hello"))
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("Hello"))
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Hello"));
	}
}

void ULog::Yes(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Yes"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("Yes"))
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("Yes"))
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Yes"));
	}
}

void ULog::Yes(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = Prefix + "Yes" + Suffix;

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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("No"))
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("No"))
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	}
}

void ULog::No(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = Prefix + ": No" + Suffix;

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
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Prefix + FString("Valid") + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid%s"), *Prefix, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid%s"), *Prefix, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Prefix + FString("Valid") + Suffix);
	}
}

void ULog::Invalid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Prefix + FString("Invalid") + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid%s"), *Prefix, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid%s"), *Prefix, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Prefix + FString("Invalid") + Suffix);
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
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%f%s"), *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%f%s"), *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
}

void ULog::Number(const long Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%d%s"), *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%d%s"), *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::Percent(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	const FString NewMessage = Prefix + FString::SanitizeFloat(Number) + "%" + Suffix;

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

void ULog::Bool(const bool bBoolToTest, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bBoolToTest)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + "True" + Suffix);
		else
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + "False" + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		if (bBoolToTest)
			UE_LOG(LogBool, Warning, TEXT("%sTrue%s"), *Prefix, *Suffix)
		else
			UE_LOG(LogBool, Warning, TEXT("%sFalse%s"), *Prefix, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		if (bBoolToTest)
		{
			UE_LOG(LogBool, Warning, TEXT("%sTrue%s"), *Prefix, *Suffix)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + "True" + Suffix);
		}
		else
		{
			UE_LOG(LogBool, Warning, TEXT("%sFalse%s"), *Prefix, *Suffix)	
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + "False" + Suffix);
		}
	}
}

void ULog::Vector(const FVector& Vector, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Vector.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s"), *Prefix, *Vector.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s"), *Prefix, *Vector.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Vector.ToString() + Suffix);
	}
}

void ULog::Rotator(const FRotator& Rotator, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Rotator.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s"), *Prefix, *Rotator.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s"), *Prefix, *Rotator.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Rotator.ToString() + Suffix);
	}
}

void ULog::Transform(const FTransform& Transform, const FString& Prefix, const bool bFormat, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bFormat)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Scale: " + Transform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Rotation: " + Transform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Location: " + Transform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Transform.ToString());
		}
	}
	else if (LoggingOption == LO_Console)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s"), *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("Location: %s"), *Transform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("Rotation: %s"), *Transform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("Scale: %s"), *Transform.GetScale3D().ToString())
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), *Prefix, *Transform.ToString())
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s"), *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("Location: %s"), *Transform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("Rotation: %s"), *Transform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("Scale: %s"), *Transform.GetScale3D().ToString())

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Scale: " + Transform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Rotation: " + Transform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "Location: " + Transform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix);
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), *Prefix, *Transform.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Transform.ToString());
		}
	}
}

void ULog::Quat(const FQuat& Quaternion, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Quaternion.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s"), *Prefix, *Quaternion.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s"), *Prefix, *Quaternion.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Quaternion.ToString() + Suffix);
	}
}

void ULog::Matrix(const FMatrix& Matrix, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Matrix.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s"), *Prefix, *Matrix.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s"), *Prefix, *Matrix.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Matrix.ToString() + Suffix);
	}
}

void ULog::Color(const FLinearColor& Color, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Color.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s"), *Prefix, *Color.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s"), *Prefix, *Color.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Color.ToString() + Suffix);
	}
}

void ULog::Number_Int_Blueprint(const int32 Number, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::Number_Float_Blueprint(const float Number, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, LoggingOption, TimeToDisplay);
}

void ULog::LogInt(const int64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%lld%s"), *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%lld%s"), *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::LogUInt(const uint64 Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%lld%s"), *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%lld%s"), *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::LogFloat(const float Number, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%f%s"), *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%f%s"), *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
}
