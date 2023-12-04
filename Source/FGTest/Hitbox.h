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
UCLASS()
class FGTEST_API AHitbox : public AActor
{
	GENERATED_BODY()

public:
	AHitbox();

    FAttackStruct* AttkInfo;

	FProjectileStruct* ProjectileInfo;

	AActor* Owner;
	
	bool IsProjectile;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoxComponent* BoxComponent;

public:
	FORCEINLINE class UBoxComponent* GetBoxComponent() const { return BoxComponent; }

	virtual void Tick(float DeltaTime) override;

	void Initialize(FAttackStruct* AttkData, FVector Size, FVector SpawnLocation, AActor* HitboxOwner);

	void InitializeProjectile(FProjectileStruct* AttkData, FVector Size, FVector SpawnLocation, AActor* HitboxOwner);

private:
	int ActiveFramesRemaining;
};
