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

UENUM(BlueprintType)
enum class EInputType : uint8
{
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
	FQCL UMETA(DisplayName = "Quarter Circle Forward Light"),
	BQCL UMETA(DisplayName = "Quarter Circle Backward Light"),
	FQCH UMETA(DisplayName = "Quarter Circle Forward Heavy"),
	BQCH UMETA(DisplayName = "Quarter Circle Backward Heavy")
};

UCLASS()
class FGTEST_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFighterController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (AllowPrivateAccess = "true"))
	EInputType PolledInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;

	static constexpr float NeutralThreshold = 0.5;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnMovePressed(const FInputActionValue &Value);
	void OnJumpPressed(const FInputActionValue &Value);
	void OnLightAttackPressed(const FInputActionValue &Value);
	void OnFireballPressed();

private:
	const TArray<EInputType> LeftSideNumPad = { EInputType::UP, EInputType::UPRIGHT, EInputType::RIGHT, 
												EInputType::DOWNRIGHT, EInputType::DOWN, EInputType::DOWNLEFT, 
												EInputType::LEFT, EInputType::UPLEFT };
	const TArray<EInputType> RightSideNumPad = { EInputType::UP, EInputType::UPLEFT, EInputType::LEFT,
												 EInputType::DOWNLEFT, EInputType::DOWN, EInputType::DOWNRIGHT, 
												 EInputType::RIGHT, EInputType::UPRIGHT };
	const int BufferMaxCapacity = 16;
	const int InputBufferLifespan = 25; /* max number of frames input can remain in buffer */
	
	int FramesSinceLastInput;

	EInputType VectorToNumPadSector(FVector2D Vector);
	void PopulateInputBuffer();
	void CheckForSequence();
	void HandleInputTimeout();
	bool IsSubSequence(TArray<EInputType> Sequence, int Lenience);

	TArray<EInputType> InputBuffer;
};
