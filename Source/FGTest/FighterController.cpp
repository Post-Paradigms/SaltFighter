// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterController.h"
#include "Fighter.h"

AFighterController::AFighterController()
{

}

void AFighterController::BeginPlay()
{
    Super::BeginPlay();
    
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AFighterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &AFighterController::OnTestPressed);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighterController::OnMovePressed);
        EnhancedInputComponent->BindAction(VectorMoveAction, ETriggerEvent::Triggered, this, &AFighterController::OnVectorMovePressed);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFighterController::OnJumpPressed);
        EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &AFighterController::OnUpPressed);
        EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AFighterController::OnDownPressed);
        EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AFighterController::OnLeftPressed);
        EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AFighterController::OnRightPressed);
        EnhancedInputComponent->BindAction(DownRightAction, ETriggerEvent::Triggered, this, &AFighterController::OnDownRightPressed);
        EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AFighterController::OnLightAttackPressed);
        EnhancedInputComponent->BindAction(FireballAction, ETriggerEvent::Triggered, this, &AFighterController::OnFireballPressed);
        EnhancedInputComponent->BindAction(CommandNormalLightAction, ETriggerEvent::Triggered, this, &AFighterController::OnCommandNormalLightPressed);
    }
}

void AFighterController::OnTestPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->TestEvent(Value);
}

void AFighterController::OnMovePressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->MoveEvent(Value);
}

void AFighterController::OnVectorMovePressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->VectorMoveEvent(Value);
}

void AFighterController::OnJumpPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->JumpEvent(Value);
}

void AFighterController::OnUpPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->UpEvent(Value);
}

void AFighterController::OnDownPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->DownEvent(Value);
}

void AFighterController::OnLeftPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->LeftEvent(Value);
}

void AFighterController::OnRightPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->RightEvent(Value);
}

void AFighterController::OnDownRightPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->DownRightEvent(Value);
}

void AFighterController::OnLightAttackPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->LightAttackEvent(Value);
}

void AFighterController::OnFireballPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->FireballEvent(Value);
}

void AFighterController::OnCommandNormalLightPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->CommandNormalLightEvent(Value);
}
