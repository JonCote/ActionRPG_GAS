// Copyright Jonathan Cote


#include "RpgGameplayTags.h"

#include "AIController.h"
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
	
	// Meta Attributes
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Attributes.Meta.IncomingXP"),
													FString("Incoming XP Meta Attribute Tag")
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
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.Passive.1"),
													FString("Input Tag for Passive Ability 1")
													);
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("InputTag.Passive.2"),
													FString("Input Tag for Passive Ability 2")
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

	// Debuffs
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Burn"),
													FString("Burn Debuff Tag")
													);
	GameplayTags.Debuff_Burn_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
														FName("Debuff.Burn.Chance"),
														FString("Debuff Chance Statistic Tag")
														);
	GameplayTags.Debuff_Burn_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Burn.Frequency"),
													FString("Debuff Frequency Statistic Tag")
													);
	GameplayTags.Debuff_Burn_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Burn.Duration"),
													FString("Debuff Duration Statistic Tag")
													);

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Stun"),
													FString("Stun Debuff Tag")
													);
	GameplayTags.Debuff_Stun_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
														FName("Debuff.Stun.Chance"),
														FString("Debuff Chance Statistic Tag")
														);
	GameplayTags.Debuff_Stun_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Stun.Frequency"),
													FString("Debuff Frequency Statistic Tag")
													);
	GameplayTags.Debuff_Stun_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Stun.Duration"),
													FString("Debuff Duration Statistic Tag")
													);
	
	
	// Debuff Statistics
	GameplayTags.Debuff_Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.Damage.Fire"),
													FString("Debuff Fire Damage Statistic Tag")
													);

	GameplayTags.DebuffDamageTypesToResistances.Add(GameplayTags.Debuff_Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	GameplayTags.DebuffTypes.Add(GameplayTags.Debuff_Burn);
	GameplayTags.DebuffTypes.Add(GameplayTags.Debuff_Stun);
	
	GameplayTags.DebuffsEffectedByTenacity.Add(GameplayTags.Debuff_Stun);
	
	
	// Debuff Damage Multipliers
	GameplayTags.Debuff_DamageMultiplier_Source_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.DamageMultiplier.Source.AttackPower"),
													FString("Debuff Damage Multiplier for Attack Power from Source (Deal a % of Sources Attack Power as Damage)")
													);
	GameplayTags.Debuff_DamageMultiplier_Source_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.DamageMultiplier.Source.MaxHealth"),
													FString("Debuff Damage Multiplier for Max Health from Source (Deal a % of Sources Max Health as Damage)")
													);
	GameplayTags.Debuff_DamageMultiplier_Source_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.DamageMultiplier.Source.Defense"),
													FString("Debuff Damage Multiplier for Defense from Source (Deal a % of Sources Defense as Damage)")
													);

	GameplayTags.Debuff_DamageMultiplier_Target_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Debuff.DamageMultiplier.Target.MaxHealth"),
													FString("Debuff Damage Multiplier for Max Health from Target (Deal a % of Sources Max Health as Damage)")
													);
	
	GameplayTags.DebuffDamageMultipliersToSourceAttributes.Add(GameplayTags.Debuff_DamageMultiplier_Source_AttackPower, GameplayTags.Attributes_Secondary_AttackPower);
	GameplayTags.DebuffDamageMultipliersToSourceAttributes.Add(GameplayTags.Debuff_DamageMultiplier_Source_MaxHealth, GameplayTags.Attributes_Secondary_MaxHealth);
	GameplayTags.DebuffDamageMultipliersToSourceAttributes.Add(GameplayTags.Debuff_DamageMultiplier_Source_Defense, GameplayTags.Attributes_Secondary_Defense);

	GameplayTags.DebuffDamageMultipliersToTargetAttributes.Add(GameplayTags.Debuff_DamageMultiplier_Target_MaxHealth, GameplayTags.Attributes_Secondary_MaxHealth);

	
	
	// Damage Multiplier
	GameplayTags.DamageMultiplier_Source_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("DamageMultiplier.Source.AttackPower"),
													FString("Damage Multiplier for Attack Power from Source (Deal a % of Sources Attack Power as Damage)")
													);
	GameplayTags.DamageMultiplier_Source_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("DamageMultiplier.Source.MaxHealth"),
													FString("Damage Multiplier for Max Health from Source (Deal a % of Sources Max Health as Damage)")
													);
	GameplayTags.DamageMultiplier_Source_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("DamageMultiplier.Source.Defense"),
													FString("Damage Multiplier for Defense from Source (Deal a % of Sources Defense as Damage)")
													);

	GameplayTags.DamageMultiplier_Target_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("DamageMultiplier.Target.MaxHealth"),
													FString("Damage Multiplier for Max Health from Target (Deal a % of Sources Max Health as Damage)")
													);
	
	GameplayTags.DamageMultipliersToSourceAttributes.Add(GameplayTags.DamageMultiplier_Source_AttackPower, GameplayTags.Attributes_Secondary_AttackPower);
	GameplayTags.DamageMultipliersToSourceAttributes.Add(GameplayTags.DamageMultiplier_Source_MaxHealth, GameplayTags.Attributes_Secondary_MaxHealth);
	GameplayTags.DamageMultipliersToSourceAttributes.Add(GameplayTags.DamageMultiplier_Source_Defense, GameplayTags.Attributes_Secondary_Defense);

	GameplayTags.DamageMultipliersToTargetAttributes.Add(GameplayTags.DamageMultiplier_Target_MaxHealth, GameplayTags.Attributes_Secondary_MaxHealth);

	
	// Abilities
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Attack"),
													FString("Attack Ability Tag")
													);
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Summon"),
													FString("Summon Ability Tag")
													);
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.HitReact"),
													FString("HitReact Ability Tag")
													);
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.None"),
													FString("No Ability - null value for Ability Tags")
													);
	
	// Ability Status
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Status.Locked"),
													FString("Ability Status Locked Tag")
													);
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Status.Unlocked"),
													FString("Ability Status Unlocked Tag")
													);
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Status.Eligible"),
													FString("Ability Status Eligible Tag")
													);
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Status.Equipped"),
													FString("Ability Status Equipped Tag")
													);
	
	// Ability Types
	GameplayTags.Abilities_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Type.Active"),
													FString("Active Ability type Tag")
													);
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Type.Passive"),
													FString("Passive Ability type Tag")
													);
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Type.None"),
													FString("None Ability type Tag")
													);
	
	// Gameplay Abilities
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Fire.FireBolt"),
													FString("FireBolt Ability Tag")
													);
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Lightning.Electrocute"),
													FString("Electrocute Ability Tag")
													);

	
	// Cooldowns
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Cooldown.Fire.FireBolt"),
													FString("FireBolt Cooldown Tag")
													);


	// Passive Spell Abilities
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Passive.HaloOfProtection"),
													FString("HaloOfProtection Ability Tag")
													);
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Passive.LifeSiphon"),
													FString("LifeSiphon Ability Tag")
													);
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Abilities.Passive.ManaSiphon"),
													FString("ManaSiphon Ability Tag")
													);
	
	// CombatSockets
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("CombatSocket.Weapon"),
													FString("Weapon based attack")
													);
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("CombatSocket.RightHand"),
													FString("Right Hand based attack")
													);
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("CombatSocket.LeftHand"),
													FString("Left Hand based attack")
													);
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("CombatSocket.Tail"),
													FString("Tail based attack")
													);

	// Montages
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Montage.Attack.1"),
													FString("Attack 1")
													);
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Montage.Attack.2"),
													FString("Attack 2")
													);
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Montage.Attack.3"),
													FString("Attack 3")
													);
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Montage.Attack.4"),
													FString("Attack 4")
													);
	
	// Effects
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Effects.HitReact"),
													FString("Reaction effect for being hit")
													);


	// Player Tags
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.CursorTrace"),
													FString("Block tracing under the cursor")
													);
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.InputHeld"),
													FString("Block Input Held callback for input")
													);
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.InputPressed"),
													FString("Block Input Pressed callback for input")
													);
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.InputReleased"),
													FString("Block Input Released callback for input")
													);
	GameplayTags.Player_Block_Movement = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.Movement"),
													FString("Block Player Movement")
													);
	GameplayTags.Player_Block_Rotation = UGameplayTagsManager::Get().AddNativeGameplayTag(
													FName("Player.Block.Rotation"),
													FString("Block Player Rotation")
													);

	
}
