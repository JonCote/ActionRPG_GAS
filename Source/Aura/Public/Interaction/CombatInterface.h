// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UNiagaraSystem;
class UAnimMontage;


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
	virtual int32 GetCharacterLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

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
};

