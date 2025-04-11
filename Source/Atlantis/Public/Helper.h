// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helper.generated.h"

class AAtlantisCharacter;
struct FBlueprintSessionResult;

UCLASS()
class ATLANTIS_API UHelper : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Online|Session")
	static FString GetSessionName(const FBlueprintSessionResult& Result);

	UFUNCTION(BlueprintPure, Category = "Online")
	static bool IsControlledByLocalPlayer(const UWorld* world, const AAtlantisCharacter* character);
};
