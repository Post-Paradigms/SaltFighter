// Fill out your copyright notice in the Description page of Project Settings.


#include "Hurtbox.h"
#include "Components/StaticMeshComponent.h"

UHurtbox::UHurtbox()
{
    // more for visual repsentation 
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(GetOber);
}

void UHurtbox::BeginPlay()
{
    Super::BeginPlay();
}
