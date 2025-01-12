

#pragma once

#include "GameplayEffectTypes.h"
#include "RpgGameplayTags.h"
#include "ScalableFloat.h"
#include "RpgAbilityTypes.generated.h"

struct FRpgGameplayTags;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FScalableFloat> DamageMultipliers = TMap<FGameplayTag, FScalableFloat>();
	
};


USTRUCT(BlueprintType)
struct FBurnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "false"))
	FGameplayTag DebuffTag = FRpgGameplayTags::Get().Debuff_Burn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DebuffChance = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DebuffFrequency = FScalableFloat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DebuffDuration = FScalableFloat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDamageInfo DebuffDamageInfo = FDamageInfo();
	
};
USTRUCT(BlueprintType)
struct FStunInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "false"))
	FGameplayTag DebuffTag = FRpgGameplayTags::Get().Debuff_Stun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DebuffChance = FScalableFloat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DebuffDuration = FScalableFloat();
	
};

UENUM(BlueprintType)
enum class EDebuffType
{
	None UMETA(DisplayName = "None"),
	Burn UMETA(DisplayName = "Burn"),
	Stun UMETA(DisplayName = "Stun"),
};


USTRUCT(BlueprintType)
struct FDebuffInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EDebuffType DebuffType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "DebuffType == EDebuffType::Burn", EditConditionHides))
	FBurnInfo BurnInfo = FBurnInfo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "DebuffType == EDebuffType::Stun", EditConditionHides))
	FStunInfo StunInfo = FStunInfo();
	
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FDamageInfo DamageInfo = FDamageInfo();

	UPROPERTY()
	TArray<FDebuffInfo> DebuffInfo = TArray<FDebuffInfo>();
};

USTRUCT(BlueprintType)
struct FDebuffEffect
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.0f;

	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffFrequency = 0.0f;
	
};


USTRUCT(BlueprintType)
struct FRpgGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDebuffTag() const { return DebuffTag; }

	void SetIsSuccessfulDebuff(const bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(const float InDebuffDamage) {	DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(const float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(const float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDebuffTag(const TSharedPtr<FGameplayTag>& InDebuffTag) { DebuffTag = InDebuffTag; }
	//TODO: Add Debuff Count
	
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FRpgGameplayEffectContext* Duplicate() const override
	{
		FRpgGameplayEffectContext* NewContext = new FRpgGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.0f;

	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffFrequency = 0.0f;

	UPROPERTY()
	int32 DebuffCount = 0.0f;
	
	
	TSharedPtr<FGameplayTag> DebuffTag;
	
	
};


template<>
struct TStructOpsTypeTraits<FRpgGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRpgGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};