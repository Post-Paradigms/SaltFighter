// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterController.h"
#include "Fighter.h"

AFighterController::AFighterController()
{
    PolledInput = NeutralInput;
    BufferMaxCapacity = 16;
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

    PopulateInputBuffer();  

    /* Uncomment to print our input buffer contents */
    // for (int i = 0; i < InputBuffer.Num(); i++)
    // {
    //     Debug = FString::Printf(TEXT("InputBuffer[%d]: %d"), i, InputBuffer[i]);
    //     if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);
    // }

    CheckForSequence();

    PolledInput = NeutralInput;
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
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) {
        const TArray<int> NumPadSectors = (player->IsLeftSide) ?  LeftSideNumPad : RightSideNumPad;
        
        float Angle = FMath::Atan2(Vector.X, Vector.Y) + (PI / 8.f);

        if (Angle < 0) Angle += 2 * PI;

        Angle = FMath::RadiansToDegrees(Angle);

        int Octant = FMath::FloorToInt(Angle / 45);

        return (Vector.Length() >= NeutralThreshold) ? NumPadSectors[Octant] : NeutralInput;
    }

    return NeutralInput;
}

void AFighterController::PopulateInputBuffer()
{
    if ((!InputBuffer.IsEmpty() && InputBuffer.Last() != PolledInput) || InputBuffer.IsEmpty()) InputBuffer.Push(PolledInput);

    if (!InputBuffer.IsEmpty() && (InputBuffer.Num() > BufferMaxCapacity))
    {
        // if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, TEXT("Popped array"));
        InputBuffer.RemoveAt(0);
    } 
}

void AFighterController::CheckForSequence()
{
    /* TODO: Move into frame data table */
    TArray<TArray<int>> MoveSet = {{2, 3, 6, 10}, {6, 10}};

    for (int i = 0; i < MoveSet.Num(); i++)
    {
        if (IsSubSequence(MoveSet[i], 2))
        {
            /* TODO: Replace with call to TakeInput and pass in Input Type */
            if (i == 0) OnFireballPressed();

            InputBuffer.Empty();
            break;
        }
    }
}

bool AFighterController::IsSubSequence(TArray<int> Sequence, int Lenience)
{
    int SeqIndex = 0;
    int BufIndex = 0;

    while (SeqIndex < Sequence.Num())
    {
        if (BufIndex == InputBuffer.Num())
        {
            return false;
        }
        else if (Sequence[SeqIndex] == InputBuffer[BufIndex])
        {
            SeqIndex++, BufIndex++;
        }
        else {
            BufIndex++;
        }
    }

    return true;
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
    PolledInput = 10;
    AFighter* player = Cast<AFighter>(this->GetCharacter());
    if (player) player->TakeInInput(PolledInput);
}

void AFighterController::OnFireballPressed()
{
    // AFighter* player = Cast<AFighter>(this->GetCharacter());
    // if (player) player->FireballEvent();

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("*Fireball*"));
}
