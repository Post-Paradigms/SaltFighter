// Fill out your copyright notice in the Description page of Project Settings.


#include "Hurtbox.h"
#include "Hitbox.h"
#include "Fighter.h"
#include "Components/StaticMeshComponent.h"

AHurtbox::AHurtbox()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Hurtbox"));
    BoxComponent->SetupAttachment(RootComponent);   
    BoxComponent->SetRelativeScale3D(FVector(1.5, 1.5, 2.5));
    BoxComponent->bHiddenInGame = false;
}

void AHurtbox::BeginPlay()
{
    Super::BeginPlay();
    GetBoxComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHurtbox::BeginOverlap);
}

void AHurtbox::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    if(AHitbox * IncomingHitbox = Cast<AHitbox>(OtherActor)) // on hit 
    { 
        if(GEngine)
	        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Overlap Activated"));

        if (AFighter * CastedOwner = Cast<AFighter>(GetParentActor())) 
        {
            if (CastedOwner->State == EFighterState::DEFENDING)
            {
                // Apply blockstun
                int Blockstun = IncomingHitbox->AttkInfo->Blockstun;
                if (CastedOwner->ValidateState(EFighterState::BLOCKSTUN))
                {
                    CastedOwner->UpdateState(EFighterState::BLOCKSTUN);
                }
            }
            else // caught lackin
            {
                // Apply hitstun
                int Hitstun = IncomingHitbox->AttkInfo->Hitstun;
                bool IsKnockdown = IncomingHitbox->AttkInfo->Knockdown;
                if (IsKnockdown && CastedOwner->ValidateState(EFighterState::KNOCKDOWN))
                {
                    CastedOwner->UpdateState(EFighterState::KNOCKDOWN);
                }
                else if (CastedOwner->ValidateState(EFighterState::HITSTUN))
                {
                    CastedOwner->UpdateState(EFighterState::HITSTUN);
                }
            }
        }
    }
}