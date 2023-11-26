// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingHUDUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Called to update the round timer display. Should be called once every second?
void UFightingHUDUserWidget::UpdateRoundTimer(int time)
{
	RoundTimer->SetText(FText::AsNumber(time));
}

// Called to update player 1's healthbar when their health changes.
void UFightingHUDUserWidget::UpdatePlayer1Health(int health)
{
	Player1Health->SetPercent(health / 100.0f);
}

// Called to update player 2's healthbar when their health changes.
void UFightingHUDUserWidget::UpdatePlayer2Health(int health)
{
	Player2Health->SetPercent(health / 100.0f);
}

// Called to update player 1's input buffer
void UFightingHUDUserWidget::UpdatePlayer1Buffer(EInputType input)
{
	Player1Buffer->ClearChildren();
	if (input != EInputType::NEUTRAL) { // temp fix for displaying buffer without neutral input duplication
		P1Buffer.Push(input);
		if (P1Buffer.Num() == 18) {
			P1Buffer.RemoveAt(0);
		}
	}
	for (int i = P1Buffer.Num() - 1; i >= 0; --i)
	{
		UImage* image = NewObject<UImage>();
		Player1Buffer->AddChild(image);
		image->Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(InputTextures[(int)P1Buffer[i]], 64, 64);
		if ((int)P1Buffer[i] < 8) {
			image->SetBrushTintColor(FSlateColor(FColor::Green));
		}
		else {
			image->SetBrushTintColor(FSlateColor(FColor::Red));
		}
	}
}
