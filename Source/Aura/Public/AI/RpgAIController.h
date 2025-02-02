// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RpgAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;


/**
 * 
 */
UCLASS()
class AURA_API ARpgAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARpgAIController();


private:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	
};
