// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LayerWidget.generated.h"

class URpgUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API ULayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	int32 PushToStack(URpgUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	URpgUserWidget* PopFromStack();

	UFUNCTION(BlueprintCallable)
	URpgUserWidget* PeekStack() const;

	UFUNCTION(BlueprintCallable)
	bool StackIsEmpty() const;

	UFUNCTION(BlueprintCallable)
	void ClearStack();

	UFUNCTION(BlueprintCallable)
	TArray<URpgUserWidget*> GetStack() const;

private:

	UPROPERTY()
	TArray<URpgUserWidget*> Stack;
	
};
