// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighter.h"
#include "FighterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsLeftSide = true;

	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
	CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = true;

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
	State = EFighterState::NEUTRAL;
	FrameTimer = -1;
	NumAirDashes = MaxAirDashes;
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Face();
	FString Debug = FString::Printf(TEXT("State: %d"), State);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);
	//FString Debug2 = FString::Printf(TEXT("IsLeft: (%d)"), IsLeftSide);
	//GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug2);

	if (FrameTimer > 0) FrameTimer--;

	if (FrameTimer == 0) {
		FrameAdvanceState();
	}
}

// Called to bind functionality to input
void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighter::MoveEvent(const FInputActionValue &Value)
{
	if (State == EFighterState::NEUTRAL || State == EFighterState::FORWARDING || State == EFighterState::DEFENDING || State == EFighterState::JUMPING) {
		if (Value.IsNonZero() && Value.GetMagnitude() > AFighterController::NeutralThreshold) {
			AddMovementInput(FVector::ForwardVector, Value.Get<FVector>().X);
		}
	}
}

void AFighter::JumpEvent(const FInputActionValue &Value)
{
	TakeInInput(EInputType::UP);
	//Jump();
}

void AFighter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	NumAirDashes = MaxAirDashes;
	UpdateState(EFighterState::NEUTRAL);
}

// Rotate player towards direction of the opponent
void AFighter::Face()
{
	if (OtherPlayer)
	{
		FVector Direction = OtherPlayer->GetActorLocation() - this->GetActorLocation();
		Direction = FVector(Direction.X, Direction.Y, 0.f);
		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		// FString Debug = FString::Printf(TEXT("Actor Rotation: (%f, %f, %f)"), Rot.Pitch, Rot.Yaw, Rot.Roll);
		// GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, Debug);

		IsLeftSide = (Rot.Yaw < 180.f);


		/* SetControlRotation for smooth turn, SetActorRelativeRotation for instant turn */
		// if (OurController) OurController->SetControlRotation(Rot);
		SetActorRelativeRotation(Rot);
	}
}

void AFighter::TakeInInput(EInputType Input) {
	switch (Input) {
		case EInputType::DOWNLEFT:
		case EInputType::LEFT:
			// back
			//left defend, walk right
			if (Locked) { return; }
			if (ValidateState(EFighterState::DEFENDING)) {
				UpdateState(EFighterState::DEFENDING);
			}
			break;
		case EInputType::DOWN:
			//crouch
			break;
		case EInputType::NEUTRAL:
			//neutral/idle
			if (Locked) { return; }
			UpdateState(EFighterState::NEUTRAL);
			break;

		case EInputType::DOWNRIGHT:
		case EInputType::RIGHT:
			// forward
			//right defend, walk left
			if (Locked) { return; }
			if (ValidateState(EFighterState::FORWARDING)) {
				UpdateState(EFighterState::FORWARDING);
			}
			break;

		case EInputType::UPLEFT:
		case EInputType::UP:
			//
		case EInputType::UPRIGHT:
			//jump
			if (ValidateState(EFighterState::JUMPING)) {
				PreviousState = State;
				UpdateState(EFighterState::JUMPING);
				Jump();
			}
			break;

		case EInputType::LB:
			if (ValidateState(EFighterState::STARTUP)) {
				LightNormal(State);
			}
			break;

		case EInputType::FQCL: //this is a test fireball state
			//all specials will have this extra bool check to see if they can special cancel
			//fortunately, the only specials in this game are 236, 214, and maybe 263, so only 3 switch cases! yay
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				//do the special
				GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Red, "i can do firebalss on keyboard");
				LightQuarterCircleForward(State);
			}
	}
}

void AFighter::PerformNormal(FName AttkName) {
	PreviousState = State;
	UpdateState(EFighterState::STARTUP);

	CurrAttk = *FighterDataTable->FindRow<FAttackStruct>(AttkName, "Normal");
	FrameTimer = CurrAttk.Startup; //starts the frame timer in tick

}

void AFighter::PerformSpecial(FName SpecialName) {
	//flush the input buffer here
	OurController->FlushBuffer();
	PreviousState = State;
	CanJumpCancel = false;
	CanSpecialCancel = false;
	UpdateState(EFighterState::STARTUP);

	CurrAttk = *FighterDataTable->FindRow<FAttackStruct>(SpecialName, "Special");
	FrameTimer = CurrAttk.Startup; //starts the frame timer in tick
}

void AFighter::PerformDash() {
	PreviousState = State;

	if (State == EFighterState::JUMPING) {
		//airdash state
		NumAirDashes--;
		UpdateState(EFighterState::AIRDASHING);
		CurrAttk = *FighterDataTable->FindRow<FAttackStruct>("AirDash", "AirDash");
		FrameTimer = CurrAttk.Startup; //starts the frame timer in tick
	} else {
		//dash state
		UpdateState(EFighterState::DASHING);
		CurrAttk = *FighterDataTable->FindRow<FAttackStruct>("Dash", "Dash");
		FrameTimer = CurrAttk.Startup; //starts the frame timer in tick
	}// do else if for backdash in the future
}

