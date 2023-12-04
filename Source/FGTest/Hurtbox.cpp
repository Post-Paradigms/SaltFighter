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
// currently heavily reworking dont look at this ok
void AHurtbox::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "eee eewwee weeee");
    AHitbox* IncomingHitbox = Cast<AHitbox>(OtherActor);
    if ((IncomingHitbox && HurtboxOwner && IncomingHitbox->Owner) && (IncomingHitbox->Owner != HurtboxOwner))
    {
        // Hitbox is owned by fighter
        AFighter* FightOwner = Cast<AFighter>(IncomingHitbox->Owner);
        if (FightOwner) {
            if (IncomingHitbox->AttkInfo) {
                HurtboxOwner->OnOw(IncomingHitbox);
                FightOwner->OnHitOther();
            }
        }
        // Hitbox is owned by projectile
        AProjectileBase* ProjectileOwner = Cast<AProjectileBase>(IncomingHitbox->Owner);
        if (ProjectileOwner) {
            if (ProjectileOwner->Owner != HurtboxOwner) {
                //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "eee eewwee weeee");
                HurtboxOwner->OnOw(IncomingHitbox);
            }
        }

        if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode())) {
            /* Where da dmg?? :/ - will be chips */
            if (!ProjectileOwner || (ProjectileOwner && ProjectileOwner->Owner != HurtboxOwner)) {
                GameMode->DamagePlayer(HurtboxOwner, 1);
            }
        }
        
        if (FightOwner) {
            ApplyKnockback(IncomingHitbox->AttkInfo->KnockbackAngle, IncomingHitbox->AttkInfo->KnockbackForce);
        }

        //if (HurtboxOwner->State == EFighterState::DEFENDING)
        //{
        //    // Apply blockstun
        //    int BlockstunLength = IncomingHitbox->AttkInfo->Blockstun;
        //    if (HurtboxOwner->ValidateState(EFighterState::BLOCKSTUN))
        //    {
        //        HurtboxOwner->UpdateState(EFighterState::BLOCKSTUN);
        //        HurtboxOwner->SetFrameTimer(BlockstunLength);
        //        if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode()))
        //        {
        //            /* Where da dmg?? :/ - will be chips */
        //            GameMode->DamagePlayer(HurtboxOwner, 1);
        //        }
        //    }
        //}
        //else // caught lackin, not blocking
        //{
        //    // Apply hitstun
        //    int HitstunLength = IncomingHitbox->AttkInfo->Hitstun;
        //    bool IsKnockdown = IncomingHitbox->AttkInfo->Knockdown;
        //    if (IsKnockdown && HurtboxOwner->ValidateState(EFighterState::KNOCKDOWN))
        //    {
        //        HurtboxOwner->UpdateState(EFighterState::KNOCKDOWN);
        //        // HurtboxOwner->SetFrameTimer(BlockstunLength); idk ??? how long to set it to
        //    }
        //    else if (HurtboxOwner->ValidateState(EFighterState::HITSTUN))
        //    {
        //        HurtboxOwner->UpdateState(EFighterState::HITSTUN);
        //        HurtboxOwner->SetFrameTimer(HitstunLength);
        //    }
        //    
        //    if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode()))
        //    {
        //        /* Where da dmg?? :/ - -.-*/
        //        // GameMode->DamagePlayer(HurtboxOwner, IncomingHitbox->AttkInfo->Damage);
        //    }
        //}
    }
}

void AHurtbox::ApplyKnockback(float Angle, float Force)
{
    float AngleRad = Angle * (PI / 180);
    FVector AngleVector = FVector(std::cos(AngleRad), 0, std::sin(AngleRad));
    FVector LaunchDirection = FVector((HurtboxOwner->IsLeftSide) ? -1 : 1, 1, 1);
    HurtboxOwner->LaunchCharacter(AngleVector.GetSafeNormal() * Force * LaunchDirection, true, true);
}