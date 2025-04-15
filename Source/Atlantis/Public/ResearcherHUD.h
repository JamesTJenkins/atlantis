// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlantisHUD.h"
#include "ResearcherHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDMessage, const FText&, text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotify);

UCLASS()
class ATLANTIS_API AResearcherHUD : public AAtlantisHUD {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnHUDMessage OnTranslateStart;

	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnNotify OnTranslateComplete;
};
