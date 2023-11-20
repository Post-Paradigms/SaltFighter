// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FightingHUDUserWidget.generated.h"

/**
 * Displays player health and round timer.
 */
UCLASS()
class FGTEST_API UFightingHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RoundTimer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player1Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player2Health;

	UFUNCTION()
	void UpdateRoundTimer(int time);

	UFUNCTION()
	void UpdatePlayer1Health(float health);

	UFUNCTION()
	void UpdatePlayer2Health(float health);
};
