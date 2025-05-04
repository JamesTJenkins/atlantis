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

UAtlantisWeaponComponent::UAtlantisWeaponComponent() {
	muzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	damagePerShot = 10;
	currentAmmoInMag = 1;
	maxAmmoPerMag = 1;
	requestedReload = false;
}

void UAtlantisWeaponComponent::Fire() {
	if (currentAmmoInMag == 0) {
		Reload();	// Maybe make this an option in settings
		return;
	}

	APlayerController* playerController = Cast<APlayerController>(Character->GetController());
	const FRotator cameraRotation = playerController->PlayerCameraManager->GetCameraRotation();
	RequestFire(cameraRotation);
	HandleClientSideFire(cameraRotation);
}

void UAtlantisWeaponComponent::RequestFire_Implementation(const FRotator& cameraRotation) {
	FireComplete(cameraRotation);
}

void UAtlantisWeaponComponent::FireComplete_Implementation(const FRotator& cameraRotation) {
	// Check if charcter is played by local player as we will skip client side fire if so as already done
	if(UHelper::IsControlledByLocalPlayer(GetWorld(), Character))
		return;

	HandleClientSideFire(cameraRotation);
}

void UAtlantisWeaponComponent::HandleClientSideFire(const FRotator& cameraRotation) {
	currentAmmoInMag--;

	UWorld* const World = GetWorld();
	if(World == nullptr || projectileClass == nullptr)
		return;

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

void UAtlantisWeaponComponent::RequestReload_Implementation() {
	HandleClientSideReload();

	// TODO: Wait for reload complete animation or do a animation event
	// call ReloadComplete afterwards to update self and client but for now we just call immediately
	ReloadComplete();
}

void UAtlantisWeaponComponent::ReloadComplete_Implementation() {
	currentAmmoInMag = maxAmmoPerMag;
	requestedReload = false;
}

void UAtlantisWeaponComponent::HandleClientSideReload() {
	// TODO: Do animation stuff here
}