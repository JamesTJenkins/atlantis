// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisWeaponComponent.h"
#include "AtlantisCharacter.h"
#include "AtlantisProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

UAtlantisWeaponComponent::UAtlantisWeaponComponent() {
	muzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	currentAmmoInMag = 1;
	maxAmmoPerMag = 1;
}

void UAtlantisWeaponComponent::Fire() {
	UWorld* const World = GetWorld();
	if (World == nullptr || projectileClass == nullptr)
		return;

	if (currentAmmoInMag == 0) {
		Reload();	// Maybe make this an option in settings
		return;
	}

	currentAmmoInMag--;
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(muzzleOffset);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	World->SpawnActor<AAtlantisProjectile>(projectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if(fireSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, Character->GetActorLocation());
	}

	// Maybe move this out of c++ and into bp like other animation stuff
	if(fireAnimation != nullptr) {
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if(AnimInstance != nullptr) {
			AnimInstance->Montage_Play(fireAnimation, 1.f);
		}
	}
}

void UAtlantisWeaponComponent::Reload() {
	currentAmmoInMag = maxAmmoPerMag;
}