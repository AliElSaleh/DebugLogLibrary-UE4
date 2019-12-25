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

	ULog::Number(324.0f, LO_Both);
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

