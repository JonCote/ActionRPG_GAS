// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();


	
protected:
	virtual void BeginPlay() override;

		
};
