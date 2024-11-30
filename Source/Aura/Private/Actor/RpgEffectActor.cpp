// Copyright Jonathan Cote


#include "Actor/RpgEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "Components/SphereComponent.h"


ARpgEffectActor::ARpgEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ARpgEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	///TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const URpgAttributeSet* RpgAttributeSet = Cast<URpgAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(URpgAttributeSet::StaticClass()));
		
		URpgAttributeSet* MutableRpgAttributeSet = const_cast<URpgAttributeSet*>(RpgAttributeSet);
		MutableRpgAttributeSet->SetHealth(RpgAttributeSet->GetHealth() + 25.f);
		MutableRpgAttributeSet->SetMana(RpgAttributeSet->GetMana() - 15.f);
		Destroy();
	}
}

void ARpgEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ARpgEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARpgEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ARpgEffectActor::EndOverlap);
}


