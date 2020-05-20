// Fill out your copyright notice in the Description page of Project Settings.

#include "Dummy.h"
#include "Log.h"

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
	
	ULog::Enum<EAxis::Type>(EAxis::X, true, "Axis: ", "", LO_Both);
	ULog::Enum<ELoggingOptions>(LO_Both, true, "Logging Option: ", "", LO_Both);
	ULog::Enum<ETestEnum>(Test1, true, "Test enum: ", "", LO_Both);
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

