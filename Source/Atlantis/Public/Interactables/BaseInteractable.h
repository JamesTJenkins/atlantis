// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Player/AtlantisCharacter.h"
#include "BaseInteractable.generated.h"

UCLASS()
class ATLANTIS_API ABaseInteractable : public AActor {
	GENERATED_BODY()
public:	
	ABaseInteractable();

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) {};
protected:
	UPROPERTY(VisibleAnywhere, Category = Interaction);
	UBoxComponent* interactBox;
};
