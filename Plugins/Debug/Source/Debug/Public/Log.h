// Copyright Ali El Saleh 2019

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Log.generated.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
// Development build only. Get the current class name and function name where this is called
#define CUR_CLASS_FUNC (FString(__FUNCTION__))

// Development build only. Get the current class name, function name and line number where this is called
#define CUR_CLASS_FUNC_WITH_LINE (CUR_CLASS_FUNC + ": " + CUR_LINE)

// Development build only. Get the current class where this is called
#define CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

// Development build only. Get the current function name where this is called
#define CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
  
// Development build only. Get the current line number in the code where this is called
#define CUR_LINE ("Line " + FString::FromInt(__LINE__))

// Development build only. Get the current function name and the line number in the code where this is called
#define CUR_FUNC_WITH_LINE  (CUR_FUNC + ": " + CUR_LINE)

// Development build only. Get the current class and line number where this is called
#define CUR_CLASS_WITH_LINE (CUR_CLASS + "(" + CUR_LINE + ")")
  
// Development build only. Get the current function signature where this is called
#define CUR_FUNC_SIG (FString(__FUNCSIG__))

// Development build only. Get the current function signature with the number where this is called
#define CUR_FUNC_SIG_WITH_LINE (FString(__FUNCSIG__) + ": " + CUR_LINE)

// Development build only. Returns the net mode as a string, possible outputs are, Empty String, [Client], [Server] or [Dedicated Server]
#define NET_MODE *(!GWorld ? FString("") \
	: GWorld->GetNetMode() == NM_Client ? FString("[Client] ") + FString::FromInt(GPlayInEditorID - 1) + FString(": ") \
	: GWorld->GetNetMode() == NM_ListenServer ? FString("[Server]: ") \
	: GWorld->GetNetMode() == NM_DedicatedServer ? FString("[Dedicated Server]: ") \
	: FString(""))
#endif

UENUM(BlueprintType)
enum EDebugLogType
{
	DL_Info		UMETA(DisplayName = "Info"),
	DL_Success	UMETA(DisplayName = "Success"),
	DL_Warning	UMETA(DisplayName = "Warning"),
	DL_Error	UMETA(DisplayName = "Error"),
	DL_Fatal	UMETA(DisplayName = "Fatal")
};

UENUM(BlueprintType)
enum ELoggingOptions
{
	LO_Viewport UMETA(DisplayName = "Viewport"),
	LO_Console	UMETA(DisplayName = "Console"),
	LO_Both		UMETA(DisplayName = "Viewport and Console"),
	LO_NoLog	UMETA(DisplayName = "Disabled")
};

/**
 * A library of log utility functions
 */
UCLASS(Abstract)
class DEBUG_API ULog final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Log whether the object is valid or not
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void ObjectValidity(UObject* ObjectRef, ELoggingOptions LoggingOption = LO_Console);

	// Log a debug message to the console or viewport (FString version)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void DebugMessage(EDebugLogType LogSeverity, const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a debug message to the console or viewport (FName version)
	static void DebugMessage(EDebugLogType LogSeverity, const FName& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);	

	// Log a fatal error message to the console and crash
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Fatal(const FString& Message);

	// Log an error message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Error(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a success message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Success(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a warning message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Warning(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log an information message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Info(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a hello message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Hello(ELoggingOptions LoggingOption = LO_Console);
	
	// Log a yes message to the console or viewport
	static void Yes(ELoggingOptions LoggingOption = LO_Console);

	// Log a no message to the console or viewport
	static void No(ELoggingOptions LoggingOption = LO_Console);

	// Log a 'valid' message to the console or viewport
	static void Valid(ELoggingOptions LoggingOption = LO_Console);

	// Log an 'invalid' message to the console or viewport
	static void Invalid(ELoggingOptions LoggingOption = LO_Console);

	// Log a 'valid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Valid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);

	// Log an 'invalid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Invalid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);

	// Log a yes message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Yes(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);

	// Log a no message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void No(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);

	// Log a number to the console or viewport (int8 version)
	static void Number(int8 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version)
	static void Number(int16 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version)
	static void Number(int32 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version)
	static void Number(int64 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint8 version)
	static void Number(uint8 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint16 version)
	static void Number(uint16 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint32 version)
	static void Number(uint32 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint64 version)
	static void Number(uint64 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	static void Number(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version)
	static void Number(double Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long version)
	static void Number(long Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int8 version, no prefix and suffix)
	static void Number(int8 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version, no prefix and suffix)
	static void Number(int16 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version, no prefix and suffix)
	static void Number(int32 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version, no prefix and suffix)
	static void Number(int64 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint8 version, no prefix and suffix)
	static void Number(uint8 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint16 version, no prefix and suffix)
	static void Number(uint16 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint32 version, no prefix and suffix)
	static void Number(uint32 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint64 version, no prefix and suffix)
	static void Number(uint64 Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version, no prefix and suffix)
	static void Number(float Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version, no prefix and suffix)
	static void Number(double Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long version, no prefix and suffix)
	static void Number(long Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	
	// Log the a percentage value to the console or viewport (Just appends a % symbol)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Percent(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Percent(float Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log the given bool value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Bool(bool bBoolToTest, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Bool(bool bBoolToTest, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a FVector value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Vector(const FVector& InVector, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Vector(const FVector& InVector, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a FRotator value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Rotator(const FRotator& InRotator, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Rotator(const FRotator& InRotator, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a FTransform value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Transform(const FTransform& InTransform, const FString& Prefix = "", bool bFormat = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Transform(const FTransform& InTransform, ELoggingOptions LoggingOption = LO_Console, bool bFormat = false, float TimeToDisplay = 5.0f);

	// Log a Quaternion value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Quat(const FQuat& Quaternion, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Quat(const FQuat& Quaternion, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a matrix value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Matrix(const FMatrix& InMatrix, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Matrix(const FMatrix& InMatrix, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a color value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Color(const FLinearColor& InColor, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Color(const FLinearColor& InColor, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

protected:
	// Log a number to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (int)", meta = (DevelopmentOnly))
		static void Number_Int_Blueprint(int32 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (float)")
		static void Number_Float_Blueprint(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

private:
	static void LogInt(int64 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogUInt(uint64 Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogLongInt(long Number, const FString& Prefix, const FString& Suffix, ELoggingOptions LoggingOption, float TimeToDisplay);
	static void LogFloat(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
};
