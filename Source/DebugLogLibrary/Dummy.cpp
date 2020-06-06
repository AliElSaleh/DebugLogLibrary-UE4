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
	const FRotator C(1.0f);
	const FQuat D(1.0f, 1.0f, 1.0f, 1.0f);
	const FVector2D E(1.0f);

	LOG_VECTOR(A);
	LOG_VECTOR_COMPACT(A, "Vector Compact: ", "");
	LOG_VECTOR(A, "Vector: ", "");
	
	LOG_LINEBREAK();
	
	LOG_VECTOR2D(E);
	LOG_VECTOR2D_COMPACT(E);

	LOG_LINEBREAK();
	
	LOG_COLOR(B);
	LOG_COLOR_COMPACT(B);

	LOG_LINEBREAK();

	LOG_ROTATOR(C);
	LOG_ROTATOR_COMPACT(C);

	LOG_LINEBREAK();

	LOG_QUAT(D);
	LOG_QUAT_COMPACT(D);

	LOG_LINEBREAK();
	LOG_LINEBREAK_S(-------------);
	LOG_LINEBREAK();
	
	LOG_ENUM(EAxis::Type, EAxis::X, true, "Axis: ", "");
	LOG_ENUM(ELoggingOptions, ELoggingOptions::LO_Both, true);
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

