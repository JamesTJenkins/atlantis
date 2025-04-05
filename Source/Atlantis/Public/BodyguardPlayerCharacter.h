// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AtlantisCharacter.h"
#include "BodyguardPlayerCharacter.generated.h"

class UAtlantisWeaponComponent;

UCLASS()
class ATLANTIS_API ABodyguardPlayerCharacter : public AAtlantisCharacter {
	GENERATED_BODY()
public:
	ABodyguardPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	UAtlantisWeaponComponent* primary;
protected:
	virtual void Fire() override;
};
