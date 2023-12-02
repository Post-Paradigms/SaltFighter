// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* ButtonVBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* CreditButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	void UpdateButtonState();

protected:
	virtual void NativeConstruct() override;

private:
	TArray<UButton*> NavigationItems;

	int NavigationIndex;

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnCreditClicked();

	UFUNCTION()
	void OnQuitClicked();

	// This would be easier in blueprint but too bad, blueprint cringe
	UFUNCTION()
	void SetPlayIndex();
	UFUNCTION()
	void SetCreditIndex();
	UFUNCTION()
	void SetQuitIndex();
};
