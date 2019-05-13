// Fill out your copyright notice in the Description page of Project Settings.


#include "SokobanGameModeBase.h"
#include "Marker.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ASokobanGameModeBase::ASokobanGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ASokobanGameModeBase::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarker::StaticClass(), FoundActors);
	InitNumMarkers = FoundActors.Num();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Number of markers is: %d"), InitNumMarkers);
}

void ASokobanGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CurrNumMarkers == InitNumMarkers)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("YOU WON"));

}