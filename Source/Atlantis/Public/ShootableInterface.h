// Copyright James Jenkins. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShootableInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UShootableInterface : public UInterface {
	GENERATED_BODY()
};

class IShootableInterface {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	virtual void OnHit(UPrimitiveComponent* hitComponent) = 0;
};
