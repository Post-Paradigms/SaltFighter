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

void AFighter::TestEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Test Event Called"));
}

void AFighter::MoveEvent(const FInputActionValue &Value)
{
	if (Value.IsNonZero())
	{
		float direction = Value.GetMagnitude();
		AddMovementInput(FVector::ForwardVector, direction);
	}
}

void AFighter::VectorMoveEvent(const FInputActionValue &Value)
{
	if (Value.IsNonZero())
	{
		// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Pressing vector move"));
		const FVector2D MovementVector = Value.Get<FVector2D>();

		FString Debug = FString::Printf(TEXT("Num Pad Sector: %d"), VectorToOctant(MovementVector));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);
	}
}

void AFighter::JumpEvent(const FInputActionValue &Value)
{
	Jump();
}

void AFighter::UpEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("8"));
}

void AFighter::DownEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("2"));
}

void AFighter::LeftEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("4"));
}

void AFighter::RightEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("6"));
}

void AFighter::DownRightEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("3"));
}

void AFighter::LightAttackEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Light"));
}

void AFighter::FireballEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("*Fireball*"));
}

void AFighter::CommandNormalLightEvent(const FInputActionValue &Value)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("*Command Normal (Light)*"));
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

int AFighter::VectorToNumPadSector(FVector2D Vector)
{
	const TArray<int> NumPadSectors = {8, 9, 6, 3, 2, 1, 4, 7};

	float Angle = FMath::Atan2(Vector.X, Vector.Y) + (PI / 8.f);

	if (Angle < 0) Angle += 2 * PI;

	Angle = FMath::RadiansToDegrees(Angle);

	FString Debug = FString::Printf(TEXT("Angle Degrees: %f"), Angle);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.015f, FColor::Green, Debug);

	int Octant = FMath::FloorToInt(Angle / 45);
		
	return NumPadSectors[Octant];
}
