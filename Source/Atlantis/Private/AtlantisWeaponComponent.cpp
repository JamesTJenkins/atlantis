// Copyright James Jenkins. All Rights Reserved.

#include "AtlantisWeaponComponent.h"
#include "Player/AtlantisCharacter.h"
#include "AtlantisProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Helper.h"
#include "TimerManager.h"

UAtlantisWeaponComponent::UAtlantisWeaponComponent() {
	muzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	damagePerShot = 10;
	currentAmmoInMag = 1;
	maxAmmoPerMag = 1;
	timeBetweenShots = 1.0f / 6.0f;
	timeToReload = 4.0f;
	requestedReload = false;
	midShot = false;
}

void UAtlantisWeaponComponent::Fire() {
	if (currentAmmoInMag == 0) {
		Reload();	// Maybe make this an option in settings
		return;
	}

	// Clientside check
	if (midShot || requestedReload)
		return;

	APlayerController* playerController = Cast<APlayerController>(Character->GetController());
	const FRotator cameraRotation = playerController->PlayerCameraManager->GetCameraRotation();
	RequestFire(cameraRotation);
	HandleClientSideFire(cameraRotation);
}

void UAtlantisWeaponComponent::RequestFire_Implementation(const FRotator& cameraRotation) {
	// Serverside check
	if (midShot || requestedReload)
		return;

	FireComplete(cameraRotation);
}

void UAtlantisWeaponComponent::FireComplete_Implementation(const FRotator& cameraRotation) {
	// Check if charcter is played by local player as we will skip client side fire if so as already done
	if(UHelper::IsControlledByLocalPlayer(GetWorld(), Character))
		return;

	HandleClientSideFire(cameraRotation);
}

void UAtlantisWeaponComponent::HandleClientSideFire(const FRotator& cameraRotation) {
	UWorld* const World = GetWorld();
	if(World == nullptr || projectileClass == nullptr)
		return;

	currentAmmoInMag--;
	StartShotTimer();

	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector spawnLocation = GetOwner()->GetActorLocation() + cameraRotation.RotateVector(muzzleOffset);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAtlantisProjectile* projectile = World->SpawnActor<AAtlantisProjectile>(projectileClass, spawnLocation, cameraRotation, ActorSpawnParams);
	projectile->damage = damagePerShot;

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
	if(requestedReload || currentAmmoInMag == maxAmmoPerMag)
		return;

	requestedReload = true;
	RequestReload();

	if(!Character->HasAuthority()) {
		HandleClientSideReload();
	}
}

// TODO: may want to change this to animation events rather than set timer but this will be fine
void UAtlantisWeaponComponent::RequestReload_Implementation() {
	HandleClientSideReload();
	StartReloadTimer();
}

void UAtlantisWeaponComponent::ReloadComplete_Implementation() {
	currentAmmoInMag = maxAmmoPerMag;
	requestedReload = false;
}

void UAtlantisWeaponComponent::HandleClientSideReload() {
	// TODO: Do animation stuff here
}

void UAtlantisWeaponComponent::StartShotTimer() {
	midShot = true;
	GetWorld()->GetTimerManager().SetTimer(shotTimerHandle, this, &UAtlantisWeaponComponent::FinishedShot, timeBetweenShots, false);
}

void UAtlantisWeaponComponent::FinishedShot() {
	midShot = false;
}

void UAtlantisWeaponComponent::StartReloadTimer() {
	GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &UAtlantisWeaponComponent::ReloadComplete, timeToReload, false);
}