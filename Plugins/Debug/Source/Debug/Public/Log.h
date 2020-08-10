// Copyright Ali El Saleh 2020

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/TextProperty.h" // <-- Fixes compile error when using FText as a parameter for blueprint functions
#include "DebugLogLibrarySettings.h"
#include "UObject/Package.h"
#include "Engine/EngineTypes.h"
#include <chrono>
#include "Log.generated.h"

#define SPACER (FString(" | "))

#define GET_ENUM_OBJECT(EnumType) FindObject<UEnum>(ANY_PACKAGE, TEXT(#EnumType), true)
#define GET_ENUM_TYPE_AS_STRING(EnumType) #EnumType
#define GET_ENUM_VALUE_AS_STRING(EnumValue) #EnumValue
#define ENUM_TO_STRING(EnumType, EnumValue, bFriendlyName) FindObject<UEnum>(ANY_PACKAGE, TEXT(#EnumType), true) ? bFriendlyName ? FindObject<UEnum>(ANY_PACKAGE, TEXT(#EnumType), true)->GetDisplayNameTextByIndex(uint8(EnumValue)).ToString() : FindObject<UEnum>(ANY_PACKAGE, TEXT(#EnumType), true)->GetNameStringByIndex(uint8(EnumValue)) : ""\

// Get the current class name and function name where this is called
#define CUR_CLASS_FUNC (FString(__FUNCTION__))

#define CUR_CLASS_FUNC_2 (FString(__FUNCTION__) + " | ")

// Get the current class name, function name and line number where this is called
#define CUR_CLASS_FUNC_WITH_LINE (CUR_CLASS_FUNC + ": " + CUR_LINE)

#define CUR_CLASS_FUNC_WITH_LINE_2 (CUR_CLASS_FUNC + ": " + CUR_LINE + " | ")

// Get the current class where this is called
#define CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) + " | ")

#define CUR_CLASS_2 (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) + " | ")

// Get the current function name where this is called
#define CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

#define CUR_FUNC_2 (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ) + " | ")
  
// Get the current line number in the code where this is called
#define CUR_LINE ("Line " + FString::FromInt(__LINE__))

#define CUR_LINE_2 ("Line " + FString::FromInt(__LINE__) + " | ")

// Get the current function name and the line number in the code where this is called
#define CUR_FUNC_WITH_LINE  (CUR_FUNC + ": " + CUR_LINE)

#define CUR_FUNC_WITH_LINE_2  (CUR_FUNC + ": " + CUR_LINE + " | ")

// Get the current class and line number where this is called
#define CUR_CLASS_WITH_LINE (CUR_CLASS + "(" + CUR_LINE + ")")

#define CUR_CLASS_WITH_LINE_2 (CUR_CLASS + "(" + CUR_LINE + ")" + " | ")

// Get the current function signature where this is called
#define CUR_FUNC_SIG (FString(__FUNCSIG__))

#define CUR_FUNC_SIG_2 (FString(__FUNCSIG__) + " | ")

// Get the current function signature with the number where this is called
#define CUR_FUNC_SIG_WITH_LINE (FString(__FUNCSIG__) + ": " + CUR_LINE)

#define CUR_FUNC_SIG_WITH_LINE_2 (FString(__FUNCSIG__) + ": " + CUR_LINE + " | ")

// Returns the net mode as a string, possible outputs are, Empty String, [Client], [Server] or [Dedicated Server]
#define NET_MODE_PREFIX *(!GWorld ? FString("") \
	: GWorld->GetNetMode() == NM_Client ? FString("[Client] ") + FString::FromInt(GPlayInEditorID - 1) + FString(": ") \
	: GWorld->GetNetMode() == NM_ListenServer ? FString("[Server]: ") \
	: GWorld->GetNetMode() == NM_DedicatedServer ? FString("[Dedicated Server]: ") \
	: FString(""))

// Logs an assert message and crashes the program
#define ASSERT(expr, message) FDebug::AssertFailed(#expr, UE_LOG_SOURCE_FILE(__FILE__), __LINE__, TEXT("%s"), *message);

