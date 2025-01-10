// Copyright Jonathan Cote


#include "AbilitySystem/Abilities/ProjectileAbility/RpgFireBolt.h"

#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "Actor/RpgProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


FString URpgFireBolt::GetDescription(const FString& AbilityName, const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float BaseDamage = GetBaseDamage(Level);
	
	TMap<FString, float> Multipliers;
	const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageInfo.DamageMultipliers)
	{
		if (Pair.Key.MatchesTagExact(GameplayTags.DamageMultiplier_Source_AttackPower))
		{
			Multipliers.Add(FString("AttackPower"), GetDamageMultiplierByTag(Pair.Key, Level) * 100.f);
		}
	}
	
	FString Description = FString();
	Description += FString::Printf(TEXT("<Title>")) + AbilityName + FString::Printf(TEXT("</>\n"));
	Description += FString::Printf(TEXT("<Small>Level: </><Level>%d</>\n<Small>Mana Cost: </><Mana>%d</>\n<Small>Cooldown: </><Cooldown>%.2f</><Small>sec</>\n\n"), Level, static_cast<int>(ManaCost), Cooldown);
	Description += FString::Printf(TEXT("<Default>Base Damage: </><BaseDamage>%d</>\n"), static_cast<int>(BaseDamage));
	for (auto& Pair : Multipliers)
	{
		Description += FString::Printf(TEXT("<Default>")) + Pair.Key + FString::Printf(TEXT(" Multiplier </><")) + Pair.Key + FString::Printf(TEXT(">%d%%</>\n"), static_cast<int>(Pair.Value));
	}

	
	return Description;
}

void URpgFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FVector& SpawnLocation, AActor* HomingTarget, const float PitchOverride)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) { return; }
	
	FRotator Rotation = (ProjectileTargetLocation - SpawnLocation).Rotation();
	Rotation.Pitch = PitchOverride;
	
	
	const int32 NumProjectiles = FMath::Min(MaxNumberOfProjectiles, GetAbilityLevel());
	const float HitRadius = TargetLocationHitRadius.GetValueAtLevel(GetAbilityLevel());
	//UKismetSystemLibrary::DrawDebugCircle(GetAvatarActorFromActorInfo(), ProjectileTargetLocation+FVector(0,0,1), HitRadius, 12, FLinearColor::White, 5, 1, FVector(1,0,0), FVector(0,1,0));
	
	const FVector Forward = Rotation.Vector();
	const TArray<FRotator> Rotations = URpgAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	
	
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		ARpgProjectile* Projectile = GetWorld()->SpawnActorDeferred<ARpgProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// Target Enemy Actor
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		}
		// Target Ground Location
		/*else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation + FVector(0, 0, 20));
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;*/
		
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
