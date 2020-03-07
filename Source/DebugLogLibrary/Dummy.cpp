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
	ULog::Hello(LO_Both);
#endif
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