// Quick debug logging macros
#if defined UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
#define LOG_VECTOR(Variable, ...) ULog::Vector(Variable, false, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_VECTOR_CONSOLE(Variable, ...) ULog::Vector(Variable, false, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_VECTOR_VIEWPORT(Variable, ...) ULog::Vector(Variable, false, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_VECTOR_COMPACT(Variable, ...) ULog::Vector(Variable, true, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_VECTOR_COMPACT_CONSOLE(Variable, ...) ULog::Vector(Variable, true, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_VECTOR_COMPACT_VIEWPORT(Variable, ...) ULog::Vector(Variable, true, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_VECTOR2D(Variable, ...) ULog::Vector2D(Variable, false,__VA_ARGS__, LO_Both, 5.0f)
#define LOG_VECTOR2D_CONSOLE(Variable, ...) ULog::Vector2D(Variable, false, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_VECTOR2D_VIEWPORT(Variable, ...) ULog::Vector2D(Variable, false,__VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_VECTOR2D_COMPACT(Variable, ...) ULog::Vector2D(Variable, true,__VA_ARGS__, LO_Both, 5.0f)
#define LOG_VECTOR2D_COMPACT_CONSOLE(Variable, ...) ULog::Vector2D(Variable, true, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_VECTOR2D_COMPACT_VIEWPORT(Variable, ...) ULog::Vector2D(Variable, true,__VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_ROTATOR(Variable, ...) ULog::Rotator(Variable, false, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_ROTATOR_CONSOLE(Variable, ...) ULog::Rotator(Variable, false, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_ROTATOR_VIEWPORT(Variable, ...) ULog::Rotator(Variable, false, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_ROTATOR_COMPACT(Variable, ...) ULog::Rotator(Variable, true, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_ROTATOR_COMPACT_CONSOLE(Variable, ...) ULog::Rotator(Variable, true, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_ROTATOR_COMPACT_VIEWPORT(Variable, ...) ULog::Rotator(Variable, true, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_TRANSFORM(Variable, ...) ULog::Transform(Variable, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_TRANSFORM_CONSOLE(Variable, ...) ULog::Transform(Variable, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_TRANSFORM_VIEWPORT(Variable, ...) ULog::Transform(Variable, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_QUAT(Variable, ...) ULog::Quat(Variable, false, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_QUAT_CONSOLE(Variable, ...) ULog::Quat(Variable, false, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_QUAT_VIEWPORT(Variable, ...) ULog::Quat(Variable, false, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_QUAT_COMPACT(Variable, ...) ULog::Quat(Variable, true, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_QUAT_COMPACT_CONSOLE(Variable, ...) ULog::Quat(Variable, true, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_QUAT_COMPACT_VIEWPORT(Variable, ...) ULog::Quat(Variable, true, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_MATRIX(Variable, ...) ULog::Matrix(Variable, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_MATRIX_CONSOLE(Variable, ...) ULog::Matrix(Variable, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_MATRIX_VIEWPORT(Variable, ...) ULog::Matrix(Variable, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_COLOR(Variable, ...) ULog::Color(Variable, false, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_COLOR_CONSOLE(Variable, ...) ULog::Color(Variable, false, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_COLOR_VIEWPORT(Variable, ...) ULog::Color(Variable, false, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_COLOR_COMPACT(Variable, ...) ULog::Color(Variable, true, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_COLOR_COMPACT_CONSOLE(Variable, ...) ULog::Color(Variable, true, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_COLOR_COMPACT_VIEWPORT(Variable, ...) ULog::Color(Variable, true, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_TRUE() ULog::Warning("True", LO_Both, false, 5.0f)
#define LOG_TRUE_CONSOLE() ULog::Warning("True", LO_Console, false, 5.0f)
#define LOG_TRUE_VIEWPORT() ULog::Warning("True", LO_Viewport, false, 5.0f)

#define LOG_FALSE() ULog::Warning("False", LO_Both, false, 5.0f)
#define LOG_FALSE_CONSOLE() ULog::Warning("False", LO_Console, false, 5.0f)
#define LOG_FALSE_VIEWPORT() ULog::Warning("False", LO_Viewport, false, 5.0f)

#define LOG_NUMBER(Variable, ...) ULog::Number(Variable, __VA_ARGS__, DLNS_Decimal, LO_Both, 5.0f)
#define LOG_NUMBER_CONSOLE(Variable, ...) ULog::Number(Variable, __VA_ARGS__, DLNS_Decimal, LO_Console, 5.0f)
#define LOG_NUMBER_VIEWPORT(Variable, ...) ULog::Number(Variable, __VA_ARGS__, DLNS_Decimal, LO_Viewport, 5.0f)

