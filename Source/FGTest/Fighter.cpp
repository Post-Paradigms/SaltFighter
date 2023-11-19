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
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Face();
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
	Jump();
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
