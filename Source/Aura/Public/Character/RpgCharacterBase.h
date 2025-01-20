// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/HighlightInterface.h"
#include "RpgCharacterBase.generated.h"


class UPassiveSpellNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;


UCLASS(Abstract)
class AURA_API ARpgCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ARpgCharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//~ Begin Combat Interface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die_Implementation() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FOnDamagedSignature& GetOnDamagedDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void SetMinionCount_Implementation(const int32 Value) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() override;
	//~ End Combat Interface

	FOnASCRegistered OnASCRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamagedSignature OnDamagedDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	void SetCharacterClass(const ECharacterClass InCharacterClass) { CharacterClass = InCharacterClass; }
	
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// TODO: Replace with a TMap for GameplayTag to Socket FName relation
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 600.f;
	
	bool bDead = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalityAttributes;
	
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	virtual void InitDefaultAttributes() const;

	void AddCharacterAbilities();

	//~ Start Dissolve Effects
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	//~ End Dissolve Effects

	//~ Start Minions
	int32 MinionCount = 0;

	//~ End Minions
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, Category = "Combat|DebuffEffects")
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat|DebuffEffects")
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
	
private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere, Category = "Combat|PassiveEffects")
	TObjectPtr<UPassiveSpellNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat|PassiveEffects")
	TObjectPtr<UPassiveSpellNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat|PassiveEffects")
	TObjectPtr<UPassiveSpellNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
