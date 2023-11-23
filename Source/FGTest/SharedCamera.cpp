// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASharedCamera::ASharedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	BaseScene->SetupAttachment(RootComponent);

	// Sets up camera properties.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(BaseScene);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);


	CameraBoom->SetRelativeLocationAndRotation(FVector(-300.0f, -40.0f, 40.0f), FRotator(0.0f, 0.0f, 0.0f));
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bDoCollisionTest = true;

	Visual = CreateDefaultSubobject<USphereComponent>(TEXT("Visual"));
	Visual->SetupAttachment(BaseScene);
}

// Called when the game starts or when spawned
void ASharedCamera::BeginPlay()
{
	Super::BeginPlay();

	//Sets views of all controllers to this actor's camera.
	for (FConstPlayerControllerIterator ControllerIt = GetWorld()->GetPlayerControllerIterator(); ControllerIt; ++ControllerIt) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("One Iteration"));
		APlayerController* FighterCtlr = ControllerIt->Get();
		if (FighterCtlr) {
			FighterCtlr->SetViewTargetWithBlend(this);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Controller view set."));
			ActiveFighters.Add(FighterCtlr->GetPawn());
		}
	}

}

// Called every frame
void ASharedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Sets views of all controllers to this actor's camera.
	if (ActiveFighters.Num() < 2) {
		for (FConstPlayerControllerIterator ControllerIt = GetWorld()->GetPlayerControllerIterator(); ControllerIt; ++ControllerIt) {
			APlayerController* FighterCtlr = ControllerIt->Get();
			if (FighterCtlr) {
				FighterCtlr->SetViewTargetWithBlend(this);
				ActiveFighters.Add(FighterCtlr->GetPawn());
			}
		}
	}
	


	//Logic assumes two fighters.
	if (ActiveFighters.Num() == 2) {
		if (ActiveFighters[0] && ActiveFighters[1]) {
			FVector Distance = ActiveFighters[0]->GetActorLocation() - ActiveFighters[1]->GetActorLocation();
			FVector FinalPos = Distance / 2 + ActiveFighters[1]->GetActorLocation();
			FinalPos.X += 40;
			SetActorLocation(FinalPos);

			CameraBoom->TargetArmLength = FMath::Clamp(Distance.Size() / 3, 100, 500);
		}
	}
}

