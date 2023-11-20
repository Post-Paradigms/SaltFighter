// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighter.h"
#include "FighterController.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Face();
	FString Debug = FString::Printf(TEXT("State: %d"), State);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);

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
	if (State == EFighterState::NEUTRAL || State == EFighterState::DEFENDING || State == EFighterState::JUMPING) {
		if (Value.IsNonZero()) {
			AddMovementInput(FVector::ForwardVector, Value.Get<FVector>().X);
		}
	}
}

void AFighter::JumpEvent(const FInputActionValue &Value)
{
	TakeInInput(8);
	//Jump();
}

void AFighter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
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

		IsLeftSide = (Rot.Yaw >= 180.f);
		// FString Debug = FString::Printf(TEXT("IsLeft: (%d)"), IsLeftSide);
		// GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);


		/* SetControlRotation for smooth turn, SetActorRelativeRotation for instant turn */
		// if (OurController) OurController->SetControlRotation(Rot);
		SetActorRelativeRotation(Rot);
	}
}

void AFighter::TakeInInput(int32 Num) {

	switch (Num) {
		case 1:
		case 4:
		case 7:
			if (Locked) { return; }
			UpdateState(EFighterState::DEFENDING);
			break;
		case 2:
			//crouch
			break;

		case 5:
			if (Locked) { return; }
			UpdateState(EFighterState::NEUTRAL);
			break;

		case 3:
		case 6:
			//walk
			if (Locked) { return; }
			UpdateState(EFighterState::NEUTRAL);
			break;
		case 8:
		case 9:
			if (UpdateState(EFighterState::JUMPING)) {
				Locked = true;
				Jump();
			}
			break;

		case 10:
			if (UpdateState(EFighterState::STARTUP)) {
				PerformNormal(FName(TEXT("LP")));
			}
			break;
	}
}

void AFighter::PerformNormal(FName AttkName) {
	Locked = true;
	CurrAttk = *FighterDataTable->FindRow<FAttackStruct>(AttkName, "Normal");
	FrameTimer = CurrAttk.Startup; //starts the frame timer in tick
}

void AFighter::PerformSpecial(FName SpecialName) {
	//flush the input buffer here
	Locked = true;
	CurrAttk = *FighterDataTable->FindRow<FAttackStruct>(SpecialName, "Special");
	FrameTimer = CurrAttk.Startup; //starts the frame timer in tick
}

bool AFighter::UpdateState(EFighterState NewState) {
	bool valid = false;
	switch (NewState) {
		case EFighterState::NEUTRAL:
			Locked = false;
			CanJumpCancel = false;
			CanSpecialCancel = false;
			valid = true;
			break;

		case EFighterState::DEFENDING:
			valid = (State == EFighterState::NEUTRAL);
			break;

		case EFighterState::JUMPING:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::DEFENDING ||
				(CanJumpCancel && State == EFighterState::ACTIVE) || (CanJumpCancel && State == EFighterState::RECOVERY));
			break;

		case EFighterState::STARTUP:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::DEFENDING || State == EFighterState::JUMPING);
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

	if (valid) {
		State = NewState;
	}
	return valid;
		
}

//used for things that last a certain amount of frames!
//ex. StartUp can last for 5 frames, which the tick function will automatically decrement,
//once FrameTimer hits zero, then it calls this function to advance into the next state!
void AFighter::FrameAdvanceState() {
	FrameTimer = -1; //for safety

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

}

//you got hit dumbass
//pass in attack frame data struct thing
void AFighter::OnOw() {
	if (State == EFighterState::DEFENDING) {
		UpdateState(EFighterState::BLOCKSTUN);
	} else {
		UpdateState(EFighterState::HITSTUN);
	}
}
