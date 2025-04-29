// Copyright James Jenkins. All Rights Reserved.

#include "Widgets/CommsWidget.h"

FText UCommsWidget::GetNextDialog() {
	currentDialogIndex++;
	return GetDialog(currentDialogIndex - 1);
}

FText UCommsWidget::GetPreviousDialog() {
	currentDialogIndex--;
	return GetDialog(currentDialogIndex + 1);
}

FText UCommsWidget::GetDialog(int dialogIndex) {
	if(dialogIndex < 0 || dialogIndex > GetDialogLength()) {
		UE_LOG(LogTemp, Error, TEXT("Comm Dialog out of bounds"));
	}
	
	FString string;
	dialogStringTable->GetStringTable()->GetSourceString(FString::FromInt(dialogIndex), string);
	FText newText = FText::FromString(string);
	return newText;
}

bool UCommsWidget::Initialize() {
	currentDialogIndex = 0;
	length = 0;

	if (dialogStringTable != nullptr) {
		int numKeys = 0;
		dialogStringTable->GetStringTable()->EnumerateKeysAndSourceStrings([&numKeys](const FTextKey& Key, const FString& SourceString) -> bool {
			numKeys++;
			return true; // Continue enumeration
		});
		this->length = numKeys;
	}

	return Super::Initialize();
}
