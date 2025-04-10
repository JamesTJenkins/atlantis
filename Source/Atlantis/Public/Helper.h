// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FindSessionsCallbackProxy.h"
#include "Helper.generated.h"

UCLASS()
class ATLANTIS_API UHelper : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "Online|Session")
	static FString GetSessionName(const FBlueprintSessionResult& Result);
};
