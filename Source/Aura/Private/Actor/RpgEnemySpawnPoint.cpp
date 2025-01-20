// Copyright Jonathan Cote


#include "Actor/RpgEnemySpawnPoint.h"

#include "Character/RpgEnemy.h"

void ARpgEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARpgEnemy* Enemy = GetWorld()->SpawnActorDeferred<ARpgEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
