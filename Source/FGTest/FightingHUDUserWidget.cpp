// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingHUDUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

// Called to update the round timer display. Should be called once every second?
void UFightingHUDUserWidget::UpdateRoundTimer(int time)
{
	RoundTimer->SetText(FText::AsNumber(time));
}

// Called to update player 1's healthbar when their health changes.
void UFightingHUDUserWidget::UpdatePlayer1Health(float health)
{
	Player1Health->Percent = health / 100.0f;
}

// Called to update player 2's healthbar when their health changes.
void UFightingHUDUserWidget::UpdatePlayer2Health(float health)
{
	Player2Health->Percent = health / 100.0f;
}
