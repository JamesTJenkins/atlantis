// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "LockedDoor.generated.h"

class UStaticMeshComponent;

UCLASS()
class ATLANTIS_API ALockedDoor : public ABaseInteractable {
	GENERATED_BODY()
public:
	ALockedDoor();

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) override;
private:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, Category = Key)
	FName requiredKeyId;
};
