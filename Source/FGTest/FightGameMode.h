// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FighterController.h"
#include "Fighter.h"
#include "FightGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFightGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AFightGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when removed from level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFightingHUDUserWidget> FightingHUDClass;

	UPROPERTY()
	class UFightingHUDUserWidget* FightingHUD;

	// Fighter Class and Controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFighter> FighterBPClass;

	AFighterController* P1FighterController;
	AFighter* P1FighterCharacter;

	AFighterController* P2FighterController;
	AFighter* P2FighterCharacter;
};
