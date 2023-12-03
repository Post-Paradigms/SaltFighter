// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameMode.h"
#include "FightGameState.h"
#include "FightPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

            //only need to add mapping context to player 2 for some reason
            if (P2FighterController) {
                if (UEnhancedInputLocalPlayerSubsystem* InputSystem = P2FighterController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
                    InputSystem->AddMappingContext(InputMapping, 0);
                }
            }

            if (P2FighterController && P2FighterCharacter) {
                P2FighterController->Possess(P2FighterCharacter);
            }

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

    P1FighterController->DisableInput(P1FighterController);
    P2FighterController->DisableInput(P2FighterController);

    StartRoundCountdownTimerCount = 0;
    GetWorld()->GetTimerManager().SetTimer(StartRoundCountdownTimer, this, &AFightGameMode::StartRoundCountdown, 1.0f, true);
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

// Reset the map and players for the next round
void AFightGameMode::ResetRound()
{
    
    P1FighterController->DisableInput(P1FighterController);
    P2FighterController->DisableInput(P2FighterController);

    // Create a player for each PlayerStart object
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        if (i == 0)
            P1FighterCharacter->SetActorLocation(PlayerStarts[i]->GetActorLocation());
        if (i == 1)
            P2FighterCharacter->SetActorLocation(PlayerStarts[i]->GetActorLocation());
    }

    FightingHUD->UpdatePlayer1Health(P1FighterCharacter->GetPlayerState<AFightPlayerState>()->PlayerHealth = 100);
    FightingHUD->UpdatePlayer2Health(P2FighterCharacter->GetPlayerState<AFightPlayerState>()->PlayerHealth = 100);

    GetGameState<AFightGameState>()->RoundNumber++;
    GetGameState<AFightGameState>()->RoundTimer = 100.0f;

    GetWorld()->GetTimerManager().SetTimer(StartRoundCountdownTimer, this, &AFightGameMode::StartRoundCountdown, 1.0f, true);
}

// Damage a player and update the hud
void AFightGameMode::DamagePlayer(AFighter* Fighter, int Damage)
{
    Fighter->GetPlayerState<AFightPlayerState>()->PlayerHealth -= Damage;

    if (Fighter->GetController() == P1FighterController) // update player 1 healthbar
    {
        FightingHUD->UpdatePlayer1Health(Fighter->GetPlayerState<AFightPlayerState>()->PlayerHealth);
        if (Fighter->GetPlayerState<AFightPlayerState>()->PlayerHealth <= 0)
        {
            ResetRound();
            FightingHUD->AddPlayer2Win();
            if (GetGameState<AFightGameState>()->Player2Wins++ == 2) {
                UGameplayStatics::OpenLevel(this, "MainMenuTestMap");
            }
        }
    }
    else { // update player 2 health bar
        FightingHUD->UpdatePlayer2Health(Fighter->GetPlayerState<AFightPlayerState>()->PlayerHealth);
        if (Fighter->GetPlayerState<AFightPlayerState>()->PlayerHealth <= 0)
        {
            ResetRound();
            FightingHUD->AddPlayer1Win();
            if (GetGameState<AFightGameState>()->Player1Wins++ == 2) {
                UGameplayStatics::OpenLevel(this, "MainMenuTestMap");
            }
        }
    }
}

void AFightGameMode::StartRoundCountdown()
{
    switch (StartRoundCountdownTimerCount) {
        case 0:
            FightingHUD->UpdateCountdown(FText::FromString("3"));
            break;
        case 1:
            FightingHUD->UpdateCountdown(FText::FromString("2"));
            break;
        case 2:
            FightingHUD->UpdateCountdown(FText::FromString("1"));
            break;
        case 3:
            FightingHUD->UpdateCountdown(FText::FromString("COOK"));
            break;
        case 4:
            FightingHUD->UpdateCountdown(FText::FromString(""));

            P1FighterController->EnableInput(P1FighterController);
            P2FighterController->EnableInput(P2FighterController);

            GetWorld()->GetTimerManager().ClearTimer(StartRoundCountdownTimer);
            StartRoundCountdownTimerCount = 0;
            return;
            break;

    }
    StartRoundCountdownTimerCount++;
}

// Fighting HUD Getter
UFightingHUDUserWidget* AFightGameMode::GetFightingHUD() { return FightingHUD; }

AFighterController* AFightGameMode::GetPlayer1Controller() { return P1FighterController; }

AFighterController* AFightGameMode::GetPlayer2Controller() { return P2FighterController; }
