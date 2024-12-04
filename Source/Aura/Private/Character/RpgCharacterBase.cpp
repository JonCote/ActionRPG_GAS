// Copyright Jonathan Cote

#include "Character/RpgCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"


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

void ARpgCharacterBase::InitAbilityActorInfo()
{
}

void ARpgCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ARpgCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalityAttributes, 1.f);
}

void ARpgCharacterBase::AddCharacterAbilities()
{
	URpgAbilitySystemComponent* RpgASC = CastChecked<URpgAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	RpgASC->AddCharacterAbilities(DefaultAbilities);

	
}




