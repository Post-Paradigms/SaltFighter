// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Hitbox.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFireball : public AProjectileBase
{
	GENERATED_BODY()
	//int Lifespan = 100;
	
public:	
	// Sets default values for this actor's properties
	AFireball();

	class AHitbox* ActiveHitbox;

//	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PerformLight();
	virtual void PerformHeavy();

};
