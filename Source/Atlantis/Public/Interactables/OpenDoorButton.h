// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "OpenDoorButton.generated.h"

class UStaticMeshComponent;

UCLASS()
class ATLANTIS_API AOpenDoorButton : public ABaseInteractable {
	GENERATED_BODY()
public:
	AOpenDoorButton();

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) override;
private:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, Category = Target)
	AActor* door;
};
