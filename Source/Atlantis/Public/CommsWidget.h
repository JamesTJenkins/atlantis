// Copyright James Jenkins. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Internationalization/StringTable.h"
#include "CommsWidget.generated.h"

UCLASS()
class ATLANTIS_API UCommsWidget : public UUserWidget {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Dialog)
	FText GetNextDialog();

	UFUNCTION(BlueprintCallable, Category = Dialog)
	FText GetPreviousDialog();

	UFUNCTION(BlueprintCallable, Category = Dialog)
	FText GetDialog(int dialogIndex);

	UFUNCTION(BlueprintCallable, Category = Dialog)
	inline int GetDialogLength() { return length; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialog)
	UStringTable* dialogStringTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialog)
	int currentDialogIndex;
protected:
	virtual bool Initialize() override;
private:
	int length;
};
