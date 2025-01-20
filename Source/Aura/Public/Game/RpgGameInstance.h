// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RpgGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URpgGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

	void UpdatePlayerStartTag(const FName& InPlayerStartTag);

	
	
};
