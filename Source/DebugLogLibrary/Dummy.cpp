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
	//const FCapsuleShape Capsule{GetActorLocation(), 5.0f, FVector::UpVector, 20.0f};
	//const FSphere Sphere{GetActorLocation(), 5.0f};
	const FBox Box{GetActorLocation() - FVector(10.0f), GetActorLocation() + FVector(10.0f)};
	//ULog::Capsule(Capsule, "[Begin My Capsule]", "[End My Capsule]", LO_Both);
	//ULog::Sphere(Sphere, "[Begin My Sphere]", "[End My Sphere]", LO_Both);
	ULog::Box(Box, "[Begin My Box]", "[End My Box]", LO_Both);
#endif
}

void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

