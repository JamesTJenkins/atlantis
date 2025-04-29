// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollingTextWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleteScrolling);

UCLASS(Abstract)
class ATLANTIS_API UScrollingTextWidget : public UUserWidget {
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* textBlock;

	UPROPERTY(BlueprintAssignable)
	FOnCompleteScrolling onComplete;

	UFUNCTION(BlueprintCallable, Category = Text)
	void SetScrollingText(const FText& text);
protected:
	virtual bool Initialize() override;
private:
	void UpdateScrollingText();

	FText completedText;
	int32 scrollIndex;
	FTimerHandle timerHandle;
	FTimerManagerTimerParameters params;
};
