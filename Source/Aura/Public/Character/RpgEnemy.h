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

	//~ Begin Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Enemy Interface

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel() override;
	//~ End Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

private:
	
};
