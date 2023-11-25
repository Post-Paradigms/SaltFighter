// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Containers/RingBuffer.h"
#include "Engine/DataTable.h"
#include "FighterController.h"
#include "Fighter.generated.h"

USTRUCT(BlueprintType)
struct FAttackStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Startup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Recovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Blockstun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Hitstun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool Knockdown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool JumpCancellable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool SpecialCancellable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool TargetComboable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EInputType NextTargetInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName NextTargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FVector HitboxLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FVector HitboxScale;
};

UENUM(BlueprintType)
enum class EFighterState : uint8 {
	NEUTRAL UMETA(DisplayName = "Neutral"),
	JUMPING UMETA(DisplayName = "Jumping"),
	FORWARDING UMETA(DisplayName = "Walking Forward"),
	DASHING UMETA(DisplayName = "Dashing"), 
	AIRDASHING UMETA(DisplayName = "Airdashing"),
	DEFENDING UMETA(DisplayName = "Defending"),
	STARTUP UMETA(DisplayName = "Startup"),
	ACTIVE UMETA(DisplayName = "Active"),
	RECOVERY UMETA(DisplayName = "Recovery"),
	HITSTUN UMETA(DisplayName = "Hitstun"),
	BLOCKSTUN UMETA(DisplayName = "Blockstun"),
	KNOCKDOWN UMETA(DisplayName = "Knocked Down")

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
	bool CanJumpCancel;

	UPROPERTY()
	bool CanSpecialCancel;

	UPROPERTY()
	bool CanTargetCombo;

	UPROPERTY()
	EInputType NextTargetInput;

	UPROPERTY()
	int32 FrameTimer;

	UPROPERTY()
	int32 AirDashFrames;

	UPROPERTY()
	int32 MaxAirDashes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UDataTable* FighterDataTable;

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

	// Used after attack/blockstun/hitstun
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFighterState PreviousState;

	UFUNCTION()
	void TakeInInput(EInputType Input);

	UFUNCTION()
	bool ValidateState(EFighterState NewState);

	UFUNCTION()
	void UpdateState(EFighterState NewState);

	UFUNCTION()
	void SetFrameTimer(int NumFrames);

	UFUNCTION()
	void OnHitOther();

	UFUNCTION()
	void OnOw(FAttackStruct OwCauser);

private:
	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

	FAttackStruct CurrAttk;

	int32 NumAirDashes;

	class AHitbox* ActiveHitbox;

	class UAnimInstance* AnimInstance;

	// Facing
	void Face();

	UFUNCTION()
	void PerformNormal(FName AttkName);

	UFUNCTION()
	void PerformSpecial(FName SpecialName);
	
	UFUNCTION()
	void PerformDash();

	UFUNCTION()
	void FrameAdvanceState();

	

	// Fighter Move Functions owo
	void LightNormal(bool Target);
	void HeavyNormal(EFighterState CurrentState);

	void LightQuarterCircleForward(EFighterState CurrentState);

};
