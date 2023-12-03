// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighter.h"
#include "FighterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Hurtbox.h"
#include "Hitbox.h"
#include "FightGameMode.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsLeftSide = true;
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->GravityScale = 2.5f;

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
	NumJumps = MaxJumps;

	// Hurtbox
	FVector SpawnLocation = GetMesh()->GetRelativeLocation() + FVector(0.f, 0.f, 90.f);
	FActorSpawnParameters SpawnInfo;
	AHurtbox* PlayerHurtbox = GetWorld()->SpawnActor<AHurtbox>(AHurtbox::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	if (PlayerHurtbox)
	{
		PlayerHurtbox->HurtboxOwner = this;
		PlayerHurtbox->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	} 

	AnimInstance = GetMesh()->GetAnimInstance();
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

void AFighter::Jump(EInputType Input)
{
	if (JumpDirections.Contains(Input))
	{
		PreviousState = State;
		NumJumps--;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		UpdateState(EFighterState::JUMPING);
		FVector FacingSide = FVector((IsLeftSide) ? 1 : -1, 1, 1);
		FVector Direction = FVector(JumpDirections[Input], 1, 1);
		LaunchCharacter(JumpVector * Direction * FacingSide, true, true);
	}
}

void AFighter::MoveEvent(const FInputActionValue &Value)
{
	if (State == EFighterState::NEUTRAL || State == EFighterState::FORWARDING || State == EFighterState::DEFENDING || State == EFighterState::JUMPING) {
		if (Value.IsNonZero() && Value.GetMagnitude() > AFighterController::NeutralThreshold) {
			AddMovementInput(FVector::ForwardVector, Value.Get<FVector>().X);
		}
	}
}

void AFighter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	NumAirDashes = MaxAirDashes;
	NumJumps = MaxJumps;
	if (ActiveHitbox) {
		ActiveHitbox->Destroy();
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	//if (AnimInstance && AnimInstance->Montage_IsPlaying(NULL)) {
	//	StopAnimMontage(nullptr);
	//}
	StopMontage();

	UpdateState(EFighterState::NEUTRAL);
}

// Rotate player towards direction of the opponent
void AFighter::Face()
{
	if (OtherPlayer && 
		State != EFighterState::STARTUP && State != EFighterState::ACTIVE && State != EFighterState::RECOVERY)
	{
		FVector Direction = OtherPlayer->GetActorLocation() - this->GetActorLocation();
		Direction = FVector(Direction.X, Direction.Y, 0.f);
		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		// FString Debug = FString::Printf(TEXT("Actor Rotation: (%f, %f, %f)"), Rot.Pitch, Rot.Yaw, Rot.Roll);
		// GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, Debug);

		IsLeftSide = (Rot.Yaw < 90.f);


		/* SetControlRotation for smooth turn, SetActorRelativeRotation for instant turn */
	/*	 if (OurController) OurController->SetControlRotation(Rot);
		SetActorRelativeRotation(Rot);*/
	}
}

/*
* The main bad boy of the state machine. Updates state given input from the input buffer.
* 
* **PARAMETERS**
* Input represents...well the input. That input can be the analog/dpad, button presses, as well as dashes and special inputs.
* 
* Quick note about the *Locked* boolean. When locked is true, it will essentially lock out updating the state to defending, neutral, and forwarding.
* Things that set locked to true are Jumping, Attacking, Dashing, Knockdown, and the Stuns.
*/
void AFighter::TakeInInput(EInputType Input) {
	switch (Input) {
		case EInputType::LEFT:
			// defending
			if (Locked) { return; }
			if (ValidateState(EFighterState::DEFENDING)) {
				UpdateState(EFighterState::DEFENDING);
			}
			break;

		case EInputType::DOWNLEFT:
			//crouch block
			if (Locked) { return; }
			if (ValidateState(EFighterState::CROUCHBLOCKING)) {
				UpdateState(EFighterState::CROUCHBLOCKING);
			}
			break;

		case EInputType::DOWNRIGHT:
		case EInputType::DOWN:
			//crouch
			if (Locked) { return; }
			if (ValidateState(EFighterState::CROUCHING)) {
				UpdateState(EFighterState::CROUCHING);
			}
			break;

		case EInputType::NEUTRAL:
			//neutral/idle
			if (Locked) { return; }
			UpdateState(EFighterState::NEUTRAL);
			break;

		case EInputType::RIGHT:
			// forward
			if (Locked) { return; }
			if (ValidateState(EFighterState::FORWARDING)) {
				UpdateState(EFighterState::FORWARDING);
			}
			break;

		case EInputType::UPLEFT:
		case EInputType::UP:
		case EInputType::UPRIGHT:
			//jump
			if (ValidateState(EFighterState::JUMPING) && NumJumps > 0) {
				Jump(Input);
			}
			break;

		//dash cases

		case EInputType::DASH:
			if (ValidateState(EFighterState::DASHING) || ValidateState(EFighterState::AIRDASHING)) {
				PerformDash(false);
			}
			break;

		case EInputType::BACKDASH:
			if (ValidateState(EFighterState::DASHING) || ValidateState(EFighterState::AIRDASHING)) {
				PerformDash(true);
			}
			break;
		//start of all the attack cases

		case EInputType::LB:
			if (ValidateState(EFighterState::STARTUP)) {
				LightNormal(false);
			} else if ((CanTargetCombo && Input == NextTargetInput)) {
				LightNormal(true);
			}
			break;

		case EInputType::HB:
			if (ValidateState(EFighterState::STARTUP)) {
				HeavyNormal(false);
			}
			else if ((CanTargetCombo && Input == NextTargetInput)) {
				HeavyNormal(true);
			}
			break;

		case EInputType::FQCL: //this is a test fireball state			pee pee poo poo
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				//do the special
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Light Fireball");
				LightQuarterCircleForward();
			}
			break;

		case EInputType::FQCH:
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Heavy Fireball");
				HeavyQuarterCircleForward();
			}
			break;

		case EInputType::BQCL:
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Light Tatsu");
				LightQuarterCircleBack();
			}
			break;

		case EInputType::BQCH:
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Heavy Tatsu");
				HeavyQuarterCircleBack();
			}
			break;

		case EInputType::DPL:
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Light DP");
				LightDragonPunch();
			}
			break;

		case EInputType::DPH:
			if (ValidateState(EFighterState::STARTUP) || CanSpecialCancel) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Heavy DP");
				HeavyDragonPunch();
			}
			break;

	}
}

