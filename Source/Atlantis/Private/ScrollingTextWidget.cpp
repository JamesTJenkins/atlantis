// Copyright James Jenkins. All Rights Reserved.

#include "ScrollingTextWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UScrollingTextWidget::SetScrollingText(const FText& text) {
	completedText = text;
	scrollIndex = 1;

	if(timerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}

	UpdateScrollingText();
}

bool UScrollingTextWidget::Initialize() {
	params.bLoop = false;
	params.bMaxOncePerFrame = true;
	params.FirstDelay = 0.1f;
	return Super::Initialize();
}

void UScrollingTextWidget::UpdateScrollingText() {
	FString scrolledString = completedText.ToString().Mid(0, scrollIndex);
	textBlock->SetText(FText::FromString(scrolledString));

	scrollIndex++;

	if (scrollIndex <= completedText.ToString().Len()) {
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UScrollingTextWidget::UpdateScrollingText, 0.1f, params);
	} else {
		onComplete.Broadcast();
	}
}
