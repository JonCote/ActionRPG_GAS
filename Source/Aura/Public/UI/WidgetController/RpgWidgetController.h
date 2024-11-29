// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/Object.h"
#include "RpgWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URpgWidgetController : public UObject
{
	GENERATED_BODY()

public:

protected:

	UPROPERTY(BlueprintReadOnly, Category="Widget\|Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Widget\|Controller")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category="Widget\|Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Widget\|Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
};
