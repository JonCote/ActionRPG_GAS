// Copyright Jonathan Cote

#include "Character/RpgCharacterBase.h"


ARpgCharacterBase::ARpgCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ARpgCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARpgCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


