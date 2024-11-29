// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Character/RpgCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "RpgEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ARpgEnemy : public ARpgCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ARpgEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
};
