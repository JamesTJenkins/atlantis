// Copyright James Jenkins All Rights Reserved.

#include "Player/AtlantisCharacter.h"
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
#include "Net/UnrealNetwork.h"
#include "Interactables/BaseInteractable.h"
#include "AtlantisDefines.h"
#include "Interactables/Carriable.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AAtlantisCharacter::AAtlantisCharacter() {
	PrimaryActorTick.bCanEverTick = true;

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

	Tags.Add(PLAYER_TAG);

	currentCarriable = nullptr;
	interactHold = false;
	movementEnabled = true;
	maxHealth = 100;
	health = 100;
	currentWeaponIndex = 0;
	interactDistance = 500;
	maxOxygen = 100;
	oxygen = 100;
	oxygenLossPerSecond = 1;
	oxygenRegenPerSecond = 10;
	holdForceStrength = 25;
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
		EnhancedInputComponent->BindAction(interactAction, ETriggerEvent::Started, this, &AAtlantisCharacter::Interact);
		EnhancedInputComponent->BindAction(interactAction, ETriggerEvent::Completed, this, &AAtlantisCharacter::ReleaseInteract);
	}
}

void AAtlantisCharacter::Tick(float deltaTime) {
	if(HasAuthority()) {
		if (oxygenZoneCount > 0) {
			oxygen = FMath::Clamp(oxygen + (oxygenRegenPerSecond * deltaTime), -1, maxOxygen);
			//UE_LOG(LogTemp, Log, TEXT("Gaining air %f"), oxygen);
		} else {
			oxygen = FMath::Clamp(oxygen - (oxygenLossPerSecond * deltaTime), -1, maxOxygen);
			//UE_LOG(LogTemp, Log, TEXT("Losing air %f"), oxygen);
			
			if(oxygen < 0) {
				// TODO: come back once the player can actually die and respawn
				UE_LOG(LogTemp, Log, TEXT("Died of lack of oxygen"));
			}
		}

		if(interactHold) {
			if(ABaseInteractable* interactable = GetInteractable()) {
				interactable->OnInteractHold(this, deltaTime);
			}
		}

		if(currentCarriable) {
			FVector targetLoc = firstPersonCameraComponent->GetComponentLocation() + firstPersonCameraComponent->GetForwardVector() * carriableHoldDistance;
			FVector moveDir = targetLoc - currentCarriable->GetActorLocation();
			float distance = moveDir.Size();

			if (distance > 5) {
				moveDir.Normalize();
				float forceMagnitude = distance * holdForceStrength;
				currentCarriable->AddForce(moveDir * forceMagnitude);
			}
		}
	}

	if (IsLocallyControlled()) {
		RequestUpdateCameraPitch(firstPersonCameraComponent->GetComponentRotation().Pitch);
	}
}

void AAtlantisCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAtlantisCharacter, oxygen);
	DOREPLIFETIME(AAtlantisCharacter, maxOxygen);
	DOREPLIFETIME(AAtlantisCharacter, health);
	DOREPLIFETIME(AAtlantisCharacter, maxHealth);
}

void AAtlantisCharacter::Move(const FInputActionValue& Value) {
	if (!movementEnabled)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAtlantisCharacter::Look(const FInputActionValue& Value) {
	if(!movementEnabled)
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAtlantisCharacter::OnRep_MaxHealth() {
	// TODO: will have to do UI
}

void AAtlantisCharacter::OnRep_Health() {
	// TODO: will have to do UI
}

void AAtlantisCharacter::OnRep_Oxygen() {
	// TODO: will have to do UI
}

void AAtlantisCharacter::OnRep_MaxOxygen() {
	// TODO: will have to do UI
}

void AAtlantisCharacter::UpdateHealth(float value) {
	health += value;

	if(health <= value) {
		// TODO: implement respawns or revive mechanic
		UE_LOG(LogTemp, Log, TEXT("Player died"));
	}
}

void AAtlantisCharacter::AddKeyId(FName id) {
	currentKeyIds.Add(id);
	// TODO: update UI to display key
}

bool AAtlantisCharacter::CheckForId(FName id) {
	return currentKeyIds.Contains(id);
}

void AAtlantisCharacter::PickupCarriable(ACarriable* carriable) {
	RequestPickupCarriable(carriable);

	if(!HasAuthority()) {
		HandleClientSidePickupCarriable(carriable);
	}
}

void AAtlantisCharacter::HandleClientSidePickupCarriable(ACarriable* carriable) {
	if (carriable == currentCarriable)
		return;

	if (carriable) {
		weapons[currentWeaponIndex]->SetVisibility(false);
		carriableHoldDistance = FVector::Distance(firstPersonCameraComponent->GetComponentLocation(), carriable->GetActorLocation());
		carriable->EnableDampening(true);
	} else {
		currentCarriable->EnableDampening(false);
		weapons[currentWeaponIndex]->SetVisibility(true);
	}

	currentCarriable = carriable;
	NotifySwitchWeapon();
}

void AAtlantisCharacter::RequestPickupCarriable_Implementation(ACarriable* carriable) {
	ReplicatePickupCarriable(carriable);
}

void AAtlantisCharacter::ReplicatePickupCarriable_Implementation(ACarriable* carriable) {
	HandleClientSidePickupCarriable(carriable);
}

void AAtlantisCharacter::Fire() {
	if(weapons[currentWeaponIndex] == nullptr || currentCarriable != nullptr)
		return;

	weapons[currentWeaponIndex]->Fire();
}

void AAtlantisCharacter::Reload() {
	if (!currentCarriable)
		return;

	weapons[currentWeaponIndex]->Reload();
}

void AAtlantisCharacter::RequestUpdateCameraPitch_Implementation(const float newPitch) {
	ReplicateUpdatedCameraPitch(newPitch);
}

void AAtlantisCharacter::ReplicateUpdatedCameraPitch_Implementation(const float newPitch) {
	if(!IsLocallyControlled()) {
		FRotator newRotation = firstPersonCameraComponent->GetComponentRotation();
		newRotation.Pitch = newPitch;
		firstPersonCameraComponent->SetWorldRotation(newRotation);
	}
}

ABaseInteractable* AAtlantisCharacter::GetInteractable() {
	FVector start = firstPersonCameraComponent->GetComponentLocation();
	FVector end = start + firstPersonCameraComponent->GetForwardVector() * interactDistance;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FHitResult result;
	if(GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, params)) {
		if (result.GetComponent()->ComponentHasTag(INTERACT_TAG)) {
			return Cast<ABaseInteractable>(result.GetActor());
		}
	}

	return nullptr;
}

void AAtlantisCharacter::Interact() {
	if(currentCarriable) {
		PickupCarriable(nullptr);
		return;
	}

	RequestInteract();
}

void AAtlantisCharacter::ReleaseInteract() {
	RequestInteractRelease();
}

void AAtlantisCharacter::RequestInteract_Implementation() {
	if(ABaseInteractable* interactable = GetInteractable()) {
		interactable->OnInteract(this);
	}

	interactHold = true;
}

void AAtlantisCharacter::RequestInteractRelease_Implementation() {
	interactHold = false;
}

void AAtlantisCharacter::SwitchWeapon() {
	if(currentCarriable) {
		PickupCarriable(nullptr);
	}

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