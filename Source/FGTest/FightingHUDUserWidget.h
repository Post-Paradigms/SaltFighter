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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RoundTimer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player1Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player2Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* Player1Buffer;

	UFUNCTION()
	void UpdateRoundTimer(int time);

	UFUNCTION()
	void UpdatePlayer1Health(int health);

	UFUNCTION()
	void UpdatePlayer2Health(int health);

	UFUNCTION()
	void UpdatePlayer1Buffer(EInputType input);

private:
	TArray<EInputType> P1Buffer;

};
