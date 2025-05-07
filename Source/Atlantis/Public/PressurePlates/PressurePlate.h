// Copyright James Jenkins All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class ATLANTIS_API APressurePlate : public AActor {
	GENERATED_BODY()
public:
	APressurePlate();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PressurePlate, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* triggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PressurePlate, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* pressurePlateMesh;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int pressed;
};
