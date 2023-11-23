// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox.h"
#include "Components/StaticMeshComponent.h"

UHitbox::UHitbox()
{
    // more for visual repsentation 
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(this);
}

void UHitbox::BeginPlay()
{
    Super::BeginPlay();
}