/*
	Starts a normal attack
	**PARAMETERS**
	AttkName is the name of the attack that will be performed.

	So what's a normal? A normal is any attack that does not require a special sequence to be performed.
	For example, just pressing Light or Heavy, is considered a normal.
	Even if you are standing, crouching, or jumping, if it does not require a special sequence (ex. 236/Forward Scoop),
	it is a normal.

	*What about 6P (Forward Light)?*
	That's something we call a command normal. Doesn't require a sequence, but it requires a direction. Common directions for
	command normals are 6, and 3. 
*/
void AFighter::PerformNormal(FName AttkName) {
	CurrAttk = FighterDataTable->FindRow<FAttackStruct>(AttkName, "Normal");
	if (!CurrAttk) { return; }

	PreviousState = State;
	UpdateState(EFighterState::STARTUP);
	CanTargetCombo = false;
	CanJumpCancel = false;
	CanSpecialCancel = false;
	//cancel previous attack hitbox if there is one already out
	if (ActiveHitbox) {
		ActiveHitbox->Destroy();
	}

	//if (AnimInstance && CurrAttk->Animation) AnimInstance->Montage_Play(CurrAttk->Animation);
	PlayMontage(CurrAttk->Animation);
	FrameTimer = CurrAttk->Startup; //starts the frame timer in tick

}

