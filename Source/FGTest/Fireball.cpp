// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"

// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    thingy = 100;
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("popcorning")));

}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireball::PerformLight() {
 // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("neigh"))); 
}

void AFireball::PerformHeavy() {
    
}