#define LOG_BOOL(Variable, ...) ULog::Bool(Variable, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_BOOL_CONSOLE(Variable, ...) ULog::Bool(Variable, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_BOOL_VIEWPORT(Variable, ...) ULog::Bool(Variable, __VA_ARGS__, LO_Viewport, 5.0f)

//#define LOG_ENUM(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(Variable, bFriendlyName, __VA_ARGS__, LO_Both, 5.0f)
//#define LOG_ENUM_CONSOLE(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(Variable, bFriendlyName, __VA_ARGS__, LO_Console, 5.0f)
//#define LOG_ENUM_VIEWPORT(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(Variable, bFriendlyName, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_ENUM(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(#EnumType, Variable, bFriendlyName, __VA_ARGS__, LO_Both, 5.0f)
#define LOG_ENUM_CONSOLE(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(#EnumType, Variable, bFriendlyName, __VA_ARGS__, LO_Console, 5.0f)
#define LOG_ENUM_VIEWPORT(EnumType, Variable, bFriendlyName, ...) ULog::Enum<EnumType>(#EnumType, Variable, bFriendlyName, __VA_ARGS__, LO_Viewport, 5.0f)

#define LOG_LINEBREAK() ULog::LineBreak(LO_Both)
#define LOG_LINEBREAK_CONSOLE() ULog::LineBreak("", "", LO_Console)
#define LOG_LINEBREAK_VIEWPORT() ULog::LineBreak("", "", LO_Viewport)

#define LOG_LINEBREAK_S(Symbol) ULog::LineBreak_Symbol(#Symbol, LO_Both)
#define LOG_LINEBREAK_S_CONSOLE(Symbol) ULog::LineBreak_Symbol(#Symbol, LO_Console)
#define LOG_LINEBREAK_S_VIEWPORT(Symbol) ULog::LineBreak_Symbol(#Symbol, LO_Viewport)
#endif

#if PLATFORM_64BITS
	typedef int64 platform_int;
	typedef uint64 platform_uint;
#else
	typedef int32 platform_int;
	typedef uint32 platform_uint;
#endif

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
	DLLU_Micrometer		UMETA(DisplayName = "Micrometer (um)"),
	DLLU_Millimeter		UMETA(DisplayName = "Millimeter (mm)"),
	DLLU_Meter			UMETA(DisplayName = "Meter (m)"),
	DLLU_Kilometer		UMETA(DisplayName = "Kilometer (km)"),
	DLLU_Inch			UMETA(DisplayName = "Inches (in)"),
	DLLU_Feet			UMETA(DisplayName = "Feet (ft)"),
	DLLU_Mile			UMETA(DisplayName = "Miles (mi)"),
	DLLU_NauticalMile	UMETA(DisplayName = "Nautical Mile (nm)"),
	DLLU_Yard			UMETA(DisplayName = "Yard (yd)"),
	DLLU_Lightyears		UMETA(DisplayName = "Lightyears (ly)")
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
	DLSU_MicrometersPerSec		UMETA(DisplayName = "Micrometers per Sec (um/s)"),
	DLSU_MicrometersPerHour		UMETA(DisplayName = "Micrometers per Hour (um/h)"),
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
enum EDebugLogAngularUnit
{
	DLAU_Degrees			UMETA(DisplayName = "Degrees (deg)"),
	DLAU_Radians			UMETA(DisplayName = "Radians (rad)")
};

UENUM(BlueprintType)
enum EDebugLogForceUnit
{
	DLFU_Newtons			UMETA(DisplayName = "Newtons (N)"),
    DLFU_PoundsForce		UMETA(DisplayName = "Pounds Force (lbf)"),
    DLFU_KilogramsForce		UMETA(DisplayName = "Kilograms Force (kgf)")
};

UENUM(BlueprintType)
enum EDebugLogFrequencyUnit
{
	DLFU_Hertz					UMETA(DisplayName = "Hertz (Hz)"),
    DLFU_Kilohertz				UMETA(DisplayName = "Kilohertz (kHz)"),
    DLFU_Megahertz				UMETA(DisplayName = "Megahertz (MHz)"),
    DLFU_Gigahertz				UMETA(DisplayName = "Gigahertz (GHz)"),
    DLFU_RevolutionsPerMinute	UMETA(DisplayName = "RevolutionsPerMinute (rpm)")
};

