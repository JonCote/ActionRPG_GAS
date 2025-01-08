// Copyright Jonathan Cote


#include "AbilitySystem/RpgAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "RpgAbilityTypes.h"
#include "Net/UnrealNetwork.h"
#include "RpgGameplayTags.h"
#include "../../../../../../UE_5.3/Engine/Plugins/Editor/GameplayTagsEditor/Source/GameplayTagsEditor/Private/GameplayTagEditorUtilities.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "Aura/RpgLogChannels.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/RpgPlayerController.h"


URpgAttributeSet::URpgAttributeSet()
{
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
	
	//~ Primary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);

	//~ Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StaminaRegeneration, GetStaminaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_AttackPower, GetAttackPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Defense, GetDefenseAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitRate, GetCriticalHitRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);

	//~ Damage Type Resistances
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice, GetIceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Curse, GetCurseResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void URpgAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//~ Primary Attributes Rep Notify
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);


	//~ Secondary Attributes Rep Notify
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, CriticalHitRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);

	//~ Damage Type Resistances
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, CurseResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	//~ Vitality Attributes Rep Notify
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URpgAttributeSet, Stamina, COND_None, REPNOTIFY_Always);

}

void URpgAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void URpgAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (bTopOffHealth)
		{
			SetHealth(GetMaxHealth());
			bTopOffHealth = false;
		}
		else
		{
			// Add the additional Max Health gained to current Health
			SetHealth(GetHealth() + (NewValue - OldValue));
		}
	}
	if (Attribute == GetMaxManaAttribute())
	{
		if (bTopOffMana)
		{
			SetMana(GetMaxMana());
			bTopOffMana = false;
		}
		else
		{
			// Add the additional Max Health gained to current Health
			SetMana(GetMana() + (NewValue - OldValue));
		}
	}
}

void URpgAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = Causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) &&
		Props.SourceASC->AbilityActorInfo.IsValid() &&
		Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void URpgAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) { return; }

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
		
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void URpgAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage < 0.f) return;

	const float NewHealth = GetHealth() - LocalIncomingDamage;
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

	const bool bFatal = NewHealth <= 0.f;
	if (bFatal)
	{
		if (Props.TargetCharacter->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_Die(Props.TargetCharacter);
		}
		SendXPEvent(Props);
	}
	//else
	//{
	//	FGameplayTagContainer TagContainer;
	//	TagContainer.AddTag(FRpgGameplayTags::Get().Effects_HitReact);
	//	Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		
	//}
		
	const bool bCrit = URpgAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
	const bool bBlock = URpgAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
	ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCrit);

	if (URpgAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
	{
		HandleDebuff(Props);
	}

	/* DEBUG CODE: adds actor and all their tags to LogRpg */
	/*
	FGameplayTagContainer TestContainer;
	Props.TargetASC->GetOwnedGameplayTags(TestContainer);
	for (auto Tag : TestContainer)
	{
		UE_LOG(LogRpg, Log, TEXT("%s: %s"), *GetOwningActor()->GetName(), *Tag.ToString());
	}
	UE_LOG(LogRpg, Log, TEXT("--------"));
	*/
}

void URpgAttributeSet::HandleDebuff(const FEffectProperties& Props)
{
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceCharacter);

	const FGameplayTag DebuffTag = URpgAbilitySystemLibrary::GetDebuffTag(Props.EffectContextHandle);
	const float DebuffDamage = URpgAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = URpgAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = URpgAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DebuffTag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	Effect->Period = DebuffFrequency;
	Effect->bExecutePeriodicEffectOnApplication = false;

	// Grant Debuff Tag to Target
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& GrantTagComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(DebuffTag);
	TagContainer.CombinedTags.AddTag(DebuffTag);
	GrantTagComponent.SetAndApplyTargetTagChanges(TagContainer);
	
	if (!Effect->GetGrantedTags().HasTagExact(DebuffTag))
	{
		UE_LOG(LogRpg, Error, TEXT("Not Granting Debuff Tag: %s"), *DebuffTag.ToString());
	}

	Effect->StackingType = EGameplayEffectStackingType::AggregateByTarget;	//Might want AggregateBySource instead
	Effect->StackLimitCount = 1;
	Effect->StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;
	Effect->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;

	const int32 Ind = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Ind];
	
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();
	
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FRpgGameplayEffectContext* RpgContext = static_cast<FRpgGameplayEffectContext*>(MutableSpec->GetContext().Get());
		RpgContext->SetDebuffTag(MakeShareable(new FGameplayTag(DebuffTag)));

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void URpgAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const int32 LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
		
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP+LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;
			for (int32 i = 0; i < NumLevelUps; i++)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel+i);
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel+i);
			}

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
				
		}
			
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}


void URpgAttributeSet::ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit)
{
	// Check if damage is not to self
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (ARpgPlayerController* PC = Cast<ARpgPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (ARpgPlayerController* PC = Cast<ARpgPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void URpgAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = URpgAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
	
}

//~ Primary Attributes OnRep
void URpgAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Strength, OldStrength);
}

void URpgAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Intelligence, OldIntelligence);
}

void URpgAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Resilience, OldResilience);
}

void URpgAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Dexterity, OldDexterity);
}


//~ Secondary Attribute OnRep
void URpgAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, AttackPower, OldAttackPower);
}

void URpgAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Defense, OldDefense);
}

void URpgAttributeSet::OnRep_CriticalHitRate(const FGameplayAttributeData& OldCriticalHitRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, CriticalHitRate, OldCriticalHitRate);
}

void URpgAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void URpgAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, MaxHealth, OldMaxHealth);
}

void URpgAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void URpgAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, MaxMana, OldMaxMana);
}

void URpgAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void URpgAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, MaxStamina, OldMaxStamina);
}

void URpgAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
}

//~ Damage Type Resistance OnRep
void URpgAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, FireResistance, OldFireResistance);
}

void URpgAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, IceResistance, OldIceResistance);
}

void URpgAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, LightningResistance, OldLightningResistance);
}

void URpgAttributeSet::OnRep_CurseResistance(const FGameplayAttributeData& OldCurseResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, CurseResistance, OldCurseResistance);
}

void URpgAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, PhysicalResistance, OldPhysicalResistance);
}


//~ Vitality Attributes OnRep
void URpgAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Health, OldHealth);
}

void URpgAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Mana, OldMana);
}

void URpgAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URpgAttributeSet, Stamina, OldStamina);
}


