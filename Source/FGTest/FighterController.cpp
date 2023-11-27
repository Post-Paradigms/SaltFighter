// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterController.h"
#include "FightGameMode.h"
#include "Fighter.h"

AFighterController::AFighterController()
{
    PolledInput = EInputType::NEUTRAL;
    FramesSinceLastInput = 0;
    
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

    PopulateInputBuffer();  

    /* Uncomment to print our input buffer contents */
    for (int i = 0; i < InputBuffer.Num(); i++)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, UEnum::GetValueAsString(InputBuffer[i]));
    }

    CheckForSequence();

    PolledInput = EInputType::NEUTRAL;
}

void AFighterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighterController::OnMovePressed);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFighterController::OnJumpPressed);
        EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &AFighterController::OnLightAttackPressed);
        EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AFighterController::OnHeavyAttackPressed);

    }
}

EInputType AFighterController::VectorToNumPadSector(FVector2D Vector)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) {
        const TArray<EInputType> NumPadSectors = (player->IsLeftSide) ?  LeftSideNumPad : RightSideNumPad;
        
        float Angle = FMath::Atan2(Vector.X, Vector.Y) + (PI / 8.f);

        if (Angle < 0) Angle += 2 * PI;

        Angle = FMath::RadiansToDegrees(Angle);

        int Octant = FMath::FloorToInt(Angle / 45);

        return (Vector.Length() >= NeutralThreshold) ? NumPadSectors[Octant] : EInputType::NEUTRAL;
    }

    return EInputType::NEUTRAL;
}

void AFighterController::PopulateInputBuffer()
{
    FramesSinceLastInput++;

    HandleInputTimeout();

    if ((!InputBuffer.IsEmpty() && InputBuffer.Last() != PolledInput) || InputBuffer.IsEmpty())
    {
        InputBuffer.Push(PolledInput);
        if (PolledInput != EInputType::NEUTRAL) { // ignore displaying neutral
            if (this == GetWorld()->GetAuthGameMode<AFightGameMode>()->GetPlayer1Controller()) {
                GetWorld()->GetAuthGameMode<AFightGameMode>()->GetFightingHUD()->UpdatePlayer1Buffer(PolledInput);
            }
            else {
                GetWorld()->GetAuthGameMode<AFightGameMode>()->GetFightingHUD()->UpdatePlayer2Buffer(PolledInput);
            }
        }
        FramesSinceLastInput = 0;
    }

    if (!InputBuffer.IsEmpty() && (InputBuffer.Num() > BufferMaxCapacity))
    {
        InputBuffer.RemoveAt(0);
    } 
}


void AFighterController::CheckForSequence() {

    AFighter* player = Cast<AFighter>(this->GetCharacter());

    for (auto& MotionInput : MotionInputs) 
    {
        if (IsSubSequence(MotionInput.Value, 2)) 
        {
            if (player) player->TakeInInput(MotionInput.Key);
            break;
        }
    }

    if (player) player->TakeInInput(PolledInput);
}


void AFighterController::HandleInputTimeout()
{
    if (FramesSinceLastInput >= InputBufferLifespan)
    {
        EInputType LastInput = InputBuffer.Last();
        InputBuffer.Empty();
        InputBuffer.Push(LastInput);
        FramesSinceLastInput = 0;
    } 
}

void AFighterController::FlushBuffer() {
    InputBuffer.Empty();
}

bool AFighterController::IsSubSequence(TArray<EInputType> Sequence, int AdditionalFrameLenience)
{
    int TotalLenience = Sequence.Num() + AdditionalFrameLenience;
    int SeqIndex = 0;

    for (int BufIndex = 0; BufIndex < InputBuffer.Num(); BufIndex++) 
    {
        if (Sequence[0] == InputBuffer[BufIndex])
        {
            SeqIndex = 0;
            for (int Frame = BufIndex; Frame < FMath::Min(TotalLenience + BufIndex, InputBuffer.Num()); Frame++)
            {
                if (Sequence[SeqIndex] == InputBuffer[Frame]) SeqIndex++;

                if (SeqIndex == Sequence.Num()) return true;
            }
        }
    }

    return false;
}

void AFighterController::OnMovePressed(const FInputActionValue &Value)
{
    /* Remove later, handle in TakeInput*/
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->MoveEvent(Value);

    if (Value.IsNonZero())
    {
        PolledInput = VectorToNumPadSector(Value.Get<FVector2D>());
    }
}

void AFighterController::OnJumpPressed(const FInputActionValue &Value)
{
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->JumpEvent(Value);
}

void AFighterController::OnLightAttackPressed(const FInputActionValue &Value)
{
    PolledInput = EInputType::LB;
}

void AFighterController::OnHeavyAttackPressed(const FInputActionValue& Value) {
    PolledInput = EInputType::HB;
}
