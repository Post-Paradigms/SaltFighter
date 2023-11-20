// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FighterController.h"
#include "Fighter.h"
#include "FightGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFightGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AFightGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext; // TODO: WHAT MAPPING CONTEXT IS SUPPOSED TO GO HERE???

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when removed from level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	AFighterController* P1FighterController;
	AFighter* P1FighterCharacter;

	AFighterController* P2FighterController;
	AFighter* P2Fighter;
};
