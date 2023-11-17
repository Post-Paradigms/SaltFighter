// Copyright Epic Games, Inc. All Rights Reserved.


#include "FGTestGameModeBase.h"
#include "Fighter.h"
#include "FighterController.h"
#include "UObject/ConstructorHelpers.h"

AFGTestGameModeBase::AFGTestGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_FighterController"));
    if (PlayerControllerBPClass.Class)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
}