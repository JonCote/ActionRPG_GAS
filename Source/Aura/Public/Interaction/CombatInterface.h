// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UNiagaraSystem;
class UAnimMontage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamagedSignature, float /*DamageAmount*/);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CombatSocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};



/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	virtual FOnDamagedSignature& GetOnDamagedDelegate() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface|Minions")
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface|Minions")
	void SetMinionCount(const int32 Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(const bool bInLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh();
	
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

	
	
};

