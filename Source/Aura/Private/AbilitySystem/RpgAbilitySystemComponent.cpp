// Copyright Jonathan Cote


#include "AbilitySystem/RpgAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/RpgLogChannels.h"
#include "Interaction/PlayerInterface.h"


void URpgAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URpgAbilitySystemComponent::ClientEffectApplied);
}

void URpgAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const URpgGameplayAbility* RpgAbility = Cast<URpgGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(RpgAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FRpgGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void URpgAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void URpgAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void URpgAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void URpgAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogRpg, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag URpgAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag URpgAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag URpgAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag URpgAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag URpgAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* URpgAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

bool URpgAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (AbilityTag.MatchesTagExact(FRpgGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
		OutNextLevelDescription = FString();
		return false;
	}
	

	const UAbilityInfo* AbilityInfo = URpgAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityInfo)
	{
		OutDescription = FString();
		OutNextLevelDescription = FString();
		return false;
	}
	
	const FString AbilityName = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityName;
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (URpgGameplayAbility* RpgAbility = Cast<URpgGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = RpgAbility->GetDescription(AbilityName, AbilitySpec->Level);
			OutNextLevelDescription = RpgAbility->GetDescription(AbilityName,AbilitySpec->Level + 1);
			return true;
		}
	}

	
	OutDescription = URpgGameplayAbility::GetLockedDescription(AbilityName, AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	OutNextLevelDescription = FString();

	return false;
}

void URpgAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (!GetAvatarActor()->Implements<UPlayerInterface>()) { return; }
	if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) <= 0) { return; }
	ServerUpgradeAttribute(AttributeTag);
	
}

void URpgAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (!GetAvatarActor()->Implements<UPlayerInterface>()) { return; }
	
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
}

void URpgAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			StatusTag = GameplayTags.Abilities_Status_Unlocked;
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(StatusTag);
			AbilitySpec->Level = 1;
			
		}
		else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		
		UpdateAbilityStatus(*AbilitySpec, AbilityTag, StatusTag);
	}
}

void URpgAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		/* Check If the ability and Slot are valid for rebinding (ability and ability in slot are not on a cooldown) */
		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySpec->Handle;
		const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

		// Check if Selected Ability is on Cooldown
		bool bNotOnCooldown = GetSpecFromAbilityTag(AbilityTag)->Ability->CheckCooldown(AbilitySpecHandle, ActorInfo);
		if (!bNotOnCooldown) { return; }

		// Check if Selected Slot has a ability that is on Cooldown
		for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
		{
			if (AbilityHasSlot(&Spec, SlotTag))
			{
				if (!Spec.Ability->CheckCooldown(Spec.Handle, ActorInfo)) { return; }
			}
		}

		/* Equip the ability */
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		const FGameplayTag& PrevSlotTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromSpec(*AbilitySpec);

		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			// Remove This InputTag (slot) from any Ability that has it
			ClearAbilitiesOfSlot(SlotTag);

			// Clear this Ability's slot, just in case, it's a different slot
			ClearAbilitySlot(AbilitySpec);

			// Now, assign Slot to this Ability
			AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);
			if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotTag, PrevSlotTag);
		}
	}
}


void URpgAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PrevSlotTag);
}

void URpgAbilitySystemComponent::ServerGetDescriptionsByAbilityTag_Implementation(const FGameplayTag& AbilityTag)
{
	FString Description = FString();
	FString NextLevelDescription = FString();
	GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	ClientGetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);

}

void URpgAbilitySystemComponent::ClientGetDescriptionsByAbilityTag_Implementation(const FGameplayTag& AbilityTag, const FString& Description, const FString& NextLevelDescription)
{
	AbilityDescriptionDelegate.Broadcast(AbilityTag, Description, NextLevelDescription);
}


/*
 * Update all Ability Statuses
**/
void URpgAbilitySystemComponent::UpdateAbilityStatuses(const int32 PlayerLevel)
{
	UAbilityInfo* AbilityInfo = URpgAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	check(AbilityInfo);
	
	for (const FRpgAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		// Give Ability when player achieves the abilities required level
		if (Info.AbilityTag.IsValid() &&
			PlayerLevel >= Info.LevelRequirement &&
			GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FRpgGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			UpdateAbilityStatus(AbilitySpec, Info.AbilityTag, FRpgGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

/*
 * Update a specific Ability Status
**/
void URpgAbilitySystemComponent::UpdateAbilityStatus(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec.Level);
	MarkAbilitySpecDirty(AbilitySpec);
}

void URpgAbilitySystemComponent::ClearAbilitySlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void URpgAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec: GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, SlotTag))
		{
			ClearAbilitySlot(&Spec);
		}
	}
}

bool URpgAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(SlotTag))
		{
			return true;
		}
	}
	return false;
}

void URpgAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void URpgAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void URpgAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
                                                                    FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}



