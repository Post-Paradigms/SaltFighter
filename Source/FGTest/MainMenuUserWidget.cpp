// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (UWidget* widget: ButtonVBox->GetAllChildren())
	{
		NavigationItems.Add(Cast<UButton>(widget));
	}

	PlayButton->OnHovered.AddDynamic(this, &UMainMenuUserWidget::SetPlayIndex);
	CreditButton->OnHovered.AddDynamic(this, &UMainMenuUserWidget::SetCreditIndex);
	QuitButton->OnHovered.AddDynamic(this, &UMainMenuUserWidget::SetQuitIndex);

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnPlayClicked);
	CreditButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnCreditClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitClicked);

	UpdateButtonState();
}

void UMainMenuUserWidget::Select()
{
	if (InCredits) {
		CreditUW->SetVisibility(ESlateVisibility::Hidden);
		SetVisibility(ESlateVisibility::Visible);
		NavigationIndex = 0;
		UpdateButtonState();
		InCredits = false;
		return;
	}
	switch (NavigationIndex) {
	case 0:
		OnPlayClicked();
		break;
	case 1:
		OnCreditClicked();
		break;
	case 2:
		OnQuitClicked();
		break;
	}
}

void UMainMenuUserWidget::UpdateButtonState()
{
	int index = 0;
	for (UButton* button : NavigationItems) {
		if (index == NavigationIndex)
		{
			button->SetBackgroundColor(FLinearColor::White);
		}
		else {
			button->SetBackgroundColor(FLinearColor::Black);
		}
		++index;
	}
}

void UMainMenuUserWidget::OnPlayClicked()
{
	UGameplayStatics::OpenLevel(this, "TestMap");
}

void UMainMenuUserWidget::OnCreditClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	CreditUW->SetVisibility(ESlateVisibility::Visible);
	InCredits = true;
}

void UMainMenuUserWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

// This would be easier in blueprint but too bad, blueprint cringe
void UMainMenuUserWidget::SetPlayIndex() { NavigationIndex = 0; UpdateButtonState(); };
void UMainMenuUserWidget::SetCreditIndex() { NavigationIndex = 1; UpdateButtonState(); };
void UMainMenuUserWidget::SetQuitIndex() { NavigationIndex = 2; UpdateButtonState(); };

