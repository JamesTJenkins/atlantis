// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AtlantisPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8 { 
	None,
	Bodyguard, 
	Researcher 
};

UCLASS()
class ATLANTIS_API AAtlantisPlayerState : public APlayerState {
	GENERATED_BODY()
public:
	AAtlantisPlayerState();

	void InitPlayerState();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetPlayerRole(EPlayerRole newRole);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Gameplay)
	EPlayerRole playerRole;
protected:
	virtual void OnRep_PlayerRole();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