/**
 * fore ball:
 * Spawn prijectile
 * Spawn a hitbox
 * Attach the hitbox
 * Detect overlap in hitbox-hurtbox
 * 
 * 
 * 
*/
void AFighter::PerformSpecial(FName SpecialName) {
	CurrAttk = FighterDataTable->FindRow<FAttackStruct>(SpecialName, "Special");

	if (!CurrAttk) { return; }
	//flush the input buffer here
	OurController->FlushBuffer();
	PreviousState = State;
	CanTargetCombo = false;
	CanJumpCancel = false;
	CanSpecialCancel = false;
	UpdateState(EFighterState::STARTUP);
	//if (AnimInstance && CurrAttk->Animation) AnimInstance->Montage_Play(CurrAttk->Animation);

	PlayMontage(CurrAttk->Animation);
	FrameTimer = CurrAttk->Startup; //starts the frame timer in tick
}

void AFighter::PerformDash(bool Back) {
	FName DashName = "";
	if (State == EFighterState::JUMPING) {
		//airdash state
		DashName = Back ? "BackAirDash" : "AirDash";
		CurrAttk = FighterDataTable->FindRow<FAttackStruct>(DashName, "AirDash");
		if (CurrAttk) {
			OurController->FlushBuffer();
			PreviousState = State;
			NumAirDashes--;
			UpdateState(EFighterState::AIRDASHING);
			//if (AnimInstance && CurrAttk->Animation) AnimInstance->Montage_Play(CurrAttk->Animation);

			PlayMontage(CurrAttk->Animation);
			FrameTimer = CurrAttk->Startup; //starts the frame timer in tick
		}
	} else {
		//dash state
		DashName = Back ? "BackDash" : "Dash";
		CurrAttk = FighterDataTable->FindRow<FAttackStruct>(DashName, "Dash");
		if (CurrAttk) {
			OurController->FlushBuffer();
			PreviousState = State;
			UpdateState(EFighterState::DASHING);
			//if (AnimInstance && CurrAttk->Animation) AnimInstance->Montage_Play(CurrAttk->Animation);

			PlayMontage(CurrAttk->Animation);
			FrameTimer = CurrAttk->Startup; //starts the frame timer in tick
		}
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


		case EFighterState::DEFENDING:
			valid = (State != EFighterState::JUMPING);
			break;

		// might be jank ;m;
		case EFighterState::JUMPING:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::FORWARDING || State == EFighterState::DEFENDING || GetCharacterMovement()->IsFalling() ||
				(CanJumpCancel && State == EFighterState::ACTIVE) || (CanJumpCancel && State == EFighterState::RECOVERY));
			break;

		case EFighterState::CROUCHING:
			valid = (State != EFighterState::JUMPING);
			break;

		case EFighterState::CROUCHBLOCKING:
			valid = (State != EFighterState::JUMPING);
			break;

		case EFighterState::DASHING:
			//i added the neutral case for when we add a dash macro
			valid = (State == EFighterState::FORWARDING || State == EFighterState::NEUTRAL);
			break;

		case EFighterState::AIRDASHING:
			valid = (State == EFighterState::JUMPING && NumAirDashes > 0);
			break;

		case EFighterState::STARTUP:
			valid = (State == EFighterState::NEUTRAL || State == EFighterState::DEFENDING || State == EFighterState::JUMPING 
				|| State == EFighterState::FORWARDING || State == EFighterState::CROUCHING || State == EFighterState::CROUCHBLOCKING);
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
		//reset character essentially
		case EFighterState::NEUTRAL:
		case EFighterState::DEFENDING:
		case EFighterState::FORWARDING:
		case EFighterState::CROUCHING:
		case EFighterState::CROUCHBLOCKING:
			Locked = false;
			CanJumpCancel = false;
			CanSpecialCancel = false;
			CanTargetCombo = false;
			break;

		//lock some states out (neutral, defending, forwarding)
		case EFighterState::KNOCKDOWN:
		case EFighterState::STARTUP:
		case EFighterState::DASHING:
		case EFighterState::AIRDASHING:
		case EFighterState::BLOCKSTUN:
		case EFighterState::HITSTUN:
			Locked = true;
			break;

		case EFighterState::JUMPING:
			Locked = true;
			if (ActiveHitbox) {
				ActiveHitbox->Destroy();
			}

			/*if (AnimInstance && AnimInstance->Montage_IsPlaying(NULL)) {
				StopAnimMontage(nullptr);
			}*/
			StopMontage();
			/*if (AnimInstance) AnimInstance->Montage_Stop(NULL);*/
			break;
	}
	State = NewState;
}

