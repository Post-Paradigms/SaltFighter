// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Hitbox.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API UHitbox : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UHitbox();

	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	// on hit - overlap 


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* MeshComponent;

};
