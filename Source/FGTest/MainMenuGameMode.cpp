// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "MainMenuUserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    // Player Controller
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_MainMenuPlayerController"));
    if (PlayerControllerBPClass.Class)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
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

    if (CreditUWClass) {
        CreditUW = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), CreditUWClass);
        MainMenuUW->CreditUW = CreditUW;
        CreditUW->AddToPlayerScreen();
        CreditUW->SetVisibility(ESlateVisibility::Hidden);
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

UMainMenuUserWidget* AMainMenuGameMode::GetMainMenuWidget() { return MainMenuUW; }
