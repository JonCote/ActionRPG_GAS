// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "RpgEnemySpawnPoint.generated.h"

class ARpgEnemy;
/**
 * 
 */
UCLASS()
class AURA_API ARpgEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Properties")
	TSubclassOf<ARpgEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Properties")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Properties")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	
};
