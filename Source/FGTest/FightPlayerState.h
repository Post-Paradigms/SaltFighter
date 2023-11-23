// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FightPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFightPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	// Sets default values
	AFightPlayerState();

	// Health, duh?
	int PlayerHealth;

	// Number of rounds this player has won
	int RoundsWon;
};
