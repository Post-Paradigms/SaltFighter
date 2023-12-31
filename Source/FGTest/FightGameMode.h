// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FighterController.h"
#include "Fighter.h"
#include "FightingHUDUserWidget.h"
#include "FightGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFightGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values
	AFightGameMode();
	
	// Fighting HUD Getter
	UFUNCTION()
	UFightingHUDUserWidget* GetFightingHUD();

	// Reset the map and players for the next round
	UFUNCTION()
	void ResetRound();

	// Damage a player and update the hud
	UFUNCTION()
	void DamagePlayer(AFighter* Fighter, int Damage);

	UFUNCTION()
	AFighterController* GetPlayer1Controller();

	UFUNCTION()
	AFighterController* GetPlayer2Controller();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* r1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* r2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* r3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* tuth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* lc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool IsPaused = false;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPauseMenu();

	UFUNCTION(BlueprintCallable)
	void SignalToPause();

	UFUNCTION(BlueprintImplementableEvent)
	void PauseGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when removed from level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//void SpawnPlayers();


private:
	// HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFightingHUDUserWidget> FightingHUDClass;

	UPROPERTY()
	class UFightingHUDUserWidget* FightingHUD;

	// Fighter Class and Controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFighter> FighterBPClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFighter> PepperBPClass;

	UPROPERTY()
	class AFighterController* P1FighterController;

	UPROPERTY()
	class AFighter* P1FighterCharacter;

	UPROPERTY()
	class AFighterController* P2FighterController;

	UPROPERTY()
	class AFighter* P2FighterCharacter;

	UFUNCTION()
	void BeginPlayPlay();

	FTimerHandle BeginPlayTimer;

	FTimerHandle StartRoundCountdownTimer;

	int StartRoundCountdownTimerCount; // jesus christ

	void StartRoundCountdown();

};
