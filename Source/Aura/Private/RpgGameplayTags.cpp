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
	
}
