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
	FQCL UMETA(DisplayName = "Quarter Circle Forward Light"),
	BQCL UMETA(DisplayName = "Quarter Circle Backward Light"),
	FQCH UMETA(DisplayName = "Quarter Circle Forward Heavy"),
	BQCH UMETA(DisplayName = "Quarter Circle Backward Heavy"),
	LB UMETA(DisplayName = "Light Button"),
	HB UMETA(DisplayName = "Heavy Button")
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
	int PolledInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	int BufferMaxCapacity;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnMovePressed(const FInputActionValue &Value);
	void OnJumpPressed(const FInputActionValue &Value);
	void OnLightAttackPressed(const FInputActionValue &Value);
	void OnFireballPressed();

private:
	const TArray<int> LeftSideNumPad = { 8, 9, 6, 3, 2, 1, 4, 7 };
	const TArray<int> RightSideNumPad = { 8, 7, 4, 1, 2, 3, 6, 9 };

	int VectorToNumPadSector(FVector2D Vector);
	void PopulateInputBuffer();
	void CheckForSequence();
	bool IsSubSequence(TArray<int> Sequence, int Lenience);

	TArray<int> InputBuffer;
};
