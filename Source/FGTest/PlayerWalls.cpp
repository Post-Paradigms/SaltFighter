// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWalls.h"

// Sets default values
APlayerWalls::APlayerWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Center = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));
	RootComponent = Center;

	LeftWall = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWall"));
	LeftWall->SetupAttachment(RootComponent);
	LeftWall->SetRelativeLocationAndRotation(FVector(0.f, DistanceFromCenter * -1, 0.f), FRotator(0.0f, 0.0f, 0.0f));
	LeftWall->SetRelativeScale3D(FVector(2.f, 0.f, 10.f));

	RightWall = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWall"));
	RightWall->SetupAttachment(RootComponent);
	RightWall->SetRelativeLocationAndRotation(FVector(0.f, DistanceFromCenter, 0.f), FRotator(0.0f, 0.0f, 0.0f));
	RightWall->SetRelativeScale3D(FVector(2.f, 0.f, 10.f));
}

// Called when the game starts or when spawned
void APlayerWalls::BeginPlay()
{
	Super::BeginPlay();
	
	for (FConstPlayerControllerIterator ControllerIt = GetWorld()->GetPlayerControllerIterator(); ControllerIt; ++ControllerIt) {
		APlayerController* FighterCtlr = ControllerIt->Get();
		if (FighterCtlr) {
			FighterCtlr->SetViewTargetWithBlend(this);
			Fighters.Add(FighterCtlr->GetPawn());
		}
	}
}

// Called every frame
void APlayerWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Fighters.Num() == 2) {

		if (Fighters[0] && Fighters[1]) {
			FVector Distance = Fighters[0]->GetActorLocation() - Fighters[1]->GetActorLocation();
			FVector FinalPos = Distance / 2 + Fighters[1]->GetActorLocation();
			SetActorRelativeLocation(FinalPos);
		}
	}
}

