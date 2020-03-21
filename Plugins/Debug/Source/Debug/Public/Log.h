// Copyright Ali El Saleh 2020

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

// Development build only. Quick debug logging macros
#define QUICK_LOG_VECTOR(Variable) ULog::Vector(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_VECTOR(Variable) ULog::Vector(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_VECTOR(Variable) ULog::Vector(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_VECTOR2D(Variable) ULog::Vector2D(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_VECTOR2D(Variable) ULog::Vector2D(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_VECTOR2D(Variable) ULog::Vector2D(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_ROTATOR(Variable) ULog::Rotator(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_ROTATOR(Variable) ULog::Rotator(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_ROTATOR(Variable) ULog::Rotator(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_TRANSFORM(Variable) ULog::Transform(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_TRANSFORM(Variable) ULog::Transform(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_TRANSFORM(Variable) ULog::Transform(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_QUAT(Variable) ULog::Quat(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_QUAT(Variable) ULog::Quat(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_QUAT(Variable) ULog::Quat(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_MATRIX(Variable) ULog::Matrix(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_MATRIX(Variable) ULog::Matrix(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_MATRIX(Variable) ULog::Matrix(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_COLOR(Variable) ULog::Color(Variable, "", "", LO_Both, 5.0f)
#define QUICK_LOG_CONSOLE_COLOR(Variable) ULog::Color(Variable, "", "", LO_Console, 5.0f)
#define QUICK_LOG_VIEWPORT_COLOR(Variable) ULog::Color(Variable, "", "", LO_Viewport, 5.0f)

#define QUICK_LOG_TRUE() ULog::Warning("True", LO_Viewport, false, 5.0f)
#define QUICK_LOG_CONSOLE_TRUE() ULog::Warning("True", LO_Viewport, false, 5.0f)
#define QUICK_LOG_VIEWPORT_TRUE() ULog::Warning("True", LO_Viewport, false, 5.0f)
#endif

#if PLATFORM_64BITS
	typedef int64 platform_int;
	typedef uint64 platform_uint;
#else
	typedef int32 platform_int;
	typedef uint32 platform_uint;
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

UENUM(BlueprintType)
enum EDebugLogTemperatureUnit
{
	DLTU_Celsius		UMETA(DisplayName = "Celsius (C)"),
	DLTU_Fahrenheit		UMETA(DisplayName = "Fahrenheit (F)"),
	DLTU_Kelvin			UMETA(DisplayName = "Kelvin (K)")
};

UENUM(BlueprintType)
enum EDebugLogVolumeUnit
{
	DLVU_Litre			UMETA(DisplayName = "Litre (L)"),
	DLVU_Millilitre		UMETA(DisplayName = "Millilitre (ml)"),
	DLVU_Gallon			UMETA(DisplayName = "Gallon (GAL)"),
	DLVU_Pint			UMETA(DisplayName = "Pint (pt)"),
	DLVU_Quart			UMETA(DisplayName = "Quart (qt)")
};

UENUM(BlueprintType)
enum EDebugLogDataUnit
{
	DLDU_Bit		UMETA(DisplayName = "Bit (bit)"),
	DLDU_Byte		UMETA(DisplayName = "Byte (byte)"),
	DLDU_Kilobyte	UMETA(DisplayName = "Kilobyte (KB)"),
	DLDU_Megabyte	UMETA(DisplayName = "Megabyte (MB)"),
	DLDU_Gigabyte	UMETA(DisplayName = "Gigabyte (GB)"),
	DLDU_Terabyte	UMETA(DisplayName = "Terabyte (TB)"),
	DLDU_Petabyte	UMETA(DisplayName = "Petabyte (PB)")
};

