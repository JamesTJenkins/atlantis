// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "AtlantisWeaponComponent.generated.h"

class AAtlantisCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATLANTIS_API UAtlantisWeaponComponent : public USkeletalMeshComponent {
	GENERATED_BODY()
public:
	UAtlantisWeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(Server, Unreliable)
	void RequestFire(const FRotator& cameraRotation);

	UFUNCTION(NetMulticast, Reliable)
	void FireComplete(const FRotator& cameraRotation);

	UFUNCTION(Server, Reliable)
	void RequestReload();

	UFUNCTION(NetMulticast, Reliable)
	void ReloadComplete();

	void HandleClientSideFire(const FRotator& cameraRotation);
	
	void HandleClientSideReload();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AAtlantisProjectile> projectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* fireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* fireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector muzzleOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int currentAmmoInMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int maxAmmoPerMag;

	bool requestedReload;

	AAtlantisCharacter* Character;
};
