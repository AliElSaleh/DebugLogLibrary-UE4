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
#define NET_MODE_PREFIX *(!GWorld ? FString("") \
	: GWorld->GetNetMode() == NM_Client ? FString("[Client] ") + FString::FromInt(GPlayInEditorID - 1) + FString(": ") \
	: GWorld->GetNetMode() == NM_ListenServer ? FString("[Server]: ") \
	: GWorld->GetNetMode() == NM_DedicatedServer ? FString("[Dedicated Server]: ") \
	: FString(""))

// Development build only. Logs an assert message and crashes the program
#define ASSERT(expr, message) FDebug::AssertFailed(#expr, UE_LOG_SOURCE_FILE(__FILE__), __LINE__, TEXT("%s"), *message);
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

UENUM(BlueprintType)
enum EDebugLogNumberSystems
{
	DLNS_Decimal	UMETA(DisplayName = "Decimal (Base 10)"),
	DLNS_Binary		UMETA(DisplayName = "Binary (Base 2)"),
	DLNS_Hex		UMETA(DisplayName = "Hexadecimal (Base 16)"),
	DLNS_Octal		UMETA(DisplayName = "Octal (Base 8)"),
	DLNS_Roman		UMETA(DisplayName = "Roman Numerals (Base 10)")
};

/**
 * A library of log utility functions
 */
UCLASS(Abstract)
class DEBUG_API ULog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Log whether the object is valid or not
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void ObjectValidity(UObject* ObjectRef, ELoggingOptions LoggingOption = LO_Console);

	// Log a debug message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void DebugMessage(EDebugLogType LogSeverity, const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);
		static void DebugMessage(EDebugLogType LogSeverity, const FName& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);	

	// Log a debug message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Debug Message (Condition)", meta = (DevelopmentOnly))
		static void DebugMessage_WithCondition(EDebugLogType LogSeverity, bool bCondition, const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Crash the game, and log the message to the console and log file
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Crash(const FString& Message = "");
		
	// Log a fatal error message to the console and crash
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Fatal(const FString& Message);

	// Log a fatal error message to the console and crash, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Fatal (Condition)", meta = (DevelopmentOnly))
		static void Fatal_WithCondition(const FString& Message, bool bCondition);

	// Log an error message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Error(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log an error message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Error (Condition)", meta = (DevelopmentOnly))
		static void Error_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a success message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Success(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a success message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Success (Condition)", meta = (DevelopmentOnly))
		static void Success_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a warning message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Warning(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a warning message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Warning (Condition)", meta = (DevelopmentOnly))
		static void Warning_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log an information message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Info(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log an information message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Info (Condition)", meta = (DevelopmentOnly))
		static void Info_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f);

	// Log a hello message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Hello(ELoggingOptions LoggingOption = LO_Console);

	// Log a 'valid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Valid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);
	// Log a 'valid' message to the console or viewport
		static void Valid(ELoggingOptions LoggingOption = LO_Console);

	// Log an 'invalid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Invalid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);
	// Log an 'invalid' message to the console or viewport
		static void Invalid(ELoggingOptions LoggingOption = LO_Console);

	// Log a yes message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Yes(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);
	// Log a yes message to the console or viewport
		static void Yes(ELoggingOptions LoggingOption = LO_Console);

	// Log a no message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void No(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console);
	// Log a no message to the console or viewport
		static void No(ELoggingOptions LoggingOption = LO_Console);
	
	// Log a number to the console or viewport (int8 version)
	static void Number(int8 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version)
	static void Number(int16 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version)
	static void Number(int32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version)
	static void Number(int64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint8 version)
	static void Number(uint8 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint16 version)
	static void Number(uint16 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint32 version)
	static void Number(uint32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint64 version)
	static void Number(uint64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	static void Number(float Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version)
	static void Number(double Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long version)
	static void Number(long Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int8 version, no prefix and suffix)
	static void Number(int8 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version, no prefix and suffix)
	static void Number(int16 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version, no prefix and suffix)
	static void Number(int32 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version, no prefix and suffix)
	static void Number(int64 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint8 version, no prefix and suffix)
	static void Number(uint8 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint16 version, no prefix and suffix)
	static void Number(uint16 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint32 version, no prefix and suffix)
	static void Number(uint32 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (uint64 version, no prefix and suffix)
	static void Number(uint64 Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version, no prefix and suffix)
	static void Number(float Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version, no prefix and suffix)
	static void Number(double Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long version, no prefix and suffix)
	static void Number(long Number, EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	
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

	// Log sphere information to the console or viewport
	static void Sphere(const FSphere& Sphere, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log box information to the console or viewport
	static void Box(const FBox& Box, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log capsule information to the console or viewport
	static void Capsule(const FCapsuleShape& Capsule, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

protected:
	void PostInitProperties() override;

	// Log a number to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (int)", meta = (DevelopmentOnly))
		static void Number_Int_Blueprint(int32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (float)", meta = (DevelopmentOnly))
		static void Number_Float_Blueprint(float Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Verifies the object and if it fails generates a callstack leading to that point to the Output Log window. (With an optional message parameter)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Ensure (Object)", meta = (DevelopmentOnly))
		static void EnsureObject(UObject* Object, bool bAlwaysEnsure = false, const FString& Message = "");

	// Verifies the expression and if it fails generates a callstack leading to that point to the Output Log window. (With an optional message parameter)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Ensure (Condition)", meta = (DevelopmentOnly))
		static void EnsureCondition(bool bCondition, bool bAlwaysEnsure = false, const FString& Message = "");

	// This function executes the expression and, if it results in a false assertion, halts execution. 
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Check (Object)", meta = (DevelopmentOnly))
		static void CheckObject(UObject* Object, const FString& Message);

	// This function executes the expression and, if it results in a false assertion, halts execution. 
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Check (Condition)", meta = (DevelopmentOnly))
		static void CheckCondition(bool bCondition, const FString& Message);

	// This function doesn't take an expression and is used to mark code paths that should never execute.
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void CheckNoEntry();

	// This function is used to prevent calls from being reentrant to a given function. Use it for functions that should only be called once and must be completed before being called again.
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Check No Re-Entry", meta = (DevelopmentOnly))
		static void CheckNoReEntry();

	// This function is used to prevent code that should never be called recursively.
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void CheckNoRecursion();

	// This function is used to mark a function that should be overridden because the base function contains no implementation. Alternatively, this can be used anywhere where a function has not been implemented yet.
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Un-Implemented", meta = (DevelopmentOnly))
		static void UnImplemented();

private:
	static void LogInt(int64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogUInt(uint64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogLongInt(long Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogFloat(float Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	static FString DecimalToHex(int64 DecimalNumber);
	static FString DecimalToBinary(int64 DecimalNumber);
	static FString DecimalToOctal(int64 DecimalNumber);
	static FString DecimalToRomanNumeral(int64 DecimalNumber);

	static const class UDebugLogLibrarySettings* Settings;
};
