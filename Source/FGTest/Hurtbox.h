// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Hurtbox.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API UHurtbox : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UHurtbox();

	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	// on hit - overlap 

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* MeshComponent;
	
};
