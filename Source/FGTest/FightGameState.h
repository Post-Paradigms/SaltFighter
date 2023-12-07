// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FightGameState.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFightGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	// Sets default values
	AFightGameState();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Current round number
	int RoundNumber;

	// Round Timer
	float RoundTimer;

	int Player1Wins;

	int Player2Wins;

	UPROPERTY()
	class UFightingHUDUserWidget* FightingHUD;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	// Round Timer Update Delay
	float RoundTimerDelay;
};