void AFighter::SetFrameTimer(int NumFrames)
{
	FrameTimer = NumFrames;
}


/*
* Used for things that last a certain amount of frames!
* In tick, you will see a timer variable that is going to constantly decrement when it is greater than 0.
* When that timer hits 0, it will call this function.
* This function will decide what state to advance to based on the current state.
* 
* For example, in PerformNormal() it will update the state to *Startup* and will set the frame timer to some number of startup frames.
* When that timer hits 0, it will enter this function essentially meaning that startup is over and we can move onto the *Active* state.
* Which then sets the timer again! For it to eventually return here to update the state into *Recovery*.
*/
void AFighter::FrameAdvanceState() {
	FrameTimer = -1; //for safety
	//GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Red, "frame advance state");
	FActorSpawnParameters SpawnInfo;
	switch (State) {
		case EFighterState::STARTUP:
			UpdateState(EFighterState::ACTIVE);
			//spawns the hitbox according to the current attack
			ActiveHitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass(), GetActorLocation() + CurrAttk->HitboxLoc, FRotator::ZeroRotator, SpawnInfo);
			ActiveHitbox->Initialize(CurrAttk, CurrAttk->HitboxScale, CurrAttk->HitboxLoc, this);
			FrameTimer = CurrAttk->Active;
			break;
		case EFighterState::ACTIVE:
			UpdateState(EFighterState::RECOVERY);
			if (ActiveHitbox) {
				ActiveHitbox->Destroy();
			}
			FrameTimer = CurrAttk->Recovery;
			break;
		case EFighterState::RECOVERY:
			//this conditional is purely for target combos
			if (PreviousState != EFighterState::ACTIVE && PreviousState != EFighterState::RECOVERY && PreviousState != EFighterState::STARTUP) {
				UpdateState(PreviousState);
			} else {
				UpdateState(EFighterState::NEUTRAL);
			}
			break;

		case EFighterState::HITSTUN:
			(PreviousState == EFighterState::JUMPING) ? UpdateState(EFighterState::JUMPING) : UpdateState(EFighterState::NEUTRAL);
			OtherPlayer->ComboCounter = 0;
			GetWorld()->GetAuthGameMode<AFightGameMode>()->GetFightingHUD()->UpdateCombo(ComboCounter, OtherPlayer);
			break;

		case EFighterState::BLOCKSTUN:
			UpdateState(EFighterState::NEUTRAL);
			//what this is doing is essentially updating the state to whatever the input buffer has last detected.
			OurController->CheckForSequence(); 
			break;

		case EFighterState::KNOCKDOWN:
			UpdateState(EFighterState::NEUTRAL);
			OurController->CheckForSequence();
			OtherPlayer->ComboCounter = 0;
			GetWorld()->GetAuthGameMode<AFightGameMode>()->GetFightingHUD()->UpdateCombo(ComboCounter, OtherPlayer);
			break;

		case EFighterState::AIRDASHING:
			UpdateState(EFighterState::JUMPING);
			break;

		case EFighterState::DASHING:
			UpdateState(EFighterState::NEUTRAL);
			break;
	}
}

/*
* Called whenever this fighter has hit the other fighter.
* This will set up all onhit cancel booleans depending on if the current attack allows it and 
* will set up the damage scaling and combo counting depending if the current attack connected or was blocked.
*/
void AFighter::OnHitOther() {
	CanJumpCancel = CurrAttk->JumpCancellable;
	CanSpecialCancel = CurrAttk->SpecialCancellable;
	CanTargetCombo = CurrAttk->TargetComboable;
	NextTargetInput = CurrAttk->NextTargetInput;
	
	if (OtherPlayer->State == EFighterState::HITSTUN || OtherPlayer->State == EFighterState::KNOCKDOWN) {
		ComboCounter++;
		GetWorld()->GetAuthGameMode<AFightGameMode>()->GetFightingHUD()->UpdateCombo(ComboCounter, this);
	}
	// FString Debug = FString::Printf(TEXT("Actor Rotation: (%f, %f, %f)"), Rot.Pitch, Rot.Yaw, Rot.Roll);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Combo counter meow: %d"), ComboCounter));
	//check if it was blocked for comboing and scaling!
	StartHitStop(0.04f);
}

