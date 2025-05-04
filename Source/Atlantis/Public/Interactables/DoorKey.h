// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "DoorKey.generated.h"

class UStaticMeshComponent;

// Dev note: Cant use AKey as FKey exists and UE reflection cant have two things named the 'same'
// so having to do ADoorKey even though may be used for stuff outside doors

UCLASS()
class ATLANTIS_API ADoorKey : public ABaseInteractable {
	GENERATED_BODY()
public:
	ADoorKey();

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) override;
private:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* staticMesh;
	
	UPROPERTY(EditAnywhere, Category = Key)
	FName keyId;
};
