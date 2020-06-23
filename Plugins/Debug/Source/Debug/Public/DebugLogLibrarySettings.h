// Copyright Ali El Saleh 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DebugLogLibrarySettings.generated.h"

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
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class DEBUG_API UDebugLogLibrarySettings final : public UObject
{
	GENERATED_BODY()

public:
	UDebugLogLibrarySettings(const FObjectInitializer& ObjectInitializer);

	// The color to use when logging a standard message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors", DisplayName = "Default Color")
	FColor InfoColor = FLinearColor(0.0f, 0.66f, 1.0f).ToFColor(true);
	
	// The color to use when logging a error message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
	FColor ErrorColor = FColor::Red;
	
	// The color to use when logging a warning message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
	FColor WarningColor = FColor::Yellow;
	
	// The color to use when logging a success message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
	FColor SuccessColor = FColor::Green;

	// If true, add a specifier text before the final output log message
	UPROPERTY(EditAnywhere, config, Category = "Settings")
	uint8 bEnableSpecifiers : 1;

	// The place to show the category specifier text
	UPROPERTY(EditAnywhere, config, Category = "Settings", meta = (EditCondition = "bEnableSpecifiers"))
	TEnumAsByte<ELoggingOptions> ShowSpecifiersIn = LO_Both;

	// The tag specifers to use when logging in these categories
	UPROPERTY(EditAnywhere, config, Category = "Settings", EditFixedSize = 5, meta = (EditCondition = "bEnableSpecifiers"))
	TMap<TEnumAsByte<EDebugLogType>, FString> LogCategorySpecifiers;

	// The key to use when calling any logging functions that specify its corresponding name (case-insensitive)
	UPROPERTY(EditAnywhere, config, Category = "Settings")
    TMap<FName, int32> ViewportLogKeys;
	
	// Should we use Two's Compliment when converting a decimal number to a hexadecimal value on negative numbers?
	UPROPERTY(EditAnywhere, config, Category = "Settings")
	uint8 bUseTwosComplimentForDecimalToHexConversionOnNegativeNumbers : 1;

	// Should the game crash if any ULog functions are called in a shipping build configuration?
	UPROPERTY(EditAnywhere, config, Category = "Settings")
	uint8 bCrashGameInShippingBuildConfiguration : 1;
};
