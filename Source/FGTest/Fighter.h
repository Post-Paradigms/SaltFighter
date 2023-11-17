// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FighterController.h"
#include "Fighter.generated.h"

UCLASS()
class FGTEST_API AFighter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFighter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
    class AFighter* OtherPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
    class AFighterController* OurController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsLeftSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	int CurrentSector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input response events
	UFUNCTION(BlueprintCallable)
	void TestEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void MoveEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void VectorMoveEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void JumpEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void UpEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void DownEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void LeftEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void RightEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void DownRightEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void LightAttackEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void FireballEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void CommandNormalLightEvent(const FInputActionValue &Value);

	// Component getters
	FORCEINLINE class UCameraComponent *GetCameraComponent() const { return CameraComponent; }
    FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }

private:
	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

	// Facing
	void Face();

	// Converts input to num pad notation
	int VectorToNumPadSector(FVector2D Vector);
};
