// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Atlantis/Public/PlayerRole.h"
#include "Online/CoreOnline.h"
#include "AtlantisPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintPlayerStateUpdate, const AAtlantisPlayerState*, playerState);

UCLASS()
class ATLANTIS_API AAtlantisPlayerState : public APlayerState {
	GENERATED_BODY()
public:
	AAtlantisPlayerState();

	void InitPlayerState();

	UFUNCTION(BlueprintCallable, Category = Network)
	bool CheckIfRoleTaken(const EPlayerRole roleToCheck, FUniqueNetIdRepl& OutNetId);

	// PROPERTYS

	UPROPERTY(BlueprintAssignable, Category = Network)
	FBlueprintPlayerStateUpdate OnRoleUpdate;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetPlayerRole(EPlayerRole newRole);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetPlayerReady(bool ready);

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerRole, BlueprintReadOnly, Category = Gameplay)
	EPlayerRole playerRole;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerReady, BlueprintReadOnly, Category = Gameplay)
	bool playerReady;

	UFUNCTION()
	void OnRep_PlayerRole();

	UFUNCTION()
	void OnRep_PlayerReady();

	// RPCs

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Network)
	void RequestClaimRole(const EPlayerRole roleToClaim);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Network)
	void RequestPlayerReady(const bool ready);
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(class APlayerState* PlayerState) override;
	virtual void OverrideWith(class APlayerState* PlayerState) override;
private:
	TSubclassOf<AHUD> bodyguardHUD;
	TSubclassOf<AHUD> researcherHUD;
};
