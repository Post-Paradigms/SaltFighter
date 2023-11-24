// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHitboxActor::AHitboxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// setting up all of the default components for the response actor 
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitboxDisplay")); // make this into a cube component
	// desired dimentions scale: .6 .2 .2 
	MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHitboxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// hitbox is ready to be drawn 
void AHitboxActor::TriggerVisualizeHitBox()
{
	VisualizeHitBox();
}
