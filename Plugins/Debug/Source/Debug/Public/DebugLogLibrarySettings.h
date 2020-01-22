// Copyright Ali El Saleh 2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DebugLogLibrarySettings.generated.h"

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
		FColor InfoColor = FColor::Cyan;
	
	// The color to use when logging a error message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor ErrorColor = FColor::Red;
	
	// The color to use when logging a warning message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor WarningColor = FColor::Yellow;
	
	// The color to use when logging a success message
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor SuccessColor = FColor::Green;

	// Should we use Two's Compliment when converting a decimal number to a hexadecimal value on negative numbers?
	UPROPERTY(EditAnywhere, config, Category = "Settings")
		uint8 bUseTwosComplimentForDecimalToHexConversionOnNegativeNumbers : 1;
};