// Validates and changes state
bool AFighter::ValidateState(EFighterState NewState) {
	bool valid = false;
	switch (NewState) {
		case EFighterState::NEUTRAL:
			valid = true;
			break;

		case EFighterState::FORWARDING:
			valid = true;
			break;

		case EFighterState::DASHING:
			//i added the neutral case for when we add a dash macro
			valid = (State == EFighterState::FORWARDING || State == EFighterState::NEUTRAL);
			break;

		case EFighterState::DEFENDING:
			valid = (State != EFighterState::JUMPING);
			break;

		// might be jank ;m;
		case EFighterState::JUMPING:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::FORWARDING || State == EFighterState::DEFENDING || GetCharacterMovement()->IsFalling() ||
				(CanJumpCancel && State == EFighterState::ACTIVE) || (CanJumpCancel && State == EFighterState::RECOVERY));
			break;

		case EFighterState::AIRDASHING:
			valid = (State == EFighterState::JUMPING && NumAirDashes > 0);
			break;

		case EFighterState::STARTUP:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::DEFENDING || State == EFighterState::JUMPING || State == EFighterState::FORWARDING);
			break;

		case EFighterState::ACTIVE:
			valid = (State == EFighterState::STARTUP);
			break;

		case EFighterState::RECOVERY:
			valid = (State == EFighterState::ACTIVE);
			break;

		case EFighterState::HITSTUN:
		case EFighterState::BLOCKSTUN:
			valid = true;
			break;
	}
	
	return valid;
		
}

//what do you spaghetti
void AFighter::UpdateState(EFighterState NewState) {
	switch (NewState) {
		//dab
		case EFighterState::NEUTRAL:
		case EFighterState::DEFENDING:
		case EFighterState::FORWARDING:
			Locked = false;
			CanJumpCancel = false;
			CanSpecialCancel = false;
			break;

		case EFighterState::KNOCKDOWN:
		case EFighterState::JUMPING:
		case EFighterState::STARTUP:
		case EFighterState::DASHING:
		case EFighterState::AIRDASHING:
			Locked = true;
			break;
	}
	State = NewState;
}

//used for things that last a certain amount of frames!
//ex. StartUp can last for 5 frames, which the tick function will automatically decrement,
//once FrameTimer hits zero, then it calls this function to advance into the next state!
void AFighter::FrameAdvanceState() {
	FrameTimer = -1; //for safety
	//GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Red, "frame advance state");
	//oh yeah baby, more switches
	switch (State) {
		case EFighterState::STARTUP:
			UpdateState(EFighterState::ACTIVE);
			FrameTimer = CurrAttk.Active;
			break;
		case EFighterState::ACTIVE:
			UpdateState(EFighterState::RECOVERY);
			FrameTimer = CurrAttk.Recovery;
			break;
		case EFighterState::RECOVERY:
			UpdateState(PreviousState);
			break;

		case EFighterState::HITSTUN:
			(PreviousState == EFighterState::JUMPING) ? UpdateState(EFighterState::JUMPING) : UpdateState(EFighterState::NEUTRAL);
			break;

		case EFighterState::BLOCKSTUN:
			UpdateState(EFighterState::NEUTRAL);
			OurController->CheckForSequence();
			break;

		//we're going to need to read from the input buffer the most recent input and update to that state
		case EFighterState::KNOCKDOWN:
			UpdateState(EFighterState::NEUTRAL);
			OurController->CheckForSequence();
			break;

		case EFighterState::AIRDASHING:
			UpdateState(EFighterState::JUMPING);
			break;

		case EFighterState::DASHING:
			UpdateState(EFighterState::NEUTRAL);
			break;
	}
}

//you hit the other person
//pass in attack frame data struct thing
void AFighter::OnHitOther() {
	//we're also going to need to know if they're blocking or if they're getting hit 
	//to handle appropriate plus/minus frames and valid combo counting
	CanJumpCancel = CurrAttk.JumpCancellable;
	CanSpecialCancel = CurrAttk.SpecialCancellable;
}

//you got hit dumbass
//pass in attack frame data struct thing
void AFighter::OnOw(FAttackStruct OwCauser) {
	if (State == EFighterState::DEFENDING || OtherPlayer->State == EFighterState::BLOCKSTUN) {
		//blocking
		UpdateState(EFighterState::BLOCKSTUN);
		FrameTimer = OwCauser.Blockstun;
	} else {
		//i didn't pay 60 bucks to block
		if (OwCauser.Knockdown) {
			UpdateState(EFighterState::KNOCKDOWN);
			FrameTimer = 60; //this has to be a consistent number across the entire cast
		} else {
			PreviousState = State;
			UpdateState(EFighterState::HITSTUN);
			FrameTimer = OwCauser.Hitstun;
		}
	}
}


// === FIGHTER MOVE FUNCTIONS ===

void AFighter::LightNormal(EFighterState CurrentState) {
	// string var name 
	GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Blue, "mrow");
	FName AttkName = "";
	if (CurrentState == EFighterState::JUMPING) {
		AttkName = "LightJump";
	}
	else {
		AttkName = "LightAttk";
	}
	PerformNormal(AttkName);
}

void AFighter::HeavyNormal(EFighterState CurrentState) {
	FName AttkName = "";
	if (CurrentState == EFighterState::JUMPING) {
		AttkName = "HeavyJump";
	}
	else {
		AttkName = "HeavyAttk";
	}
	PerformNormal(AttkName);
}

void AFighter::LightQuarterCircleForward(EFighterState CurrentState) {
	//we're not gonna have any jumping specials for now
	//so i don't need be like
	FName SpecialName = "LightFQC";
	PerformSpecial(SpecialName);
}
