// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"


/* 
 Proximity Hitbox -> indication am i going to be hit by this high medium or low - gives all info before damage is dealt attack starting
 Strike -> damage is being done and what part of the body its on - attack active
 hurtbox -> goes around the actual player - attack connects
 damage occurs when there is a collision from a strike hitbox and a hurbox
*/
UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	//HB_PROX 		UMETA(DisplayName = "Proximity"),
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_HURTBOX 		UMETA(DisplayName = "Hurtbox")
	
};

UCLASS()
class FGTEST_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	// hitbox enum instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitboxEnum HitboxType;

	// location to spawn hitbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FVector HitboxLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hitbox Actor")
	class UStaticMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// notify that the hitbox is ready to be drawn
	UFUNCTION(BlueprintCallable)
	void TriggerVisualizeHitBox();

	// visualizes the hitbox 
	UFUNCTION(BlueprintImplementableEvent)
	void VisualizeHitBox();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnHitbox();



};
