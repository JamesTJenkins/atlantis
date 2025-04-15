// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "../Plugins/Experimental/Text3D/Source/Text3D/Public/Text3DComponent.h"
#include "LanguageTranslate.generated.h"

UCLASS()
class ATLANTIS_API ALanguageTranslate : public ABaseInteractable {
	GENERATED_BODY()
public:	
	ALanguageTranslate();

	UFUNCTION(BlueprintCallable, Category = Language)
	void SetNormalText(const FText& newText);

	UFUNCTION(BlueprintCallable, Category = Language)
	void Update3DText();

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void UpdateInteractionBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Language)
	FText normalText;

	virtual void OnInteract(AAtlantisCharacter* playerCharacter) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) override;
#endif
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Language, meta = (AllowPrivateAccess = "true"))
	UText3DComponent* worldText;
};
