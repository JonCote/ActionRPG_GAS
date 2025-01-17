// Copyright Jonathan Cote


#include "AbilitySystem/Passive/PassiveSpellNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RpgGameplayTags.h"
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
		ActivateIfEquipped(RpgASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				RpgASC->ActivePassiveEffectDelegate.AddUObject(this, &UPassiveSpellNiagaraComponent::OnPassiveActivate);
				ActivateIfEquipped(RpgASC);
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

void UPassiveSpellNiagaraComponent::ActivateIfEquipped(URpgAbilitySystemComponent* RpgASC)
{
	if (RpgASC->bStartupAbilitiesGiven)
	{
		if (RpgASC->GetStatusFromAbilityTag(PassiveSpellTag).MatchesTagExact(FRpgGameplayTags::Get().Abilities_Status_Equipped))
		{
			Activate();
		}
	}
}
