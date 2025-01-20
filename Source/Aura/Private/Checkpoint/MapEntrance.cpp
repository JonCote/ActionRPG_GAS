// Copyright Jonathan Cote


#include "Checkpoint/MapEntrance.h"

#include "Game/RpgGameInstance.h"
#include "Game/RpgGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// save world state for world we are leaving
			RpgGameMode->SaveWorldState(GetWorld());
		}
		
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag, DestinationMap.LoadSynchronous());
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
		
	}
}
