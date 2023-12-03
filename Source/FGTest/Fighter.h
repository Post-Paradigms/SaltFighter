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
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
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
	EAttackType AttackType;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float KnockbackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float KnockbackForce;
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
	KNOCKDOWN UMETA(DisplayName = "Knocked Down"),
	CROUCHING UMETA(DisplayName = "Crouching"),
	CROUCHBLOCKING UMETA(DisplayName = "Crouch Blocking")
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
	uint32 ComboCounter;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	int32 MaxJumps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	int32 MaxAirDashes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UDataTable* FighterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* JumpSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* LandedSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* LightAttackCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* HeavyAttackCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* LightHitCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* HeavyHitCue;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump(EInputType Input);

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void MoveEvent(const FInputActionValue &Value);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMontage(UAnimMontage* Montage);

	UFUNCTION(BlueprintImplementableEvent)
	void StopMontage();

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

	void OnOw(FAttackStruct* OwCauser);

private:
	const TMap<EInputType, int> JumpDirections = {
		{EInputType::UPLEFT, -1},
		{EInputType::UP,      0},
		{EInputType::UPRIGHT, 1},
	};

	const FVector JumpVector = FVector(400.f, 0.f, 1100.f);

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

	FAttackStruct* CurrAttk;

	int32 NumAirDashes;

	int32 NumJumps;

	class AHitbox* ActiveHitbox;

	class UAnimInstance* AnimInstance;

	// Facing
	void Face();

	UFUNCTION()
	void PerformNormal(FName AttkName);

	UFUNCTION()
	void PerformSpecial(FName SpecialName);
	
	UFUNCTION()
	void PerformDash(bool Back);

	UFUNCTION()
	void FrameAdvanceState();

	UFUNCTION()
	void StartHitStop(float Duration);

	UFUNCTION()
	void StopHitStop();

	FTimerHandle HitStopTimerHandler;

	

	// Fighter Move Functions owo
	void LightNormal(bool Target);
	void HeavyNormal(bool Target);

	void LightQuarterCircleForward();
	void HeavyQuarterCircleForward();

	void LightQuarterCircleBack();
	void HeavyQuarterCircleBack();

	void LightDragonPunch();
	void HeavyDragonPunch();

	//Sound functions
	void LightAttackSound();
	void HeavyAttackSound();

	void LightHitSound();
	void HeavyHitSound();
};
