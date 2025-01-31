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

	// Equipment Tags
	FGameplayTag Equipment_Helmet;
	FGameplayTag Equipment_Chest;
	FGameplayTag Equipment_Gloves;
	FGameplayTag Equipment_Legs;
	FGameplayTag Equipment_Boots;
	FGameplayTag Equipment_Necklace;
	FGameplayTag Equipment_Ring;
	FGameplayTag Equipment_Belt;
	FGameplayTag Equipment_Weapon;
	
	// Damage Types TODO: Update to Damage_Type_xxx 
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

	// Debuffs
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Burn_Chance;
	FGameplayTag Debuff_Burn_Frequency;
	FGameplayTag Debuff_Burn_Duration;

	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Stun_Chance;
	FGameplayTag Debuff_Stun_Frequency;
	FGameplayTag Debuff_Stun_Duration;

	FGameplayTag Debuff_Slow;
	FGameplayTag Debuff_Slow_Chance;
	FGameplayTag Debuff_Slow_Duration;
	FGameplayTag Debuff_Slow_Magnitude;
	

	// Debuff Statistics
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Damage_Fire;
	

	TArray<FGameplayTag> DebuffTypes;
	TArray<FGameplayTag> DebuffsEffectedByTenacity;
	TMap<FGameplayTag, FGameplayTag> DebuffDamageTypesToResistances;
	
	// Debuff Damage Multipliers
	FGameplayTag Debuff_DamageMultiplier_Source_AttackPower;
	FGameplayTag Debuff_DamageMultiplier_Source_MaxHealth;
	FGameplayTag Debuff_DamageMultiplier_Source_Defense;

	FGameplayTag Debuff_DamageMultiplier_Target_MaxHealth;

	TMap<FGameplayTag, FGameplayTag> DebuffDamageMultipliersToSourceAttributes;
	TMap<FGameplayTag, FGameplayTag> DebuffDamageMultipliersToTargetAttributes;

	// Damage Multipliers TODO: Update name to start with Damage_
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
	FGameplayTag Abilities_Ice_IceShards;
	
	// Cooldowns
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Ice_IceShards;

	// Passive Spell Abilities
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	

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

	// Player (PlayerController, PlayerState, ...)
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
	FGameplayTag Player_Block_Movement;
	FGameplayTag Player_Block_Rotation;


private:
	static FRpgGameplayTags GameplayTags;
	
};
