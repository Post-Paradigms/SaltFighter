// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox.h"
#include "Components/StaticMeshComponent.h"
#include "Fighter.h"

AHitbox::AHitbox()
{
    ActiveFramesRemaining = -1;
     
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Hurtbox"));
    BoxComponent->SetupAttachment(RootComponent);
    BoxComponent->bHiddenInGame = false;
}

void AHitbox::BeginPlay()
{
    Super::BeginPlay();
}

void AHitbox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

  /*  if (ActiveFramesRemaining > 0) ActiveFramesRemaining--;

    if (ActiveFramesRemaining <= 0) Destroy();*/
}

void AHitbox::Initialize(FAttackStruct* AttkData, FVector Size, FVector SpawnLocation, AActor* HitboxOwner)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("awoooooooooooo")));
    AttkInfo = AttkData;
    Owner = HitboxOwner;
    this->AttachToActor(HitboxOwner, FAttachmentTransformRules::KeepRelativeTransform);
    BoxComponent->SetRelativeScale3D(Size);
    BoxComponent->SetRelativeLocation(SpawnLocation);
    ActiveFramesRemaining = AttkInfo->Active;
}


void AHitbox::InitializeProjectile(FProjectileStruct* ProjectileData, FVector Size, FVector SpawnLocation, AActor* HitboxOwner)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("awoooooooooooo")));
    ProjectileInfo = ProjectileData;
    Owner = HitboxOwner;
    this->AttachToActor(HitboxOwner, FAttachmentTransformRules::KeepRelativeTransform);
    BoxComponent->SetRelativeScale3D(Size);
    BoxComponent->SetRelativeLocation(SpawnLocation);
    ActiveFramesRemaining = ProjectileInfo->Lifespan;
}