UENUM(BlueprintType)
enum EDebugLogLengthUnit
{
	DLLU_Centimeter		UMETA(DisplayName = "Centimeter (cm)"),
	DLLU_Millimeter		UMETA(DisplayName = "Millimeter (mm)"),
	DLLU_Meter			UMETA(DisplayName = "Meter (m)"),
	DLLU_Kilometer		UMETA(DisplayName = "Kilometer (km)"),
	DLLU_Inch			UMETA(DisplayName = "Inches (in)"),
	DLLU_Feet			UMETA(DisplayName = "Feet (ft)"),
	DLLU_Mile			UMETA(DisplayName = "Miles (mi)"),
	DLLU_NauticalMile	UMETA(DisplayName = "Nautical Mile (nm)"),
	DLLU_Yard			UMETA(DisplayName = "Yard (yd)")
};

UENUM(BlueprintType)
enum EDebugLogMassUnit
{
	DLMU_Microgram		UMETA(DisplayName = "Microgram (μg)"),
	DLMU_Milligram		UMETA(DisplayName = "Milligram (mg)"),
	DLMU_Gram			UMETA(DisplayName = "Gram (g)"),
	DLMU_Kilogram		UMETA(DisplayName = "Kilogram (kg)"),
	DLMU_Tonne			UMETA(DisplayName = "Tonne (t)"),
	DLMU_Ounce			UMETA(DisplayName = "Ounce (oz)"),
	DLMU_Pound			UMETA(DisplayName = "Pound (lb)"),
	DLMU_Stone			UMETA(DisplayName = "Stone (st)")
};

UENUM(BlueprintType)
enum EDebugLogSpeedUnit
{
	DLSU_CentimetersPerSec		UMETA(DisplayName = "Centimeters per Sec (cm/s)"),
	DLSU_CentimetersPerHour		UMETA(DisplayName = "Centimeters per Hour (cm/h))"),
	DLSU_MillimetersPerSec		UMETA(DisplayName = "Millimeters per Sec (mm/s)"),
	DLSU_MillimetersPerHour		UMETA(DisplayName = "Millimeters per Hour (mm/h)"),
	DLSU_MetersPerSec			UMETA(DisplayName = "Meters per Sec (m/s)"),
	DLSU_MetersPerHour			UMETA(DisplayName = "Meters per Hour (m/h)"),
	DLSU_KilometersPerSec		UMETA(DisplayName = "Kilometers per Sec (km/s)"),
	DLSU_KilometersPerHour		UMETA(DisplayName = "Kilometers per Hour (km/h)"),
	DLSU_InchesPerSec			UMETA(DisplayName = "Inches per Sec (in/s)"),
	DLSU_InchesPerHour			UMETA(DisplayName = "Inches per Hour (in/h)"),
	DLSU_FeetPerSec				UMETA(DisplayName = "Feet per Sec (ft/s)"),
	DLSU_FeetPerHour			UMETA(DisplayName = "Feet per Hour (ft/h)"),
	DLSU_MilesPerSec			UMETA(DisplayName = "Miles per Sec (mi/s)"),
	DLSU_MilesPerHour			UMETA(DisplayName = "Miles per Hour (mi/h)"),
	DLSU_YardsPerSec			UMETA(DisplayName = "Yards per Sec (yd/s)"),
	DLSU_YardsPerHour			UMETA(DisplayName = "Yards per Hour (yd/h)"),
	DLSU_Knots					UMETA(DisplayName = "Knots (kn)")
};

UENUM(BlueprintType)
enum EDebugLogTimeUnit
{
	DLTU_Nanoseconds			UMETA(DisplayName = "Nanoseconds (ns)"),
	DLTU_Microseconds			UMETA(DisplayName = "Microseconds (us)"),
	DLTU_Milliseconds			UMETA(DisplayName = "Milliseconds (ms)"),
	DLTU_Seconds				UMETA(DisplayName = "Seconds (sec)"),
	DLTU_Minutes				UMETA(DisplayName = "Minutes (min)"),
	DLTU_Hours					UMETA(DisplayName = "Hours (hr)"),
	DLTU_Days					UMETA(DisplayName = "Days (day)"),
	DLTU_Weeks					UMETA(DisplayName = "Weeks (wk)"),
	DLTU_Months					UMETA(DisplayName = "Months (mth)"),
	DLTU_Years					UMETA(DisplayName = "Years (yrs)"),
	DLTU_Decades				UMETA(DisplayName = "Decades (decade)"),
	DLTU_Centuries				UMETA(DisplayName = "Centuries (century)"),
	DLTU_Millennium				UMETA(DisplayName = "Millennuim"),
};

