// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameState.h"
#include "FightGameMode.h"

// Sets default values
AFightGameState::AFightGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	RoundTimer = 100.0f;
	RoundTimerDelay = 1.0f;
}

void AFightGameState::BeginPlay()
{
	Super::BeginPlay();

	FightingHUD = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode())->GetFightingHUD();
}

void AFightGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RoundTimer -= DeltaTime;

	RoundTimerDelay -= DeltaTime;
	if (RoundTimerDelay <= 0.0f)
	{
		FightingHUD->UpdateRoundTimer((int) (RoundTimer + 0.5f));
		RoundTimerDelay = 1.0f;
	}
}
