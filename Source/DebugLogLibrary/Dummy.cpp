// Fill out your copyright notice in the Description page of Project Settings.

#include "Dummy.h"
#include "Log.h"
#include "Engine/Engine.h"

ADummy::ADummy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADummy::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING
	ULog::Error(CUR_CLASS_FUNC_WITH_LINE, LO_Both);
#endif
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

