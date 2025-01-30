// Copyright Jonathan Cote


#include "Actor/LootableItem.h"

#include "Aura/RpgLogChannels.h"
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"


ALootableItem::ALootableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemMesh->SetEnableGravity(true);
	ItemMesh->SetSimulatePhysics(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(ItemMesh);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ALootableItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALootableItem::OnSphereOverlap);
	
}

void ALootableItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (!IPlayerInterface::Execute_LootItem(OtherActor, ItemName))
		{
			UE_LOG(LogRpg, Warning, TEXT("Looting of item [%s] failed!"), *ItemName);
		}
		else
		{
			Destroy();
		}
	}
}


