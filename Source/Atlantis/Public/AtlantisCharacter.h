// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AtlantisCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UAtlantisWeaponComponent;
class ABaseInteractable;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AAtlantisCharacter : public ACharacter {
	GENERATED_BODY()
public:
	AAtlantisCharacter();

	USkeletalMeshComponent* GetMesh1P() const {
		return mesh1P;
	}

	UCameraComponent* GetFirstPersonCameraComponent() const {
		return firstPersonCameraComponent;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* firstPersonCameraComponent;

	// Temp so can see player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* debug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* fireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* switchWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* reloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* interactAction;
	
	// Base Character Variables and functions

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TArray<UAtlantisWeaponComponent*> weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	int currentWeaponIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	float interactDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Air)
	float oxygenZoneCount;	// This uses an int instead of bool so player can move between oxygen zones without weird problems

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Air)
	float oxygenRegenPerSecond;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Air)
	float oxygenLossPerSecond;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MaxOxygen, VisibleAnywhere, BlueprintReadWrite, Category = Air)
	float maxOxygen;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Oxygen, VisibleAnywhere, BlueprintReadWrite, Category = Air)
	float oxygen;

	UFUNCTION()
	void OnRep_Oxygen();

	UFUNCTION()
	void OnRep_MaxOxygen();
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Weapons)
	void NotifySwitchWeapon();

	void SwitchWeapon();
	void HandleClientSideSwitchWeapon(int newWeaponIndex);

	UFUNCTION(Server, Unreliable)
	void RequestSwitchWeapon(int newWeaponIndex);

	UFUNCTION(NetMulticast, Reliable)
	void ReplicateSwitchWeapon(int newWeaponIndex);

	void Interact();

	// Theres no replicate callback as interaction will be handled by the actor that is being interacted with instead
	UFUNCTION(Server, Unreliable)
	void RequestInteract();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire();
	void Reload();

	ABaseInteractable* GetInteractable();

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void Tick(float deltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

