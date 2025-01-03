// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RpgAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};


template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


UCLASS()
class AURA_API URpgAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URpgAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	
	//~ Begin Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Attribute|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Attribute|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resilience, Category="Attribute|Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Attribute|Primary")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Dexterity);

	//~ End Primary Attribute

	//~ Begin Secondary Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackPower, Category="Attribute|Secondary")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, Category="Attribute|Secondary")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitRate, Category="Attribute|Secondary")
	FGameplayAttributeData CriticalHitRate;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, CriticalHitRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Attribute|Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Attribute|Secondary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Attribute|Secondary")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Attribute|Secondary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, MaxMana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Attribute|Secondary")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Attribute|Secondary")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaRegeneration, Category="Attribute|Secondary")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, StaminaRegeneration);
	
	//~ End Secondary Attribute

	//~ Begin Damage Type Resistance Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Attribute|Resistances")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IceResistance, Category="Attribute|Resistances")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, IceResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightningResistance, Category="Attribute|Resistances")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurseResistance, Category="Attribute|Resistances")
	FGameplayAttributeData CurseResistance;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, CurseResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Attribute|Resistances")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, PhysicalResistance);
	
	//~ End Damage Type Resistance Attributes
	
	//~ Begin Vitality Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Attribute|Vitality")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Attribute|Vitality")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Attribute|Vitality")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, Stamina);

	//~ End Vitality Attribute

	//~ Begin Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(URpgAttributeSet, IncomingXP);

	//~ End Meta Attributes
	
	
	//~ Begin Primary Attributes OnRep
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	//~ End Primary Attribute OnRep

	//~ Begin Secondary Attribute OnRep
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;

	UFUNCTION()
	void OnRep_CriticalHitRate(const FGameplayAttributeData& OldCriticalHitRate) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const;
	
	//~ End Secondary Attribute OnRep

	//~ Begin Damage Type Resistance OnRep
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_CurseResistance(const FGameplayAttributeData& OldCurseResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	//~ End Damage Type Resistance OnRep
	
	//~ Begin Vitality Attributes OnRep
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	//~ End Vitality Attribute OnRep

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	static void ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit);
	static void SendXPEvent(const FEffectProperties& Props);

	bool bTopOffHealth = false;
	bool bTopOffMana = false;
	
};

