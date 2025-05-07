// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PressurePlates/PressurePlate.h"
#include "GatePlate.generated.h"

UCLASS()
class ATLANTIS_API AGatePlate : public APressurePlate {
	GENERATED_BODY()
public:
	AGatePlate();
protected:
	UFUNCTION()
	virtual void Tick(float deltaTime) override;

private:
	bool IsRaised();

	UPROPERTY(EditAnywhere, Category = Gate);
	AActor* gate;

	UPROPERTY(EditAnywhere, Category = Gate)
	float raiseAmount;

	UPROPERTY(EditAnywhere, Category = Gate)
	float lowerAmount;
	
	UPROPERTY(EditAnywhere, Category = Gate)
	float lowestZPos;

	UPROPERTY(EditAnywhere, Category = Gate)
	float highestZPos;
};
