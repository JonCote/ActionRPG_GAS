// Copyright Jonathan Cote


#include "AbilitySystem/Passive/PassiveSpellNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveSpellNiagaraComponent::UPassiveSpellNiagaraComponent()
{
	bAutoActivate = false;
	
}

void UPassiveSpellNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		RpgASC->ActivePassiveEffectDelegate.AddUObject(this, &UPassiveSpellNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				RpgASC->ActivePassiveEffectDelegate.AddUObject(this, &UPassiveSpellNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveSpellNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