/*
* Called whenever this fighter is hit by the other fighter.
* This will update our next state depending on if we're blocking or not and if the
  attack is a knockdown or not
  
  **PARAMETERS**
	OwCauser is the attackstruct that represents the attack that hit us,
	the main things we're looking from that struct are
	Blockstun, Hitstun, and (is)Knockdown.
*/
void AFighter::OnOw(FAttackStruct* OwCauser) {
	if (ActiveHitbox) {
		ActiveHitbox->Destroy();
	}

	if (!OwCauser) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "OWCAUSER SHOULD NOT BE NULL");
		return;
	}

	if (State == EFighterState::BLOCKSTUN ||
		(State == EFighterState::DEFENDING &&OwCauser->AttackType != EAttackType::LOW) ||
		(State == EFighterState::CROUCHBLOCKING && OwCauser->AttackType != EAttackType::HIGH)) {
		//blocking
		UpdateState(EFighterState::BLOCKSTUN);
		FrameTimer = OwCauser->Blockstun;
	} else {
		//i didn't pay 60 bucks to block
		if (OwCauser->Knockdown) {
			UpdateState(EFighterState::KNOCKDOWN);
			FrameTimer = 60; //this has to be a consistent number across the entire cast
		} else {
			PreviousState = State;
			UpdateState(EFighterState::HITSTUN);
			FrameTimer = OwCauser->Hitstun;
		}
	}
}

void AFighter::StartHitStop(float Duration) {
	OurController->SetActorTickEnabled(false);
	OtherPlayer->OurController->SetActorTickEnabled(false);
	OurController->GetCharacter()->CustomTimeDilation = 0.0f;
	OtherPlayer->OurController->GetCharacter()->CustomTimeDilation = 0.0f;
	GetWorldTimerManager().SetTimer(HitStopTimerHandler, this, &AFighter::StopHitStop, Duration, false);
}

void AFighter::StopHitStop() {
	OurController->SetActorTickEnabled(true);
	OtherPlayer->OurController->SetActorTickEnabled(true);
	OurController->GetCharacter()->CustomTimeDilation = 1.0f;
	OtherPlayer->OurController->GetCharacter()->CustomTimeDilation = 1.0f;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "RACIST");
}


// === FIGHTER MOVE FUNCTIONS ===
void AFighter::LightNormal(bool Target) {
	// string var name 
	FName AttkName = "";
	if (Target && CurrAttk) {
		AttkName = CurrAttk->NextTargetName;
	} else if (State == EFighterState::JUMPING) {
		AttkName = "LightJumpAttk";
	} else if (State == EFighterState::CROUCHING || State == EFighterState::CROUCHBLOCKING) {
		AttkName = "LightCrouchAttk";
	} else {
		AttkName = "LightAttk";
	}
	PerformNormal(AttkName);
}

void AFighter::HeavyNormal(bool Target) {
	FName AttkName = "";
	if (Target && CurrAttk) {
		AttkName = CurrAttk->NextTargetName;
	} else if (State == EFighterState::JUMPING) {
		AttkName = "HeavyJumpAttk";
	} else if (State == EFighterState::CROUCHING || State == EFighterState::CROUCHBLOCKING) {
		AttkName = "HeavyCrouchAttk";
	} else {
		AttkName = "HeavyAttk";
	}
	PerformNormal(AttkName);
}

void AFighter::LightQuarterCircleForward() {
	//we're not gonna have any jumping specials for now
	//so i don't need be like
	FName SpecialName = "LightFQC";
	PerformSpecial(SpecialName);
}

void AFighter::HeavyQuarterCircleForward() {
	FName SpecialName = "HeavyFQC";
	PerformSpecial(SpecialName);
}

void AFighter::LightQuarterCircleBack()
{
}

void AFighter::HeavyQuarterCircleBack()
{
}

void AFighter::LightDragonPunch()
{
}

void AFighter::HeavyDragonPunch()
{
}


