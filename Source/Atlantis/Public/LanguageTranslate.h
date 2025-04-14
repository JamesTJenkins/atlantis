// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Plugins/Experimental/Text3D/Source/Text3D/Public/Text3DComponent.h"
#include "LanguageTranslate.generated.h"

UCLASS()
class ATLANTIS_API ALanguageTranslate : public AActor
{
	GENERATED_BODY()
public:	
	ALanguageTranslate();

	UFUNCTION(BlueprintCallable, Category = Language)
	void SetNormalText(const FText& newText);

	UFUNCTION(BlueprintCallable, Category = Language)
	void Update3DText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Language)
	FText normalText;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) override;
#endif
private:
	UText3DComponent* worldText;
};
