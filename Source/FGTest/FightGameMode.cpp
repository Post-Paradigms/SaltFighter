// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameMode.h"
#include "FightGameState.h"
#include "FightPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FightingHUDUserWidget.h"

// Sets default values
AFightGameMode::AFightGameMode()
{
    // Game State
    static ConstructorHelpers::FClassFinder<AGameState> GameStateBPClass(TEXT("/Game/Blueprints/Framework/Fight/BP_FightGameState"));
    if (GameStateBPClass.Class)
    {
        GameStateClass = GameStateBPClass.Class;
    }

    // Player State
    static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Game/Blueprints/Framework/Fight/BP_FightPlayerState"));
    if (PlayerStateBPClass.Class)
    {
        PlayerStateClass = PlayerStateBPClass.Class;
    }

    // Player Controller
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_FighterController"));
    if (PlayerControllerBPClass.Class)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
}

// Called when the game starts or when spawned
void AFightGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Create a player for each PlayerStart object
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        if (i == 0) { // Setup player 1
            if (FighterBPClass)
                P1FighterCharacter = GetWorld()->SpawnActor<AFighter>(FighterBPClass, PlayerStarts[i]->GetActorTransform());

            if (GetWorld()->GetNumPlayerControllers() == 0)
                UGameplayStatics::CreatePlayer(GetWorld(), 0);
            P1FighterController = Cast<AFighterController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));

            if (P1FighterController && P1FighterCharacter)
                P1FighterController->Possess(P1FighterCharacter);
        }
        else if (i == 1) { // Setup player 2
            if (FighterBPClass)
                P2FighterCharacter = GetWorld()->SpawnActor<AFighter>(FighterBPClass, PlayerStarts[i]->GetActorTransform());

            if (GetWorld()->GetNumPlayerControllers() == 1)
                UGameplayStatics::CreatePlayer(GetWorld(), 1);
            P2FighterController = Cast<AFighterController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 1));

            if (P2FighterController && P2FighterCharacter)
                P2FighterController->Possess(P2FighterCharacter);
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

    // Create HUD
    if (FightingHUDClass && P1FighterController)
    {
        FightingHUD = CreateWidget<UFightingHUDUserWidget>(P1FighterController, FightingHUDClass);
        FightingHUD->AddToPlayerScreen();
    }
}

// Called when the game ends or when removed from level
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
