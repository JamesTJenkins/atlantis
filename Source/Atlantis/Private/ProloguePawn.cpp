// Copyright James Jenkins. All Rights Reserved.

#include "ProloguePawn.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AProloguePawn::AProloguePawn() {
	// Character rotation only changes in Yaw, to prevent the capsule from changing orientation.
	// Ask the Controller for the full rotation if desired (ie for aiming).
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	capsuleComponent->InitCapsuleSize(55.f, 96.0f);
	capsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	capsuleComponent->CanCharacterStepUpOn = ECB_No;
	capsuleComponent->SetShouldUpdatePhysicsVolume(true);
	capsuleComponent->SetCanEverAffectNavigation(false);
	capsuleComponent->bDynamicObstacle = true;
	capsuleComponent->SetIsReplicated(true);
	RootComponent = capsuleComponent;

	debug = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("debugCube"));
	debug->SetupAttachment(capsuleComponent);
	debug->SetEnableGravity(false);
	debug->bOwnerNoSee = true;
	debug->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	debug->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	debug->CanCharacterStepUpOn = ECB_No;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	cameraComponent->SetupAttachment(capsuleComponent);
	cameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	cameraComponent->bUsePawnControlRotation = true;

	minPitch = -30;
	maxPitch = 30;
	minYaw = -50;
	maxYaw = 50;
}

void AProloguePawn::NotifyControllerChanged() {
	Super::NotifyControllerChanged();

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}
}

void AProloguePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AProloguePawn::Look);
	} else {
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProloguePawn::Look(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		FRotator rot = cameraComponent->GetComponentRotation();
		float newYaw = rot.Yaw + LookAxisVector.X;
		float newPitch = rot.Pitch + LookAxisVector.Y;

		bool overMaxYaw = newYaw > maxYaw;
		bool overMinYaw = newYaw < minYaw;

		if((!overMaxYaw && !overMinYaw) || (overMaxYaw && LookAxisVector.X < 0) || (overMinYaw && LookAxisVector.X > 0)) {
			AddControllerYawInput(LookAxisVector.X);
		}
		
		bool overMaxPitch = newPitch > maxPitch;
		bool overMinPitch = newPitch < minPitch;

		if((!overMaxPitch && !overMinPitch) || (overMaxPitch && LookAxisVector.Y > 0) || (overMinPitch && LookAxisVector.Y < 0)) {
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}
