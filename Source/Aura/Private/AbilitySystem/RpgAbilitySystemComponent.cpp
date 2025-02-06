// Copyright Jonathan Cote


#include "AbilitySystem/RpgAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/RpgGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/RpgLogChannels.h"
#include "Game/LoadScreenSaveGame.h"
#include "Interaction/PlayerInterface.h"


void URpgAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URpgAbilitySystemComponent::ClientEffectApplied);
}

void URpgAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{

	for (const FSavedAbility& AbilityData : SaveData->SavedAbilities)
	{
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = AbilityData.GameplayAbilityClass;

		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, AbilityData.AbilityLevel);
		LoadedAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilitySlot);
		LoadedAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilityStatus);

		GiveAbility(LoadedAbilitySpec);
		
		if (AbilityData.AbilityType.MatchesTagExact(GameplayTags.Abilities_Type_Passive) && AbilityData.AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
		{
			TryActivateAbility(LoadedAbilitySpec.Handle);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void URpgAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const URpgGameplayAbility* RpgAbility = Cast<URpgGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(RpgAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FRpgGameplayTags::Get().Abilities_Status_Equipped);
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
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(FRpgGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbility(AbilitySpec);
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void URpgAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
	}
}

void URpgAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
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

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			//InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey());
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
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag URpgAbilitySystemComponent::GetSlotTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
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
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
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

FGameplayTag URpgAbilitySystemComponent::GetSlotTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetSlotTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* URpgAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* URpgAbilitySystemComponent::GetSpecFromSlotTag(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(SlotTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool URpgAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, SlotTag))
		{
			return false;
		}
	}
	return true;
}

bool URpgAbilitySystemComponent::IsSlotOnCooldown(const FGameplayTag& SlotTag, const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromSlotTag(SlotTag))
	{
		if (!Spec->Ability->CheckCooldown(Spec->Handle, ActorInfo)) { return true; }
	}
	return false;
}

bool URpgAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(SlotTag);
}

bool URpgAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

bool URpgAbilitySystemComponent::AbilityOnCooldown(const FGameplayAbilitySpec& AbilitySpec,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& AbilityTag)
{
	return !GetSpecFromAbilityTag(AbilityTag)->Ability->CheckCooldown(AbilitySpec.Handle, ActorInfo);
}

bool URpgAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const UAbilityInfo* AbilityInfo = URpgAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(GetAbilityTagFromSpec(AbilitySpec)).AbilityTypeTag;
	return AbilityType.MatchesTagExact(FRpgGameplayTags::Get().Abilities_Type_Passive);
}

void URpgAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
	ClearAbilitySlot(&AbilitySpec);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(SlotTag);

	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	if (GetStatusTagFromSpec(AbilitySpec).MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		AbilitySpec.GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Unlocked);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);
	}
}

void URpgAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivePassiveEffectDelegate.Broadcast(AbilityTag, bActivate);
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
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(StatusTag);
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
		const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

		// Check if Selected Ability is on Cooldown
		if (AbilityOnCooldown(*AbilitySpec, ActorInfo, AbilityTag)) { return; }
		

		/* Equip the ability */
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		const FGameplayTag& PrevSlotTag = GetSlotTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromSpec(*AbilitySpec);

		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			
			if (!SlotIsEmpty(SlotTag))
			{
				// Check if Selected Slot has an ability that is on Cooldown
				if (IsSlotOnCooldown(SlotTag, ActorInfo)) { return; }
				
				// There is an ability in this slot already. Deactivate and clear its slot
				if (FGameplayAbilitySpec* SpecWithSlotTag = GetSpecFromSlotTag(SlotTag))
				{
					// is Selected ability the same as slot ability? If so, return
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlotTag))) { return; }

					// Is slot ability a passive ability? If so, deactivate it
					if (IsPassiveAbility(*SpecWithSlotTag))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlotTag), false);
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*SpecWithSlotTag));
					}

					ClearAbilitySlot(SpecWithSlotTag);
				}
				
			}
			
			if (!AbilityHasAnySlot(*AbilitySpec))	// Ability doesn't yet have a slot (it's not active)
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusTagFromSpec(*AbilitySpec));
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			AssignSlotToAbility(*AbilitySpec, SlotTag);
			
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
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FRpgGameplayTags::Get().Abilities_Status_Eligible);
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

void URpgAbilitySystemComponent::ClearAbilitySlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag Slot = GetSlotTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Slot);

	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	if (GetStatusTagFromSpec(*AbilitySpec).MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Equipped);
		AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
	}
	
}

// TODO: Remove this function since not used
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

bool URpgAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag : AbilitySpec->GetDynamicSpecSourceTags())
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



