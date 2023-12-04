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
        AFighter* FightOwner;
        if (IncomingHitbox->IsProjectile) {
            AProjectileBase* ProjectileOwner = Cast<AProjectileBase>(IncomingHitbox->Owner);
            if (ProjectileOwner->Owner == HurtboxOwner || !IncomingHitbox->ProjectileInfo) {
                return;
            }
            // Hitbox owned by projectile 
            FightOwner = ProjectileOwner->Owner;
        }
        else {
            // Hitbox owned by fighter owo
            FightOwner = Cast<AFighter>(IncomingHitbox->Owner);
            if (!IncomingHitbox->AttkInfo) {
                return;
            }
        }

        if (FightOwner) {
                HurtboxOwner->OnOw(IncomingHitbox);
                FightOwner->OnHitOther();
        }

        if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode())) {
            /* Where da dmg?? :/ - will be chips */
            GameMode->DamagePlayer(HurtboxOwner, 1);
        }
        
        if (FightOwner && IncomingHitbox->AttkInfo) {
            ApplyKnockback(IncomingHitbox->AttkInfo->KnockbackAngle, IncomingHitbox->AttkInfo->KnockbackForce);
        }
    }
}

void AHurtbox::ApplyKnockback(float Angle, float Force)
{
    float AngleRad = Angle * (PI / 180);
    FVector AngleVector = FVector(std::cos(AngleRad), 0, std::sin(AngleRad));
    FVector LaunchDirection = FVector((HurtboxOwner->IsLeftSide) ? -1 : 1, 1, 1);
    HurtboxOwner->LaunchCharacter(AngleVector.GetSafeNormal() * Force * LaunchDirection, true, true);
}