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

	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors", DisplayName = "Default Color")
		FColor InfoColor = FColor::Cyan;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor ErrorColor = FColor::Red;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor WarningColor = FColor::Yellow;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FColor SuccessColor = FColor::Green;

	UPROPERTY(EditAnywhere, config, Category = "Settings")
		uint8 bUseTwosComplimentForDecimalToHexConversionOnNegativeNumbers : 1;
};
