// Copyright Epic Games, Inc. All Rights Reserved.

#include "AtlantisCharacter.h"
#include "AtlantisProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "AtlantisWeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AAtlantisCharacter::AAtlantisCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetIsReplicated(true);

	debug = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("debugCube"));
	debug->SetupAttachment(GetCapsuleComponent());
	debug->SetEnableGravity(false);
	debug->bOwnerNoSee = true;
	debug->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	debug->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	debug->CanCharacterStepUpOn = ECB_No;

	// Create a CameraComponent	
	firstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	firstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	firstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	firstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	mesh1P->SetOnlyOwnerSee(true);
	mesh1P->SetupAttachment(firstPersonCameraComponent);
	mesh1P->bCastDynamicShadow = false;
	mesh1P->CastShadow = false;
	mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetCapsuleComponent()->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_No;

	bAlwaysRelevant = true;

	UAtlantisWeaponComponent* primary = CreateDefaultSubobject<UAtlantisWeaponComponent>(TEXT("primary"));
	primary->SetupAttachment(GetMesh1P(), FName(TEXT("GripPoint")));
	primary->Character = this;
	weapons.Add(primary);

	UAtlantisWeaponComponent* secondary = CreateDefaultSubobject<UAtlantisWeaponComponent>(TEXT("secondary"));
	secondary->SetupAttachment(GetMesh1P(), FName(TEXT("GripPoint")));
	secondary->Character = this;
	secondary->SetVisibility(false);
	weapons.Add(secondary);

	currentWeaponIndex = 0;
}

void AAtlantisCharacter::NotifyControllerChanged() {
	Super::NotifyControllerChanged();

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}
}

void AAtlantisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AAtlantisCharacter::Move);
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AAtlantisCharacter::Look);
		EnhancedInputComponent->BindAction(fireAction, ETriggerEvent::Started, this, &AAtlantisCharacter::Fire);
		EnhancedInputComponent->BindAction(switchWeaponAction, ETriggerEvent::Started, this, &AAtlantisCharacter::SwitchWeapon);
		EnhancedInputComponent->BindAction(reloadAction, ETriggerEvent::Started, this, &AAtlantisCharacter::Reload);
	} else {
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AAtlantisCharacter::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAtlantisCharacter::Look(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAtlantisCharacter::Fire() {
	if(weapons[currentWeaponIndex] == nullptr)
		return;

	weapons[currentWeaponIndex]->Fire();
}

void AAtlantisCharacter::Reload() {
	weapons[currentWeaponIndex]->Reload();
}

void AAtlantisCharacter::SwitchWeapon() {
	int newIndex = (currentWeaponIndex + 1) % weapons.Num();
	RequestSwitchWeapon(newIndex);

	if (!HasAuthority()) {
		HandleClientSideSwitchWeapon(newIndex);
	}
}

void AAtlantisCharacter::HandleClientSideSwitchWeapon(int newWeaponIndex) {
	if (newWeaponIndex == currentWeaponIndex)
		return;

	weapons[currentWeaponIndex]->SetVisibility(false);
	weapons[newWeaponIndex]->SetVisibility(true);
	currentWeaponIndex = newWeaponIndex;

	NotifySwitchWeapon();
}

void AAtlantisCharacter::RequestSwitchWeapon_Implementation(int newWeaponIndex) {
	if (newWeaponIndex < 0 || newWeaponIndex >= weapons.Num())
		return;

	ReplicateSwitchWeapon(newWeaponIndex);
}

void AAtlantisCharacter::ReplicateSwitchWeapon_Implementation(int newWeaponIndex) {
	HandleClientSideSwitchWeapon(newWeaponIndex);
}