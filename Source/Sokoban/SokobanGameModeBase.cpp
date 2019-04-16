// Fill out your copyright notice in the Description page of Project Settings.


#include "SokobanGameModeBase.h"
//#include "TPPawn.h"

ASokobanGameModeBase::ASokobanGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//DefaultPawnClass = ATPPawn::StaticClass();
}

void ASokobanGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
}