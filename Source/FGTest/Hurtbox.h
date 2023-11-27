// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Fighter.h"
#include "Hurtbox.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AHurtbox : public AActor
{
	GENERATED_BODY()

public:
	AHurtbox();

	FORCEINLINE class UBoxComponent* GetBoxComponent() const { return BoxComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AFighter* HurtboxOwner;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
   	UBoxComponent* BoxComponent;
	 
    UFUNCTION()
    void BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void ApplyKnockback(float Angle, float Force);
	
	int seq;
};
