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

	UPROPERTY(BlueprintAssignable, Category = Replication)
	FBlueprintPlayerStateUpdate OnRoleUpdate;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetPlayerRole(EPlayerRole newRole);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Network)
	void RequestClaimRole(const EPlayerRole roleToClaim);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool CheckIfRoleTaken(const EPlayerRole roleToCheck, FUniqueNetIdRepl& OutNetId);

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerRole, BlueprintReadOnly, Category = Gameplay)
	EPlayerRole playerRole;

	UFUNCTION()
	void OnRep_PlayerRole();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
