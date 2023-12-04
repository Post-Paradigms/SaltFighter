// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuUserWidget.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMainMenuGameMode();

	UMainMenuUserWidget* GetMainMenuWidget();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when removed from level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// User Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenuUserWidget> MainMenuUWClass;

	UPROPERTY()
	class UMainMenuUserWidget* MainMenuUW;
};
