// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerWalls.generated.h"

UCLASS()
class FGTEST_API APlayerWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWalls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LeftWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RightWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wall, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Center;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Wall, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Fighters;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Wall, meta = (AllowPrivateAccess = "true"))
	int DistanceFromCenter = 500;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
