// Copyright Jonathan Cote


#include "RpgGameplayTags.h"
#include "GameplayTagsManager.h"

FRpgGameplayTags FRpgGameplayTags::GameplayTags;

void FRpgGameplayTags::InitNativeGameplayTags()
{
	// Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Primary.Strength"),
													FString("Greatly increases Attack Power")
													);
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Primary.Intelligence"),
													FString("Increases Attack Power, Max Mana, and Mana Regeneration")
													);
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Primary.Resilience"),
													FString("Increases Max Health and Health Regeneration")
													);
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Primary.Dexterity"),
													FString("Increases Critical Hit Rate and Stamina Regeneration")
													);
	
	
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.MaxHealth"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.HealthRegeneration"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.MaxMana"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.ManaRegeneration"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.MaxStamina"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.StaminaRegeneration"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.AttackPower"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.Defense"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_CriticalHitRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.CriticalHitRate"),
													FString("")
													);
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Secondary.CriticalHitDamage"),
													FString("")
													);

	
	// Input keys
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.LMB"),
													FString("")
													);
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.RMB"),
													FString("")
													);
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.1"),
													FString("")
													);
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.2"),
													FString("")
													);
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.3"),
													FString("")
													);
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.4"),
													FString("")
													);

	// Damage Types
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage"),
													FString("Damage")
													);
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage.Fire"),
													FString("Fire Damage Type")
													);
	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage.Ice"),
													FString("Ice Damage Type")
													);
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage.Lightning"),
													FString("Lightning Damage Type")
													);
	GameplayTags.Damage_Curse = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage.Curse"),
													FString("Curse Damage Type")
													);
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Damage.Physical"),
													FString("Physical Damage Type")
													);

	// Damage Resistance Types
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Resistance.Fire"),
													FString("Resistance to Fire Damage")
													);
	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Resistance.Ice"),
													FString("Resistance to Ice Damage")
													);
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Resistance.Lightning"),
													FString("Resistance to Lightning Damage")
													);
	GameplayTags.Attributes_Resistance_Curse = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Resistance.Curse"),
													FString("Resistance to Curse Damage")
													);
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Resistance.Physical"),
													FString("Resistance to Physical Damage")
													);

	// Map of Damage Types to Resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Ice, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Curse, GameplayTags.Attributes_Resistance_Curse);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	
	// Abilities
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Attack"),
													FString("Attack Ability Tag")
													);
	
	
	// Effects
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Effects.HitReact"),
													FString("Reaction effect for being hit")
													);
	
}
