// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "MainMenuGameMode.h"
#include "MainMenuUserWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{

}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMainMenuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(NavigationAction, ETriggerEvent::Started, this, &AMainMenuPlayerController::OnNavigatePressed);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &AMainMenuPlayerController::OnSelectPressed);
    }

}

void AMainMenuPlayerController::OnNavigatePressed(const FInputActionValue& Value)
{
	UMainMenuUserWidget* MainMenuWidget = GetWorld()->GetAuthGameMode<AMainMenuGameMode>()->GetMainMenuWidget();
	float navigationdir = Value.Get<float>();
	if (navigationdir > 0 && MainMenuWidget->NavigationIndex > 0)
	{
		MainMenuWidget->NavigationIndex--;
		MainMenuWidget->UpdateButtonState();
	}
	else if (navigationdir < 0 && MainMenuWidget->NavigationIndex < 2)
	{
		MainMenuWidget->NavigationIndex++;
		MainMenuWidget->UpdateButtonState();
	}
}

void AMainMenuPlayerController::OnSelectPressed()
{
	GetWorld()->GetAuthGameMode<AMainMenuGameMode>()->GetMainMenuWidget()->Select();
}
