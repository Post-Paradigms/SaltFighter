// Fill out your copyright notice in the Description page of Project Settings.


#include "Hurtbox.h"
#include "Hitbox.h"
#include "FightGameMode.h"
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

/* Kennet pls chek thx <3 */
void AHurtbox::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    // if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode()))
    // {
    //     /* Where da dmg?? :/ - will be chips */
    //     GameMode->DamagePlayer(HurtboxOwner, 20);
    // } 

    AHitbox* IncomingHitbox = Cast<AHitbox>(OtherActor);
    if (IncomingHitbox && HurtboxOwner && IncomingHitbox->AttkInfo)
    {
        if (HurtboxOwner->State == EFighterState::DEFENDING)
        {
            // Apply blockstun
            int BlockstunLength = IncomingHitbox->AttkInfo->Blockstun;
            if (HurtboxOwner->ValidateState(EFighterState::BLOCKSTUN))
            {
                HurtboxOwner->UpdateState(EFighterState::BLOCKSTUN);
                HurtboxOwner->SetFrameTimer(BlockstunLength);
                if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode()))
                {
                    /* Where da dmg?? :/ - will be chips */
                    GameMode->DamagePlayer(HurtboxOwner, 1);
                }
            }
        }
        else // caught lackin, not blocking
        {
            // Apply hitstun
            int HitstunLength = IncomingHitbox->AttkInfo->Hitstun;
            bool IsKnockdown = IncomingHitbox->AttkInfo->Knockdown;
            if (IsKnockdown && HurtboxOwner->ValidateState(EFighterState::KNOCKDOWN))
            {
                HurtboxOwner->UpdateState(EFighterState::KNOCKDOWN);
                // HurtboxOwner->SetFrameTimer(BlockstunLength); idk ??? how long to set it to
            }
            else if (HurtboxOwner->ValidateState(EFighterState::HITSTUN))
            {
                HurtboxOwner->UpdateState(EFighterState::HITSTUN);
                HurtboxOwner->SetFrameTimer(HitstunLength);
            }
            
            if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode()))
            {
                /* Where da dmg?? :/ - -.-*/
                // GameMode->DamagePlayer(HurtboxOwner, IncomingHitbox->AttkInfo->Damage);
            }
        }
    }
}