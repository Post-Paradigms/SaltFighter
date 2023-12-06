// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ProjectileBase.h"
#include "Fighter.h"
#include "Hitbox.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE(FTriggerDelegate);

UCLASS()
class FGTEST_API AHitbox : public AActor
{
	GENERATED_BODY()

public:
	AHitbox();

	AFighter* Owner;

    FAttackStruct* AttkInfo;

	bool IsProjectile;
	FProjectileStruct* ProjectileInfo;
	AProjectileBase* MyProjectile;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	FTriggerDelegate OnTriggerDelegate;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* BoxComponent;

public:
	FORCEINLINE class UBoxComponent* GetBoxComponent() const { return BoxComponent; }

	virtual void Tick(float DeltaTime) override;

	void Initialize(FAttackStruct* AttkData, FVector Size, FVector SpawnLocation, AFighter* HitboxOwner);

	void Initialize(FProjectileStruct* AttkData, FVector Size, FVector SpawnLocation, AProjectileBase* Projectile);

private:
	int ActiveFramesRemaining;
};
