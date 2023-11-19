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

void AFighterController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    FString Debug = FString::Printf(TEXT("Num Pad Sector: %d"), PolledInput);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);

    PolledInput = 5;
}

void AFighterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighterController::OnMovePressed);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFighterController::OnJumpPressed);
        EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AFighterController::OnLightAttackPressed);
    }
}

int AFighterController::VectorToNumPadSector(FVector2D Vector)
{
    const TArray<int> NumPadSectors = { 8, 9, 6, 3, 2, 1, 4, 7 };

    float Angle = FMath::Atan2(Vector.X, Vector.Y) + (PI / 8.f);

    if (Angle < 0) Angle += 2 * PI;

    Angle = FMath::RadiansToDegrees(Angle);

    // FString Debug = FString::Printf(TEXT("Angle Degrees: %f"), Angle);
    // if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);

    int Octant = FMath::FloorToInt(Angle / 45);

    return (Vector.Length() >= 0.5) ? NumPadSectors[Octant] : 0;

}

void AFighterController::OnMovePressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->MoveEvent(Value);

    if (Value.IsNonZero())
    {
         
        const FVector2D MovementVector = Value.Get<FVector2D>();

        PolledInput = VectorToNumPadSector(MovementVector);
    }
}

void AFighterController::OnJumpPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->JumpEvent(Value);
}

void AFighterController::OnLightAttackPressed(const FInputActionValue &Value)
{
  /*  AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->LightAttackEvent(Value);*/
}

