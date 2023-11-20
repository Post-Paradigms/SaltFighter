// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FightingHUDUserWidget.h"

AFightGameMode::AFightGameMode()
{
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_FighterController"));
    if (PlayerControllerBPClass.Class)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
}

void AFightGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Create a player for each PlayerStart object
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        if (i == 0) { // Setup player 1
            P1FighterController = Cast<AFighterController>(UGameplayStatics::CreatePlayer(GetWorld()));
            if (FighterBPClass)
                P1FighterCharacter = GetWorld()->SpawnActor<AFighter>(FighterBPClass, PlayerStarts[i]->GetActorTransform());
            if (P1FighterController && P1FighterCharacter)
                P1FighterController->Possess(P1FighterCharacter);
        }
        else if (i == 1) { // Setup player 2
            P2FighterController = Cast<AFighterController>(UGameplayStatics::CreatePlayer(GetWorld()));
            if (FighterBPClass)
                P2FighterCharacter = GetWorld()->SpawnActor<AFighter>(FighterBPClass, PlayerStarts[i]->GetActorTransform());
            if (P2FighterController && P2FighterCharacter)
                P2FighterController->Possess(P2FighterCharacter);
        }
        else {
            // umm, need i explain?
        }
    }

    // Both players done setup, set other player and controller references
    if (P1FighterCharacter && P2FighterCharacter)
    {
        P1FighterCharacter->OtherPlayer = P2FighterCharacter;
        P1FighterCharacter->OurController = P1FighterController;

        P2FighterCharacter->OtherPlayer = P1FighterCharacter;
        P2FighterCharacter->OurController = P2FighterController;
    }
    else {
        // Something is wrong!!!!
    }

    // Create HUD
    if (FightingHUDClass)
    {
        FightingHUD = CreateWidget<UFightingHUDUserWidget>(P1FighterController, FightingHUDClass);
        FightingHUD->AddToPlayerScreen();
    }

}

void AFightGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Remove HUD
    if (FightingHUD)
    {
        FightingHUD->RemoveFromParent();
        FightingHUD = nullptr;
    }

}
