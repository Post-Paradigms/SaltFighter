// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenuUserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	// TODO: Set default player controller
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    // Create User Widget
    if (MainMenuUWClass)
    {
        MainMenuUW = CreateWidget<UMainMenuUserWidget>(GetWorld()->GetFirstPlayerController(), MainMenuUWClass);
        MainMenuUW->AddToPlayerScreen();
    }
}

void AMainMenuGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Remove User Widget
    if (MainMenuUW)
    {
        MainMenuUW->RemoveFromParent();
        MainMenuUW = nullptr;
    }
}
