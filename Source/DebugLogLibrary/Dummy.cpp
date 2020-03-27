// Fill out your copyright notice in the Description page of Project Settings.

#include "Dummy.h"
#if !UE_BUILD_SHIPPING
#include "Log.h"
#endif

ADummy::ADummy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADummy::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING
	const FVector A(1.0f);
	const FColor B(1.0f);

	QUICK_LOG_VIEWPORT_VECTOR(A);
	QUICK_LOG_CONSOLE_COLOR(B);
#endif
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

