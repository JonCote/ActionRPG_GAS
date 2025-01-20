// Copyright Jonathan Cote


#include "Actor/RpgSpawnVolume.h"

#include "Actor/RpgEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"


ARpgSpawnVolume::ARpgSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ARpgSpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}


void ARpgSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARpgSpawnVolume::OnBoxOverlap);
	
}

void ARpgSpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	
	bReached = true;
	for (ARpgEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


