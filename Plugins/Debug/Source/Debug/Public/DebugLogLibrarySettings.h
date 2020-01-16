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

	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FLinearColor InfoColor = FColor::Cyan;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FLinearColor ErrorColor = FColor::Red;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FLinearColor WarningColor = FColor::Yellow;
	
	UPROPERTY(EditAnywhere, config, Category = "Viewport Log Colors")
		FLinearColor SuccessColor = FColor::Green;

	UPROPERTY(EditAnywhere, config, Category = "Settings")
		uint8 bUseTwosComplimentForDecimalToHexConversion : 1;
};
