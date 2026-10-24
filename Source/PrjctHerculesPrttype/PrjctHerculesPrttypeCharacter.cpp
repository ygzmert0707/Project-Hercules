// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrjctHerculesPrttypeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PrjctHerculesPrttype.h"
#include "ItemPickup.h"

APrjctHerculesPrttypeCharacter::APrjctHerculesPrttypeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APrjctHerculesPrttypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrjctHerculesPrttypeCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APrjctHerculesPrttypeCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrjctHerculesPrttypeCharacter::Look);

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(
				InteractAction, ETriggerEvent::Started,
				this, &APrjctHerculesPrttypeCharacter::Interact);
		}
	}
	else
	{
		UE_LOG(LogPrjctHerculesPrttype, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APrjctHerculesPrttypeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APrjctHerculesPrttypeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APrjctHerculesPrttypeCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void APrjctHerculesPrttypeCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APrjctHerculesPrttypeCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void APrjctHerculesPrttypeCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void APrjctHerculesPrttypeCharacter::Interact()
{
	UE_LOG(LogPrjctHerculesPrttype, Log, TEXT("Interact pressed"));
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AItemPickup::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		AItemPickup* Pickup = Cast<AItemPickup>(Actor);

		if (Pickup)
		{
			Pickup->TryInteract(this);
			break;
		}
	}
}

bool APrjctHerculesPrttypeCharacter::AddItem(FName ItemId)
{
	if (ItemId.IsNone())
	{
		return false;
	}

	Inventory.Add(ItemId);
	UE_LOG(LogPrjctHerculesPrttype, Log, TEXT("Item added to inventory"));
	return true;
}

bool APrjctHerculesPrttypeCharacter::EquipItem(FName ItemId)
{
	if (ItemId.IsNone() || !Inventory.Contains(ItemId))
	{
		return false;
	}

	EquippedItem = ItemId;
	OnEquippedItemChanged(ItemId);
	return true;
}

bool APrjctHerculesPrttypeCharacter::AssignItemToSlot(FName ItemId, int32 SlotNumber)
{
	if (ItemId.IsNone() || !Inventory.Contains(ItemId))
	{
		return false;
	}

	if (SlotNumber != 1 && SlotNumber != 2)
	{
		return false;
	}

	FName& TargetSlot = (SlotNumber == 1) ? QuickSlot1Item : QuickSlot2Item;
	FName& OtherSlot = (SlotNumber == 1) ? QuickSlot2Item : QuickSlot1Item;

	if (TargetSlot == ItemId)
	{
		return false;
	}

	if (OtherSlot == ItemId)
	{
		FName TempItem = TargetSlot;
		TargetSlot = ItemId;
		OtherSlot = TempItem;
	}
	else
	{
		TargetSlot = ItemId;
	}

	UE_LOG(LogTemp, Warning, TEXT("Slot1: %s | Slot2: %s"),
		*QuickSlot1Item.ToString(),
		*QuickSlot2Item.ToString());

	return true;
}

bool APrjctHerculesPrttypeCharacter::ClearQuickSlot(int32 SlotNumber)
{
	if (SlotNumber == 1)
	{
		QuickSlot1Item = NAME_None;
	}
	else if (SlotNumber == 2)
	{
		QuickSlot2Item = NAME_None;
	}
	else
	{
		return false;
	}

	return true;
}

FName APrjctHerculesPrttypeCharacter::GetQuickSlot1Item() const
{
	return QuickSlot1Item;
}

FName APrjctHerculesPrttypeCharacter::GetQuickSlot2Item() const
{
	return QuickSlot2Item;
}

void APrjctHerculesPrttypeCharacter::AddUpgradeCurrency(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	UpgradeCurrency += Amount;
	UE_LOG(LogPrjctHerculesPrttype, Log, TEXT("Upgrade currency total: %d"), UpgradeCurrency);
	UE_LOG(LogPrjctHerculesPrttype, Log, TEXT("Upgrade currency added: %d"), Amount);
}

int32 APrjctHerculesPrttypeCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 APrjctHerculesPrttypeCharacter::GetUpgradeCurrency() const
{
	return UpgradeCurrency;
}

bool APrjctHerculesPrttypeCharacter::UpgradeStat(ECharacterStat Stat)
{
	const int32 Cost = GetUpgradeCost();

	if (UpgradeCurrency < Cost)
	{
		return false;
	}

	switch (Stat)
	{
	case ECharacterStat::Strength:
		Strength++;
		break;

	case ECharacterStat::Dexterity:
		Dexterity++;
		break;

	case ECharacterStat::Agility:
		Agility++;
		break;

	case ECharacterStat::Thickness:
		Thickness++;
		break;

	case ECharacterStat::Vigor:
		Vigor++;
		break;

	case ECharacterStat::Endurance:
		Endurance++;
		break;

	case ECharacterStat::Pace:
		Pace++;
		break;

	case ECharacterStat::MagicalCapability:
		MagicalCapability++;
		break;

	case ECharacterStat::Intelligence:
		Intelligence++;
		break;

	case ECharacterStat::Faith:
		Faith++;
		break;

	default:
		return false;
	}

	UpgradeCurrency -= Cost;
	CharacterLevel++;

	UE_LOG(LogPrjctHerculesPrttype, Log, TEXT("Stat upgraded. Level: %d | Currency: %d"), CharacterLevel, UpgradeCurrency);

	return true;
}
int32 APrjctHerculesPrttypeCharacter::GetStatValue(ECharacterStat Stat) const
{
	switch (Stat)
	{
	case ECharacterStat::Strength:
		return Strength;

	case ECharacterStat::Dexterity:
		return Dexterity;

	case ECharacterStat::Agility:
		return Agility;

	case ECharacterStat::Thickness:
		return Thickness;

	case ECharacterStat::Vigor:
		return Vigor;

	case ECharacterStat::Endurance:
		return Endurance;

	case ECharacterStat::Pace:
		return Pace;

	case ECharacterStat::MagicalCapability:
		return MagicalCapability;

	case ECharacterStat::Intelligence:
		return Intelligence;

	case ECharacterStat::Faith:
		return Faith;

	default:
		return 0;
	}
}

int32 APrjctHerculesPrttypeCharacter::GetUpgradeCost() const
{
	const int32 LevelsGained = CharacterLevel - 1;
	const int32 UpgradeCost = 100 + 20 * LevelsGained + 2 * LevelsGained * LevelsGained;
	return UpgradeCost;
}