// Copyright James Jenkins. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PlayerRole.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8 {
	None,
	Bodyguard,
	Researcher
};
