// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FighterController.generated.h"

/**
 * 
 */
UCLASS()
class FGTEST_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFighterController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	int PolledInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnMovePressed(const FInputActionValue &Value);
	void OnJumpPressed(const FInputActionValue &Value);
	void OnLightAttackPressed(const FInputActionValue &Value);


private:
	int VectorToNumPadSector(FVector2D Vector);
};
