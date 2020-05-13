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

	const FVector A(1.0f);
	const FColor B(1.0f);

	LOG_VECTOR(A);
	LOG_VECTOR(A, "Vector: ", "");
	LOG_COLOR(B);
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

