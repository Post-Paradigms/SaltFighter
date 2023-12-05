// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fighter.h"
#include "ProjectileBase.generated.h"


USTRUCT(BlueprintType)
struct FProjectileStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FVector HitboxLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FVector HitboxScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int Lifespan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int Blockstun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int Hitstun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool Knockdown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int KnockbackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int KnockbackForce;
};

UCLASS()
class FGTEST_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	AFighter* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UDataTable* ProjectileDataTable;

	int Lifespan;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FProjectileStruct* ProjectileData;

	int Direction;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UStaticMeshComponent* MeshComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PerformLight() PURE_VIRTUAL(AProjectileBase::PerformLight, );
	virtual void PerformHeavy() PURE_VIRTUAL(AProjectileBase::PerformHeavy, );
	FORCEINLINE void SetOwner(AFighter* FighterRef) { Owner = FighterRef; }
};
