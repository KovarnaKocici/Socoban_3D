// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SokobanGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SOKOBAN_API ASokobanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ASokobanGameModeBase(const FObjectInitializer& ObjectInitializer);
	virtual void StartPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int InitNumMarkers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrNumMarkers;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
