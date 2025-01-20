// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "RpgSpawnVolume.generated.h"

class ARpgEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API ARpgSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ARpgSpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<ARpgEnemySpawnPoint*> SpawnPoints;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
};
