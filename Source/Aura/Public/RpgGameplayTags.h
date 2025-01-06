// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * RpgGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FRpgGameplayTags
{
public:
	static const FRpgGameplayTags& Get() { return GameplayTags; }
	static void InitNativeGameplayTags();

	// Primary Attributes
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Dexterity;
	
	// Secondary Attributes
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;
	FGameplayTag Attributes_Secondary_AttackPower;
	FGameplayTag Attributes_Secondary_Defense;
	FGameplayTag Attributes_Secondary_CriticalHitRate;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;

	// Meta Attributes
	FGameplayTag Attributes_Meta_IncomingXP;
	
	// Input keys
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	// Damage Types
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Curse;
	FGameplayTag Damage_Physical;
	
	// Damage Type Resistances
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Curse;
	FGameplayTag Attributes_Resistance_Physical;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	// Damage Multipliers
	FGameplayTag DamageMultiplier_Source_AttackPower;
	FGameplayTag DamageMultiplier_Source_MaxHealth;
	FGameplayTag DamageMultiplier_Source_Defense;

	FGameplayTag DamageMultiplier_Target_MaxHealth;

	TMap<FGameplayTag, FGameplayTag> DamageMultipliersToSourceAttributes;
	TMap<FGameplayTag, FGameplayTag> DamageMultipliersToTargetAttributes;
	

	// Abilities
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_None;
	
	// Ability Status
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Equipped;
	
	// Ability Types
	FGameplayTag Abilities_Type_Active;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	// Gameplay Abilities
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;

	
	// Cooldowns
	FGameplayTag Cooldown_Fire_FireBolt;
	
	

	// CombatSockets
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	// Montages
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	// Effects
	FGameplayTag Effects_HitReact;


private:
	static FRpgGameplayTags GameplayTags;
	
};
