// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

    // Create a player start for each PlayerStart object
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        if (i == 0) {
            P1FighterController = Cast<AFighterController>(UGameplayStatics::CreatePlayer(GetWorld()));

            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(P1FighterController->GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }

        }
        else if (i == 1) {
            P2FighterController = Cast<AFighterController>(UGameplayStatics::CreatePlayer(GetWorld()));

            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(P1FighterController->GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }

        }
        else {
            // umm, need i explain?
        }
    }


}

void AFightGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);


}
