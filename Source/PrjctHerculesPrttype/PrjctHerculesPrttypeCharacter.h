// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PrjctHerculesPrttypeCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterStat : uint8
{
	Strength,
	Dexterity,
	Agility,
	Thickness,
	Vigor,
	Endurance,
	Pace,
	MagicalCapability,
	Intelligence,
	Faith,
};

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class APrjctHerculesPrttypeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FName EquippedItem = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FName QuickSlot1Item = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	FName QuickSlot2Item = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 UpgradeCurrency = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Strength = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Dexterity = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Agility = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Thickness = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Vigor = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Endurance = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Pace = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 MagicalCapability = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Intelligence = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	int32 Faith = 1;
public:

	/** Constructor */
	APrjctHerculesPrttypeCharacter();	
	bool AddItem(FName ItemId);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool EquipItem(FName ItemId);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnEquippedItemChanged(FName ItemId);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AssignItemToSlot(FName ItemId, int32 SlotNumber);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool ClearQuickSlot(int32 SlotNumber);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	FName GetQuickSlot1Item() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	FName GetQuickSlot2Item() const;

	UFUNCTION(BlueprintPure, Category = "Progression")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddUpgradeCurrency(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Progression")
	int32 GetUpgradeCurrency() const;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	bool UpgradeStat(ECharacterStat Stat);

	UFUNCTION(BlueprintPure, Category = "Progression")
	int32 GetStatValue(ECharacterStat Stat) const;

	UFUNCTION(BlueprintPure, Category = "Progression")
	int32 GetUpgradeCost() const;
protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

