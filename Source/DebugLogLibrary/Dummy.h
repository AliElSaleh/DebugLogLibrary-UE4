// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dummy.generated.h"

UCLASS()
class DEBUGLOGLIBRARY_API ADummy : public AActor
{
	GENERATED_BODY()
	
public:	
	ADummy();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
};
