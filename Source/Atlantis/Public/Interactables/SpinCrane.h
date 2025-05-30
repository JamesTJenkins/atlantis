// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "ShootableInterface.h"
#include "SpinCrane.generated.h"

class UStaticMeshComponent;

UCLASS()
class ATLANTIS_API ASpinCrane : public ABaseInteractable, public IShootableInterface {
	GENERATED_BODY()
public:
	ASpinCrane();

	virtual void OnInteractHold(AAtlantisCharacter* playerCharacter, float deltaTime) override;

	virtual void Tick(float deltaTime) override;

	virtual void OnHit(UPrimitiveComponent* hitComponent) override;

	UFUNCTION(NetMulticast, Unreliable)
	void UpdateLoadLocation(const float zLocation);
private:
	bool IsRaised();
	bool CanRotate(float yaw);

	UPROPERTY(VisibleAnywhere, Category = Mesh);
	UStaticMeshComponent* craneBottom;

	UPROPERTY(VisibleAnywhere, Category = Mesh);
	UStaticMeshComponent* craneTop;

	UPROPERTY(VisibleAnywhere, Category = Mesh);
	UStaticMeshComponent* craneLoad;

	UPROPERTY(VisibleAnywhere, Category = Interaction);
	UBoxComponent* rotateRightHitbox;

	UPROPERTY(VisibleAnywhere, Category = Interaction);
	UBoxComponent* rotateLeftHitbox;

	UPROPERTY(EditAnywhere, Category = Crane)
	float raiseAmount;

	UPROPERTY(EditAnywhere, Category = Crane)
	float lowerAmount;

	UPROPERTY(EditAnywhere, Category = Crane)
	float lowestLoadZPos;

	UPROPERTY(EditAnywhere, Category = Crane)
	float highestLoadZPos;

	UPROPERTY(EditAnywhere, Category = Crane)
	float minCraneYawRotation;

	UPROPERTY(EditAnywhere, Category = Crane)
	float maxCraneYawRotation;

	bool currentlyInteracting;
};
