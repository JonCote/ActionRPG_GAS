// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RpgDamageGameplayAbility.h"
#include "RpgProjectileSpell.generated.h"


class ARpgProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class AURA_API URpgProjectileSpell : public URpgDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectileAtLocation(const FVector& ProjectileTargetLocation, const FVector SocketLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Info")
	TSubclassOf<ARpgProjectile> ProjectileClass;


};
