// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FighterController.generated.h"

/**
 * @brief Construct a new UENUM object
 * 
 */

UENUM(BlueprintType)
enum class EInputType : uint8
{
	/* Simple Inputs */
	UP UMETA(DisplayName = "Up"),
	DOWN UMETA(DisplayName = "Down"),
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right"),
	UPLEFT UMETA(DisplayName = "Up-Left"),
	UPRIGHT UMETA(DisplayName = "Up-Right"),
	DOWNLEFT UMETA(DisplayName = "Down-Left"),
	DOWNRIGHT UMETA(DisplayName = "Down-Right"),
	NEUTRAL UMETA(DisplayName = "Neutral"),
	LB UMETA(DisplayName = "Light Button"),
	HB UMETA(DisplayName = "Heavy Button"),

	/* Complex Inputs */
	FQCL UMETA(DisplayName = "Quarter Circle Forward Light"),
	BQCL UMETA(DisplayName = "Quarter Circle Backward Light"),
	FQCH UMETA(DisplayName = "Quarter Circle Forward Heavy"),
	BQCH UMETA(DisplayName = "Quarter Circle Backward Heavy"),
	DPL UMETA(DisplayName = "Dragon Punch Light"),
	DPH UMETA(DisplayName = "Dragon Punch Heavy"),
	DASH UMETA(DisplayName = "Dash"),
	BACKDASH UMETA(DisplayName = "Back Dash")
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MID UMETA(DisplayName = "Mid Attack"),
	HIGH UMETA(DisplayName = "High Attack"),
	LOW UMETA(DisplayName = "Low Attack")
};

UCLASS()
class FGTEST_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFighterController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FlushBuffer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	EInputType PolledInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* HeavyAttackAction;

	static constexpr float NeutralThreshold = 0.5;

	void CheckForSequence();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void SendInputToUI(EInputType Input);

	void OnMovePressed(const FInputActionValue &Value);
	void OnLightAttackPressed(const FInputActionValue &Value);
	void OnHeavyAttackPressed(const FInputActionValue &Value);

private:
	const TArray<EInputType> LeftSideNumPad = { EInputType::UP, EInputType::UPRIGHT, EInputType::RIGHT, 
												EInputType::DOWNRIGHT, EInputType::DOWN, EInputType::DOWNLEFT, 
												EInputType::LEFT, EInputType::UPLEFT };
	const TArray<EInputType> RightSideNumPad = { EInputType::UP, EInputType::UPLEFT, EInputType::LEFT,
												 EInputType::DOWNLEFT, EInputType::DOWN, EInputType::DOWNRIGHT, 
												 EInputType::RIGHT, EInputType::UPRIGHT };
	const TMap<EInputType, TArray<EInputType>> MotionInputs = {
		{EInputType::FQCL, {EInputType::DOWN, EInputType::DOWNRIGHT, EInputType::RIGHT, EInputType::LB} },
		{EInputType::FQCH, {EInputType::DOWN, EInputType::DOWNLEFT, EInputType::LEFT, EInputType::HB} },
		{EInputType::BQCL, {EInputType::DOWN, EInputType::DOWNRIGHT, EInputType::RIGHT, EInputType::LB} },
		{EInputType::BQCH, {EInputType::DOWN, EInputType::DOWNLEFT, EInputType::LEFT, EInputType::HB} },
		{EInputType::DPL, {EInputType::RIGHT, EInputType::DOWN, EInputType::DOWNRIGHT, EInputType::LB} },
		{EInputType::DPH, {EInputType::RIGHT, EInputType::DOWN, EInputType::DOWNRIGHT, EInputType::HB} },
		{EInputType::DASH, {EInputType::RIGHT, EInputType::NEUTRAL, EInputType::RIGHT} },
		{EInputType::BACKDASH, {EInputType::LEFT, EInputType::NEUTRAL, EInputType::LEFT} },
	};
	const int BufferMaxCapacity = 16;
	const int InputBufferLifespan = 25; /* max number of frames input can remain in buffer */
	const int DefaultInputLenience = 2;
	
	int FramesSinceLastInput;

	EInputType VectorToNumPadSector(FVector2D Vector);
	void PopulateInputBuffer();
	void HandleInputTimeout();
	bool IsSubSequence(TArray<EInputType> Sequence, int Lenience);

	TArray<EInputType> InputBuffer;
};
