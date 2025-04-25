// Copyright James Jenkins. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class ICharacterInterface {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	virtual void UpdateHealth(float value) = 0;
};
