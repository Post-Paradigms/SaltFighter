// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FighterController.h"
#include "FightingHUDUserWidget.generated.h"

/**
 * Displays player health and round timer.
 */
UCLASS()
class FGTEST_API UFightingHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> InputTextures;

	UPROPERTY(EditAnywhere)
	UTexture2D* WinTexture;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RoundTimer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player1Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player2Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* Player1Buffer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* Player2Buffer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1Combo;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2Combo;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Countdown;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* Player1Wins;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* Player2Wins;

	UFUNCTION()
	void UpdateRoundTimer(int time);

	UFUNCTION()
	void UpdatePlayer1Health(int health);

	UFUNCTION()
	void UpdatePlayer2Health(int health);

	UFUNCTION()
	void UpdatePlayer1Buffer(EInputType input);

	UFUNCTION()
	void UpdatePlayer2Buffer(EInputType input);

	UFUNCTION()
	void UpdateCombo(int combo, AFighter* comboer);

	UFUNCTION()
	void UpdateCountdown(FText text);

	UFUNCTION()
	void AddPlayer1Win();

	UFUNCTION()
	void AddPlayer2Win();

	UFUNCTION()
	void StartRoundTimer();

	UFUNCTION()
	void PauseRoundTimer();

private:
	TArray<EInputType> P1Buffer;

	TArray<EInputType> P2Buffer;

public:
	bool AllowCountdown;

};
