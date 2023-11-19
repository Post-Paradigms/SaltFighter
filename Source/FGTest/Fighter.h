// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Containers/RingBuffer.h"
#include "FighterController.h"
#include "Fighter.generated.h"

UENUM(BlueprintType)
enum class EFighterState : uint8 {
	NEUTRAL UMETA(DisplayName = "Neutral"),
	JUMPING UMETA(DisplayName = "Jumping"),
	DEFENDING UMETA(DisplayName = "Defending"),
	STARTUP UMETA(DisplayName = "Startup"),
	ACTIVE UMETA(DisplayName = "Active"),
	RECOVERY UMETA(DisplayName = "Recovery"),
	HITSTUN UMETA(DisplayName = "Hitstun"),
	BLOCKSTUN UMETA(DisplayName = "Blockstun")
	//crouching
	//crouch blocking
	//might need a walking state for 
};

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
	int PolledInput;

	UPROPERTY()
	bool Locked;

	UPROPERTY()
	int32 FrameTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void MoveEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintCallable)
	void JumpEvent(const FInputActionValue &Value);



	// Component getters
	FORCEINLINE class UCameraComponent *GetCameraComponent() const { return CameraComponent; }
    FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFighterState State;

	UFUNCTION()
	void TakeInInput(int32 KeypadNum);

private:
	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

	// Facing
	void Face();


	UFUNCTION()
	bool UpdateState(EFighterState NewState);

	UFUNCTION()
	void FrameAdvanceState();

	UFUNCTION()
	void OnHitOther();

	UFUNCTION()
	void OnOw();
};
