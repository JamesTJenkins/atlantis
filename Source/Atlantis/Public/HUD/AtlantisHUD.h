// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AtlantisHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDMessage, const FText&, text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotify);

UCLASS()
class ATLANTIS_API AAtlantisHUD : public AHUD {
	GENERATED_BODY()
public:
	// TODO: health bar, oxygen bar, etc.

	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnHUDMessage OnTempMessageStart;

	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnNotify OnTempMessageEnd;
};