UENUM(BlueprintType)
enum EDebugLogLightUnit
{
	DLLU_Lumens				UMETA(DisplayName = "Lumens (lm)"),
    DLLU_Candela			UMETA(DisplayName = "Candela (cd)"),
    DLLU_Lux				UMETA(DisplayName = "Lux (lx)"),
    DLLU_CandelaPerMeter2	UMETA(DisplayName = "CandelaPerMeter2 (cd/m2)"),
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

class FDebugLogTimer
{
public:
	FDebugLogTimer()
	{
		Start();
	}

	double GetDuration() const
	{
		return Duration;
	}

	double GetDurationInSeconds() const
	{
		return Duration / 1000000000;
	}

	double GetDurationInMilliseconds() const
	{
		return Duration / 1000000;
	}
	
	double GetDurationInMicroseconds() const
	{
		return Duration / 1000;
	}

	FString GetDescription() const
	{
		return Description;
	}

	void Start(const FString& InDescription = "")
	{
		Duration = 0.0f;
		Description = InDescription;
		StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		Duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - StartTimePoint).count();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTimePoint;

	FString Description;
	double Duration = 0.0f;
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
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void ObjectValidity(UObject* InObject, bool bSilenceOnError = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log the given object's name, if it's valid
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void ObjectName(UObject* InObject, bool bSilenceOnError = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a debug message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void DebugMessage(EDebugLogType LogSeverity, const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void DebugMessage(EDebugLogType LogSeverity, const FName& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);	

	// Log a debug message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Debug Message (Condition)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void DebugMessage_WithCondition(EDebugLogType LogSeverity, bool bCondition, const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Crash the game, and log the message to the console and log file. FromFunction means the function (as a string) that this 'Crash' function was called from. Ultimately, this is for better debug information and is optional
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
	static void Crash(const FString& Message = "", const FString& FromFunction = "", UObject* ContextObject = nullptr);
		
	// Log a fatal error message to the console and crash
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
	static void Fatal(const FString& Message);

	// Log a fatal error message to the console and crash, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Fatal (Condition)", meta = (DevelopmentOnly))
	static void Fatal_WithCondition(const FString& Message, bool bCondition);

	// Log an error message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Error(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an error message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Error (Condition)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Error_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a success message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Success(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a success message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Success (Condition)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Success_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a warning message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Warning(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a warning message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Warning (Condition)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Warning_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an information message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Info(const FString& Message, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an information message to the console or viewport, only if the condition is met
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Info (Condition)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Info_WithCondition(const FString& Message, bool bCondition, ELoggingOptions LoggingOption = LO_Console, bool bAddPrefix = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a hello message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Hello(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a hey message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Hey(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a bye message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Bye(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a goodbye message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Goodbye(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a cya message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Cya(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a wassup message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Wassup(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a yo message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Yo(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a 'valid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Valid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	// Log a 'valid' message to the console or viewport
	static void Valid(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an 'invalid' message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Invalid(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	// Log an 'invalid' message to the console or viewport
	static void Invalid(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a yes message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Yes(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	// Log a yes message to the console or viewport
	static void Yes(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a no message to the console or viewport (with an optional prefix and suffix string)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void No(const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	// Log a no message to the console or viewport
	static void No(ELoggingOptions LoggingOption = LO_Console, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a line break to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
    static void LineBreak(ELoggingOptions LoggingOption = LO_Console);

	// Log a line break with a symbol string to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly), DisplayName = "Line Break (Symbol)")
    static void LineBreak_Symbol(const FString& Symbol, ELoggingOptions LoggingOption = LO_Console);

	// Creates and starts a debug timer
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
	static void StartDebugTimer(const FString& Description);
	
	// Stops and destroys the active debug timer, outputting how long the operation took since StartDebugTimer was called
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void StopDebugTimer(bool bAutoDetermineTimeUnit = false, EDebugLogTimeUnit DisplayIn = DLTU_Seconds, ELoggingOptions LoggingOption = LO_Both, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Return the string with brackets () around it
	static FString InBrackets(const FString& String);

	// Return the name with brackets () around it
	static FName InBrackets(const FName& Name);

	// Return the text with brackets () around it
	static FText InBrackets(const FText& Text);
	
	// Log a number to the console or viewport (int8 version)
	static void Number(int8 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int16 version)
	static void Number(int16 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int32 version)
	static void Number(int32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int64 version)
	static void Number(int64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint8 version)
	static void Number(uint8 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint16 version)
	static void Number(uint16 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint32 version)
	static void Number(uint32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint64 version)
	static void Number(uint64 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (float version)
	static void Number(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (double version)
	static void Number(double Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (long version)
	static void Number(long Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int8 version, no prefix and suffix)
	static void Number(int8 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int16 version, no prefix and suffix)
	static void Number(int16 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int32 version, no prefix and suffix)
	static void Number(int32 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (int64 version, no prefix and suffix)
	static void Number(int64 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint8 version, no prefix and suffix)
	static void Number(uint8 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint16 version, no prefix and suffix)
	static void Number(uint16 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint32 version, no prefix and suffix)
	static void Number(uint32 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (uint64 version, no prefix and suffix)
	static void Number(uint64 Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (float version, no prefix and suffix)
	static void Number(float Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (double version, no prefix and suffix)
	static void Number(double Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (long version, no prefix and suffix)
	static void Number(long Number, EDebugLogNumberSystems NumberSystem, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a enum value to the console or viewport.
	// Enum classes must be marked with a UENUM() macro
	template<typename EnumType>
    static void Enum(const FString& EnumTypeString, const EnumType& EnumValue, bool bFriendlyName = false, const FString& Prefix = "", const FString& Suffix = "", const ELoggingOptions& LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a enum value to the console or viewport. (no prefix and suffix)
	// Enum classes must be marked with a UENUM() macro
	template<typename EnumType>
    static void Enum(const FString& EnumTypeString, const EnumType& EnumValue, bool bFriendlyName = false, const ELoggingOptions& LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an array of int32 to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (int32)")
	static void Array_Int32(TArray<int32> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of int64 to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (int64)")
    static void Array_Int64(TArray<int64> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);
	
	// Log an array of floats to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (float)")
    static void Array_Float(TArray<float> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of doubles to the console or viewport
    static void Array_Double(TArray<double> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);
	
	// Log an array of bools to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (bool)")
    static void Array_Bool(TArray<bool> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FVectors to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Vector)")
	static void Array_Vector(TArray<FVector> InArray, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FVector2D to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Vector 2D)")
    static void Array_Vector2D(TArray<FVector2D> InArray, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FRotator to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Rotator)")
    static void Array_Rotator(TArray<FRotator> InArray, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FTransform to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Transform)")
    static void Array_Transform(TArray<FTransform> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of Quaternion to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Quaternion)")
    static void Array_Quat(TArray<FQuat> InArray, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of Matrix to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Matrix)")
    static void Array_Matrix(TArray<FMatrix> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FString to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (String)")
    static void Array_String(TArray<FString> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FName to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Name)")
    static void Array_Name(TArray<FName> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FText to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Text)")
    static void Array_Text(TArray<FText> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FDateTime to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (DateTime)")
    static void Array_DateTime(TArray<FDateTime> InArray, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log an array of FLinearColor to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"), DisplayName = "Array (Color)")
    static void Array_Color(TArray<FLinearColor> InArray,  bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	// Log the a percentage value to the console or viewport (Just appends a % symbol)
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Percent(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Percent(float Number, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log the given bool value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Bool(bool bBoolToTest, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Bool(bool bBoolToTest, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a FVector to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Vector(const FVector& InVector, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Vector(const FVector& InVector, bool bCompact = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a FVector2D to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Vector2D(const FVector2D& InVector2D, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Vector2D(const FVector2D& InVector2D, bool bCompact = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a FRotator to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Rotator(const FRotator& InRotator, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Rotator(const FRotator& InRotator, bool bCompact = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a FTransform value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Transform(const FTransform& InTransform, const FString& Prefix = "", bool bFormat = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Transform(const FTransform& InTransform, ELoggingOptions LoggingOption = LO_Console, bool bFormat = false, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a Quaternion value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Quat(const FQuat& Quaternion, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Quat(const FQuat& Quaternion, bool bCompact = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a matrix value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Matrix(const FMatrix& InMatrix, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Matrix(const FMatrix& InMatrix, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a color value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Color(const FLinearColor& InColor, bool bCompact = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Color(const FLinearColor& InColor, bool bCompact = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a DateTime value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void DateTime(const FDateTime& InDateTime, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void DateTime(const FDateTime& InDateTime, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	
	// Log a message in the world
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
	static void MessageInWorld(const FString& Message, const FVector& WorldLocation, float FontScale = 1.0f, const FString& Prefix = "", const FString& Suffix = "", float TimeToDisplay = 5.0f);
	static void MessageInWorld(const FString& Message, const FVector& WorldLocation, float FontScale = 1.0f, float TimeToDisplay = 5.0f);
	
	#pragma region Unit Systems
	// Log a unit value to the console or viewport (Just adds the appropriate symbol at the end) Not supported in blueprints
	static void Unit(float InUnitValue, EUnit InUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Unit(float InUnitValue, EUnit InUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	
	// Log a temperature value in degrees celsius to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Temperature(float InTemperatureValue, EDebugLogTemperatureUnit TemperatureUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Temperature(float InTemperatureValue, EDebugLogTemperatureUnit TemperatureUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a volume unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Volume(float InVolumeValue, EDebugLogVolumeUnit VolumeUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Volume(float InVolumeValue, EDebugLogVolumeUnit VolumeUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a data unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Data(float InDataValue, EDebugLogDataUnit DataUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Data(float InDataValue, EDebugLogDataUnit DataUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a length unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Length(float InLengthValue, EDebugLogLengthUnit LengthUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Length(float InLengthValue, EDebugLogLengthUnit LengthUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a mass unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Mass(float InMassValue, EDebugLogMassUnit MassUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Mass(float InMassValue, EDebugLogMassUnit MassUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a speed unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Speed(float InSpeedValue, EDebugLogSpeedUnit SpeedUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Speed(float InSpeedValue, EDebugLogSpeedUnit SpeedUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a time unit value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Time(float InTimeValue, EDebugLogTimeUnit TimeUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Time(float InTimeValue, EDebugLogTimeUnit TimeUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log an angular value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
    static void Angle(float InAngleValue, EDebugLogAngularUnit AngleUnit = DLAU_Degrees, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Angle(float InAngleValue, EDebugLogAngularUnit AngleUnit = DLAU_Degrees, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	
	// Log a force value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
    static void Force(float InForceValue, EDebugLogForceUnit ForceUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Force(float InForceValue, EDebugLogForceUnit ForceUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	
	// Log a frequency value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
    static void Frequency(float InFrequencyValue, EDebugLogFrequencyUnit FrequencyUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Frequency(float InFrequencyValue, EDebugLogFrequencyUnit FrequencyUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a light value to the console or viewport (Just adds the appropriate symbol at the end)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
    static void Light(float InLightValue, EDebugLogLightUnit LightUnit, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Light(float InLightValue, EDebugLogLightUnit LightUnit, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a dollar value to the console or viewport (Just adds the $ symbol at the start)
	UFUNCTION(BlueprintCallable, Category = "Debug | Unit Systems", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Dollar(float InDollarValue, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void Dollar(float InDollarValue, bool bConvertValueToInt = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	#pragma endregion

	// Log an actor's net role value to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug | Networking", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void NetRole(AActor* InActor, bool bLocal = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void NetRole(AActor* InActor, bool bLocal = false, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log sphere information to the console or viewport
	static void Sphere(const FSphere& Sphere, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log box information to the console or viewport
	static void Box(const FBox& Box, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Log capsule information to the console or viewport
	static void Capsule(const FCapsuleShape& Capsule, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f);

	// Assert that two bools are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Bool)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Bool(bool bActual, bool bExpected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two integers are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Integer)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Integer(int32 Actual, int32 Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert on a relationship between two integers
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (Integer)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertValue_Integer(int32 Actual, int32 Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two floats are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Float)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Float(float Actual, float Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert on a relationship between two floats
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (Float)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertValue_Float(float Actual, float Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two strings are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (String)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_String(FString Actual, FString Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two strings are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (String)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_String(FString Actual, FString Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two names are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Name)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Name(FName Actual, FName Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two names are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Name)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Name(FName Actual, FName Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two objects are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Object)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Object(UObject* Actual, UObject* Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two objects are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Object)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Object(UObject* Actual, UObject* Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two vectors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Vector)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Vector(FVector Actual, FVector Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two vectors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Vector2D)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Vector2D(FVector2D Actual, FVector2D Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two vectors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Vector)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Vector(FVector Actual, FVector Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two vectors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Vector2D)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Vector2D(FVector2D Actual, FVector2D Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two rotators are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Rotator)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Rotator(FRotator Actual, FRotator Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two rotators are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Rotator)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Rotator(FRotator Actual, FRotator Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two quaternions are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Quaternion)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Quat(FQuat Actual, FQuat Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two quaternions are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Quaternion)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Quat(FQuat Actual, FQuat Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two transforms are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Transform)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Transform(FTransform Actual, FTransform Expected, FString Message, bool bNoScale = false, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two transforms are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Transform)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Transform(FTransform Actual, FTransform Expected, FString Message, bool bNoScale = false, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two colors are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (Color)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_Color(FColor Actual, FColor Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two colors are not equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Not Equal", DisplayName = "Assert Not Equal (Color)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertNotEqual_Color(FColor Actual, FColor Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that two DateTime values are equal
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Equal", DisplayName = "Assert Equal (DateTime)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertEqual_DateTime(FDateTime Actual, FDateTime Expected, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert a relationship between two DateTime values
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert | Value", DisplayName = "Assert Value (DateTime)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool AssertValue_DateTime(FDateTime Actual, FDateTime Expected, EDebugLogComparisonMethod ShouldBe, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that a bool value is true
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert True", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool Assert_True(bool bCondition, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Assert that a bool value is false
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert False", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool Assert_False(bool bCondition, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	
	// Assert that an object is valid
	UFUNCTION(BlueprintCallable, Category = "Debug | Assert", DisplayName = "Assert Is Valid", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static bool Assert_IsValid(UObject* Object, FString Message, bool bCrashOnFailure = false, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

protected:
	void PostInitProperties() override;
	void FinishDestroy() override;

	// Log a number to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (int)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Number_Int_Blueprint(int32 Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Log a number to the console or viewport (float version)
	UFUNCTION(BlueprintCallable, Category = "Debug", DisplayName = "Number (float)", meta = (DevelopmentOnly, AdvancedDisplay = "ViewportKeyName", DefaultToSelf = "ContextObject", HidePin = "ContextObject"))
	static void Number_Float_Blueprint(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	// Return the string with brackets () around it
	UFUNCTION(BlueprintPure, Category = "Debug", DisplayName = "In Brackets (String)")
	static FString InBrackets_String_Blueprint(const FString& String);

	// Return the name with brackets () around it
	UFUNCTION(BlueprintPure, Category = "Debug", DisplayName = "In Brackets (Name)")
	static FName InBrackets_Name_Blueprint(FName Name);
		
	// Return the text with brackets () around it
	UFUNCTION(BlueprintPure, Category = "Debug", DisplayName = "In Brackets (Text)")
	static FText InBrackets_Text_Blueprint(FText Text);

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
	static void LogMessage_Internal(const FString& Message, EDebugLogType LogSeverity, const FString& Prefix = "", const FString& Suffix = "", const FColor& InLogColor = FColor::Cyan, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, const FString& LogCategory = "", bool bWriteToLog = true, UObject* ContextObject = nullptr);

	static void LogFloat(float Number, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void LogInt(platform_int Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void LogUInt(platform_uint Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void LogLongInt(long Number, const FString& Prefix = "", const FString& Suffix = "", EDebugLogNumberSystems NumberSystem = DLNS_Decimal, ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	static void LogUnitSystem(float Value, const FString& UnitSymbol, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);
	static void LogCurrencyUnitSystem(float Value, const FString& UnitSymbol, bool bConvertValueToInt = false, const FString& Prefix = "", const FString& Suffix = "", ELoggingOptions LoggingOption = LO_Console, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	static void LogArray_Internal(const TArray<FString>& InArray, const FString& Prefix = "", const FString& Suffix = "", const ELoggingOptions& LoggingOption = LO_Console, float TimeToDisplay = 5.0f, UObject* ContextObject = nullptr);

	static void QuitApplication_Internal();

	static bool WriteToLogFile(const FString& FileName, const FString& Text, bool bAllowOverwriting = false);

	static FString GetSavedLogsDirectory();

	static FString DecimalToHex(platform_int DecimalNumber);
	static FString DecimalToBinary(platform_int DecimalNumber);
	static FString DecimalToOctal(platform_int DecimalNumber);
	static FString DecimalToRomanNumeral(platform_int DecimalNumber);

	static platform_int HexDigitToDecimal(FString HexDigit);
	static FString DecimalToHexDigit(platform_int DecimalNumber);

	static void AssertFailed(const FString& Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool AssertEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool AssertNotEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool AssertEqual(T* Actual, T* Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool AssertNotEqual(T* Actual, T* Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool AssertValue(T Actual, T Expected, EDebugLogComparisonMethod ComparisonMethod, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption = LO_Both, float TimeToDisplay = 5.0f, FName ViewportKeyName = NAME_None, UObject* ContextObject = nullptr);

	template<typename T>
	static bool PerformComparison(T LHS, T RHS, EDebugLogComparisonMethod ComparisonMethod);

	static class UDebugLogLibrarySettings* Settings;

	static class FDebugLogTimer* Timer;

	static FDateTime SavedDateTime;
	
	static FString LogFilename;

	static bool bIsShippingBuild;
};

template <typename T>
bool ULog::AssertEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption, float TimeToDisplay, FName ViewportKeyName, UObject* ContextObject)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual != Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName, ContextObject);
	
		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

template <typename T>
bool ULog::AssertNotEqual(T Actual, T Expected, FString AssertType, FString Message, bool bCrashOnFailure, ELoggingOptions LoggingOption, float TimeToDisplay, FName ViewportKeyName, UObject* ContextObject)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual == Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName, ContextObject);
	
		return false;
	}

	return true;
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

template <typename T>
bool ULog::AssertEqual(T* Actual, T* Expected, FString AssertType, const FString Message, const bool bCrashOnFailure, ELoggingOptions LoggingOption, float TimeToDisplay, FName ViewportKeyName, UObject* ContextObject)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual != Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName, ContextObject);

		return false;
	}

	return true;	
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

template <typename T>
bool ULog::AssertNotEqual(T* Actual, T* Expected, FString AssertType, const FString Message, const bool bCrashOnFailure, ELoggingOptions LoggingOption, float TimeToDisplay, FName ViewportKeyName, UObject* ContextObject)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (Actual == Expected)
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName, ContextObject);

		return false;
	}

	return true;	
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

template <typename T>
bool ULog::AssertValue(T Actual, T Expected, const EDebugLogComparisonMethod ComparisonMethod, FString AssertType, const FString Message, const bool bCrashOnFailure, ELoggingOptions LoggingOption, float TimeToDisplay, FName ViewportKeyName, UObject* ContextObject)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!PerformComparison<T>(Actual, Expected, ComparisonMethod))
	{
		if (bCrashOnFailure)
			Crash(AssertType + " | Assert Failed: " + Message);
		else
			Error(AssertType + " | Assert Failed: " + Message, LoggingOption, false, TimeToDisplay, ViewportKeyName, ContextObject);

		return false;	
	}
	
	return true;	
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
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
#elif (UE_BUILD_SHIPPING)
	if (Settings->bCrashGameInShippingBuildConfiguration)
		Crash("", FString(__FUNCTION__));
#endif

	return false;
}

template <typename EnumType>
void ULog::Enum(const FString& EnumTypeString, const EnumType& EnumValue, const bool bFriendlyName, const FString& Prefix, const FString& Suffix, const ELoggingOptions& LoggingOption, const float TimeToDisplay, const FName ViewportKeyName, UObject* ContextObject)
{
	UEnum* EnumObject = FindObject<UEnum>(ANY_PACKAGE, *EnumTypeString, true);
	if (EnumObject)
	{
		if (bFriendlyName)
		{
			LogMessage_Internal(EnumObject->GetDisplayNameTextByIndex(uint8(EnumValue)).ToString(), DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, "LogEnum", true, ContextObject);
		}
		else
		{
			LogMessage_Internal(EnumObject->GetNameStringByIndex(uint8(EnumValue)), DL_Info, Prefix, Suffix, Settings->InfoColor, LoggingOption, TimeToDisplay, ViewportKeyName, "LogEnum", true, ContextObject);
		}
	}
	else
	{
		Error(CUR_CLASS_FUNC + " | The given enum type '" + EnumTypeString + "' does not exist! Is '" + EnumTypeString + "' marked with a UENUM() macro?", LoggingOption, true, TimeToDisplay, ViewportKeyName, ContextObject);
	}
}

template <typename EnumType>
void ULog::Enum(const FString& EnumTypeString, const EnumType& EnumValue, const bool bFriendlyName, const ELoggingOptions& LoggingOption, const float TimeToDisplay, const FName ViewportKeyName, UObject* ContextObject)
{
	Enum<EnumType>(EnumTypeString, EnumValue, bFriendlyName, "", "", LoggingOption, TimeToDisplay, ViewportKeyName, ContextObject);
}
