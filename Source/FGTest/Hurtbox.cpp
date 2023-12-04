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

    // Hit Effect
    static ConstructorHelpers::FClassFinder<ANiagaraActor> HurtEffectClass(TEXT("/Game/Blueprints/BP_HitEffect"));
    if (HurtEffectClass.Class)
    {
        HurtEffectComponent = Cast<ANiagaraActor>(HurtEffectClass.Class->GetDefaultObject());
    }
}

void AHurtbox::BeginPlay()
{
    Super::BeginPlay();
    GetBoxComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHurtbox::BeginOverlap);
}

/* Kennet pls chek thx <3 */
void AHurtbox::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{

    AHitbox* IncomingHitbox = Cast<AHitbox>(OtherActor);
    if (IncomingHitbox && HurtboxOwner && IncomingHitbox->Owner && IncomingHitbox->Owner != HurtboxOwner 
        && IncomingHitbox->AttkInfo)
    {
        HurtboxOwner->OnOw(IncomingHitbox->AttkInfo);
        IncomingHitbox->Owner->OnHitOther();

        if (AFightGameMode* GameMode = Cast<AFightGameMode>(GetWorld()->GetAuthGameMode())) {
            /* Where da dmg?? :/ - will be chips */
            GameMode->DamagePlayer(HurtboxOwner, 1);
        }
        
        ApplyKnockback(IncomingHitbox->AttkInfo->KnockbackAngle, IncomingHitbox->AttkInfo->KnockbackForce);

        SpawnHurtEffect();
    }
}

void AHurtbox::ApplyKnockback(float Angle, float Force)
{
    float AngleRad = Angle * (PI / 180);
    FVector AngleVector = FVector(std::cos(AngleRad), 0, std::sin(AngleRad));
    FVector LaunchDirection = FVector((HurtboxOwner->IsLeftSide) ? -1 : 1, 1, 1);
    HurtboxOwner->LaunchCharacter(AngleVector.GetSafeNormal() * Force * LaunchDirection, true, true);
}

void AHurtbox::SpawnHurtEffect()
{
    if (HurtEffectComponent)
    {
        FVector SpawnLocation = HurtboxOwner->GetActorLocation() + FVector(-50.f, 0.f, 0.f) * ((HurtboxOwner->IsLeftSide) ? -1.f : 1.f);
        FRotator SpawnRotation = FRotator(90.f, 0.f, 0.f) * ((HurtboxOwner->IsLeftSide) ? -1.f : 1.f);
        FActorSpawnParameters SpawnInfo;
        ANiagaraActor* EffectInstance = GetWorld()->SpawnActor<ANiagaraActor>(HurtEffectComponent->GetClass(), SpawnLocation, SpawnRotation, SpawnInfo);
        EffectInstance->SetLifeSpan(0.5f);
    }
}