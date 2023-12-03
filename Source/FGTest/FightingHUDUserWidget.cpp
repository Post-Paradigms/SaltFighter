// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingHUDUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Fighter.h"
#include "FightGameMode.h"

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
	P1Buffer.Push(input);
	if (P1Buffer.Num() == 18) {
		P1Buffer.RemoveAt(0);
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

// Called to update player 2's input buffer
void UFightingHUDUserWidget::UpdatePlayer2Buffer(EInputType input)
{
	Player2Buffer->ClearChildren();
	P2Buffer.Push(input);
	if (P2Buffer.Num() == 18) {
		P2Buffer.RemoveAt(0);
	}
	for (int i = P2Buffer.Num() - 1; i >= 0; --i)
	{
		UImage* image = NewObject<UImage>();
		Player2Buffer->AddChild(image);
		image->Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(InputTextures[(int)P2Buffer[i]], 64, 64);
		if ((int)P2Buffer[i] < 8) {
			image->SetBrushTintColor(FSlateColor(FColor::Green));
		}
		else {
			image->SetBrushTintColor(FSlateColor(FColor::Red));
		}
	}
}

void UFightingHUDUserWidget::UpdateCombo(int combo, AFighter* comboer)
{
	if (comboer->GetController<AFighterController>() == GetWorld()->GetAuthGameMode<AFightGameMode>()->GetPlayer1Controller())
	{
		Player1Combo->SetText(FText::FromString(FString::FormatAsNumber(combo) + " HITS"));
	}
	else {
		Player2Combo->SetText(FText::FromString(FString::FormatAsNumber(combo) + " HITS"));
	}
}

void UFightingHUDUserWidget::UpdateCountdown(FText text) {
	Countdown->SetText(text);
}
