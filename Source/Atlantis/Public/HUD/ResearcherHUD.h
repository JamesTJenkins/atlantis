// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlantisHUD.h"
#include "ResearcherHUD.generated.h"

UCLASS()
class ATLANTIS_API AResearcherHUD : public AAtlantisHUD {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnHUDMessage OnTranslateStart;

	UPROPERTY(BlueprintAssignable, Category = UI)
	FOnNotify OnTranslateComplete;
};
