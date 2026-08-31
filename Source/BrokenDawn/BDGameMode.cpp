// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGameMode.h"

void ABDGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// Test log
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World"));
}