//UENUM(BlueprintType)
//enum EDebugLogCurrencyUnit
//{
//	DLCU_Dollar					UMETA(DisplayName = "Dollar ($)"),
//	DLCU_Pound					UMETA(DisplayName = "Pound (£)"),
//	DLCU_Euro					UMETA(DisplayName = "Euro (€)"),
//	DLCU_ChineseYuan			UMETA(DisplayName = "Chinese Yuan (元)"),
//	DLCU_JapaneseYen 			UMETA(DisplayName = "Japanese Yen (¥)"),
//	DLCU_IndianRupee			UMETA(DisplayName = "Indian Rupee (₹)"),
//	DLCU_TurkishLira 			UMETA(DisplayName = "Turkish Lira (₺)"),
//};

UENUM(BlueprintType)
enum EDebugLogComparisonMethod
{
	CM_Equal_To					UMETA(DisplayName = "Equal To (==)"),
	CM_Not_Equal_To				UMETA(DisplayName = "Not Equal To (!=)"),
	CM_Greater_Than_Or_Equal_To	UMETA(DisplayName = "Greater Than or Equal To (>=)"),
	CM_Less_Than_Or_Equal_To	UMETA(DisplayName = "Less Than or Equal To (<=)"),
	CM_Greater_Than				UMETA(DisplayName = "Greater Than (>)"),
	CM_Less_Than				UMETA(DisplayName = "Less Than (<)")
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
		static void ObjectValidity(UObject* InObject, bool bSilenceOnError = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log the given object's name, if it's valid
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void ObjectName(UObject* InObject, bool bSilenceOnError = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

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

	// Log a hey message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Hey(ELoggingOptions LoggingOption = LO_Console);

	// Log a bye message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Bye(ELoggingOptions LoggingOption = LO_Console);

	// Log a goodbye message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Goodbye(ELoggingOptions LoggingOption = LO_Console);

	// Log a cya message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Cya(ELoggingOptions LoggingOption = LO_Console);

	// Log a wassup message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Wassup(ELoggingOptions LoggingOption = LO_Console);

	// Log a yo message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Yo(ELoggingOptions LoggingOption = LO_Console);

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
	
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
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
	static void Number(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version)
	static void Number(double Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

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
#endif
	
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

	// Log a FVector2D value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void Vector2D(const FVector2D& InVector2D, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Vector2D(const FVector2D& InVector2D, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

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

	// Log a DateTime value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		static void DateTime(const FDateTime& InDateTime, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void DateTime(const FDateTime& InDateTime, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	
	#pragma region Unit Systems
	// Log a temperature value in degrees celsius to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Temperature(float InTemperatureValue, EDebugLogTemperatureUnit TemperatureUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Temperature(float InTemperatureValue, EDebugLogTemperatureUnit TemperatureUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a volume unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Volume(float InVolumeValue, EDebugLogVolumeUnit VolumeUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Volume(float InVolumeValue, EDebugLogVolumeUnit VolumeUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a data unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Data(float InDataValue, EDebugLogDataUnit DataUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Data(float InDataValue, EDebugLogDataUnit DataUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a length unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Length(float InLengthValue, EDebugLogLengthUnit LengthUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Length(float InLengthValue, EDebugLogLengthUnit LengthUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a mass unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Mass(float InMassValue, EDebugLogMassUnit MassUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Mass(float InMassValue, EDebugLogMassUnit MassUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a speed unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Speed(float InSpeedValue, EDebugLogSpeedUnit SpeedUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Speed(float InSpeedValue, EDebugLogSpeedUnit SpeedUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a time unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Time(float InTimeValue, EDebugLogTimeUnit TimeUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Time(float InTimeValue, EDebugLogTimeUnit TimeUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a dollar value to the console or viewport (Just adds the $ symbol at the start)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly))
		static void Dollar(float InDollarValue, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
		static void Dollar(float InDollarValue, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	#pragma endregion

	// Log sphere information to the console or viewport
	static void Sphere(const FSphere& Sphere, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log box information to the console or viewport
	static void Box(const FBox& Box, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log capsule information to the console or viewport
	static void Capsule(const FCapsuleShape& Capsule, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Assert that two bools are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Bool)", meta = (DevelopmentOnly))
		static bool AssertEqual_Bool(bool bActual, bool bExpected, FString Message, bool bCrashOnFailure = false);

	// Assert that two integers are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Integer)", meta = (DevelopmentOnly))
		static bool AssertEqual_Integer(int32 Actual, int32 Expected, FString Message, bool bCrashOnFailure = false);

	// Assert on a relationship between two integers
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (Integer)", meta = (DevelopmentOnly))
		static bool AssertValue_Integer(int32 Actual, int32 Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false);

	// Assert that two floats are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Float)", meta = (DevelopmentOnly))
		static bool AssertEqual_Float(float Actual, float Expected, FString Message, bool bCrashOnFailure = false);

	// Assert on a relationship between two floats
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (Float)", meta = (DevelopmentOnly))
		static bool AssertValue_Float(float Actual, float Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false);

	// Assert that two strings are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (String)", meta = (DevelopmentOnly))
		static bool AssertEqual_String(FString Actual, FString Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two strings are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (String)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_String(FString Actual, FString Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two names are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Name)", meta = (DevelopmentOnly))
		static bool AssertEqual_Name(FName Actual, FName Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two names are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Name)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Name(FName Actual, FName Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two objects are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Object)", meta = (DevelopmentOnly))
		static bool AssertEqual_Object(UObject* Actual, UObject* Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two objects are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Object)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Object(UObject* Actual, UObject* Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two vectors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Vector)", meta = (DevelopmentOnly))
		static bool AssertEqual_Vector(FVector Actual, FVector Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two vectors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Vector2D)", meta = (DevelopmentOnly))
		static bool AssertEqual_Vector2D(FVector2D Actual, FVector2D Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two vectors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Vector)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Vector(FVector Actual, FVector Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two vectors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Vector2D)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Vector2D(FVector2D Actual, FVector2D Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two rotators are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Rotator)", meta = (DevelopmentOnly))
		static bool AssertEqual_Rotator(FRotator Actual, FRotator Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two rotators are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Rotator)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Rotator(FRotator Actual, FRotator Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two quaternions are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Quaternion)", meta = (DevelopmentOnly))
		static bool AssertEqual_Quat(FQuat Actual, FQuat Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two quaternions are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Quaternion)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Quat(FQuat Actual, FQuat Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two transforms are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Transform)", meta = (DevelopmentOnly))
		static bool AssertEqual_Transform(FTransform Actual, FTransform Expected, FString Message, bool bNoScale = false, bool bCrashOnFailure = false);

	// Assert that two transforms are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Transform)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Transform(FTransform Actual, FTransform Expected, FString Message, bool bNoScale = false, bool bCrashOnFailure = false);

	// Assert that two colors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Color)", meta = (DevelopmentOnly))
		static bool AssertEqual_Color(FColor Actual, FColor Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two colors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Color)", meta = (DevelopmentOnly))
		static bool AssertNotEqual_Color(FColor Actual, FColor Expected, FString Message, bool bCrashOnFailure = false);

	// Assert that two DateTime values are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (DateTime)", meta = (DevelopmentOnly))
		static bool AssertEqual_DateTime(FDateTime Actual, FDateTime Expected, FString Message, bool bCrashOnFailure = false);

	// Assert a relationship between two DateTime values
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (DateTime)", meta = (DevelopmentOnly))
		static bool AssertValue_DateTime(FDateTime Actual, FDateTime Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false);

	// Assert that a bool value is true
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert True", meta = (DevelopmentOnly))
		static bool Assert_True(bool bCondition, FString Message, bool bCrashOnFailure = false);

	// Assert that a bool value is false
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert False", meta = (DevelopmentOnly))
		static bool Assert_False(bool bCondition, FString Message, bool bCrashOnFailure = false);
	
	// Assert that an object is valid
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert Is Valid", meta = (DevelopmentOnly))
		static bool Assert_IsValid(UObject* Object, FString Message, bool bCrashOnFailure = false);

protected:
	void PostInitProperties() override;

	// Log a number to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (int)", meta = (DevelopmentOnly))
		static void Number_Int_Blueprint(int32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (float)", meta = (DevelopmentOnly))
		static void Number_Float_Blueprint(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

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
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	static void LogMessage_Internal(const FString& Message, const FString& Prefix = "", const FString& Suffix = "", const FColor& LogColor = FColor::Cyan, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	static void LogFloat(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogInt(platform_int Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogUInt(platform_uint Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogLongInt(long Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	static void LogUnitSystem(float Value, const FString& UnitSymbol, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);
	static void LogCurrencyUnitSystem(float Value, const FString& UnitSymbol, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	static FString DecimalToHex(platform_int DecimalNumber);
	static FString DecimalToBinary(platform_int DecimalNumber);
	static FString DecimalToOctal(platform_int DecimalNumber);
	static FString DecimalToRomanNumeral(platform_int DecimalNumber);

	static platform_int HexDigitToDecimal(FString HexDigit);
	static FString DecimalToHexDigit(platform_int DecimalNumber);

	static void AssertFailed(const FString& Message, bool bCrashOnFailure);
#endif

	template<typename T>
	static bool AssertEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure);

	template<typename T>
	static bool AssertNotEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure);

	template<typename T>
	static bool AssertEqual(T* Actual, T* Expected, FString AssertType, FString Message, bool bCrashOnFailure);

	template<typename T>
	static bool AssertNotEqual(T* Actual, T* Expected, FString AssertType, FString Message, bool bCrashOnFailure);

	template<typename T>
	static bool AssertValue(T Actual, T Expected, EDebugLogComparisonMethod ComparisonMethod, FString AssertType, FString Message, bool bCrashOnFailure);

	template<typename T>
	static bool PerformComparison(T LHS, T RHS, EDebugLogComparisonMethod ComparisonMethod);

	static const class UDebugLogLibrarySettings* Settings;
};

template <typename T>
bool ULog::AssertEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual != Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LO_Both, false);
	
		return false;
	}

	return true;
#endif

	return false;
}

template <typename T>
bool ULog::AssertNotEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual == Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LO_Both, false);
	
		return false;
	}

	return true;
#endif

	return false;
}

template <typename T>
bool ULog::AssertEqual(T* Actual, T* Expected, FString AssertType, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual != Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LO_Both, false);

		return false;
	}

	return true;	
#endif

	return false;
}

template <typename T>
bool ULog::AssertNotEqual(T* Actual, T* Expected, FString AssertType, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual == Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LO_Both, false);

		return false;
	}

	return true;	
#endif

	return false;
}

template <typename T>
bool ULog::AssertValue(T Actual, T Expected, const EDebugLogComparisonMethod ComparisonMethod, FString AssertType, const FString Message, const bool bCrashOnFailure)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!PerformComparison<T>(Actual, Expected, ComparisonMethod))
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LO_Both, false);

		return false;	
	}
	
	return true;	
#endif

	return false;
}

template <typename T>
bool ULog::PerformComparison(T LHS, T RHS, const EDebugLogComparisonMethod ComparisonMethod)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	switch (ComparisonMethod)
	{
	case CM_Equal_To:
		return LHS == RHS;

	case CM_Not_Equal_To:
		return LHS != RHS;

	case CM_Greater_Than_Or_Equal_To:
		return LHS >= RHS;

	case CM_Less_Than_Or_Equal_To:
		return LHS <= RHS;

	case CM_Greater_Than:
		return LHS > RHS;

	case CM_Less_Than:
		return LHS < RHS;
	}

	Error("Invalid comparison method", LO_Both, true);
	return false;
#endif

	return false;
}
