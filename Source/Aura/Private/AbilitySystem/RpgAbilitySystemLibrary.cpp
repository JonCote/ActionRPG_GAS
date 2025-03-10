// Copyright Jonathan Cote


#include "AbilitySystem/RpgAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RpgAbilityTypes.h"
#include "RpgGameplayTags.h"
#include "Aura/RpgLogChannels.h"
#include "Character/RpgCharacter.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/RpgGameModeBase.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RpgPlayerState.h"
#include "UI/WidgetController/RpgWidgetController.h"
#include "UI/HUD/RpgHUD.h"



/* ====================================================================================================================
 *	Widget Controller
 */

bool URpgAbilitySystemLibrary::ConstructWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ARpgHUD*& OutRpgHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutRpgHUD = Cast<ARpgHUD>(PC->GetHUD());
		if (OutRpgHUD)
		{
			ARpgPlayerState* PS = PC->GetPlayerState<ARpgPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* URpgAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	ARpgHUD* RpgHUD = nullptr;
	if (ConstructWidgetControllerParams(WorldContextObject, WidgetControllerParams, RpgHUD))
	{
		return RpgHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* URpgAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	ARpgHUD* RpgHUD = nullptr;
	if (ConstructWidgetControllerParams(WorldContextObject, WidgetControllerParams, RpgHUD))
	{
		return RpgHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

USpellMenuWidgetController* URpgAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	ARpgHUD* RpgHUD = nullptr;
	if (ConstructWidgetControllerParams(WorldContextObject, WidgetControllerParams, RpgHUD))
	{
		return RpgHUD->GetSpellMenuWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UInventoryWidgetController* URpgAbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	ARpgHUD* RpgHUD = nullptr;
	if (ConstructWidgetControllerParams(WorldContextObject, WidgetControllerParams, RpgHUD))
	{
		return RpgHUD->GetInventoryWidgetController(WidgetControllerParams);
	}
	return nullptr;
}


/* ====================================================================================================================
 *	Ability System
 */

void URpgAbilitySystemLibrary::InitializeEnemyDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void URpgAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.0, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Dexterity, SaveGame->Dexterity);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void URpgAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UAbilityInfo* URpgAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ARpgGameModeBase* RpgGM = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (RpgGM == nullptr) return nullptr;
	return RpgGM->AbilityInfo;
}

UCharacterClassInfo* URpgAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ARpgGameModeBase* RpgGM = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (RpgGM == nullptr) return nullptr;
	return RpgGM->CharacterClassInfo;
}



/* ====================================================================================================================
 *	Effect Context Getter
 */

bool URpgAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->IsBlockedHit();
	}
	return false;
}

bool URpgAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->IsCriticalHit();
	}
	return false;
}

bool URpgAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float URpgAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float URpgAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float URpgAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag URpgAbilitySystemLibrary::GetDebuffTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (RpgEffectContext->GetDebuffTag().IsValid())
		{
			return *RpgEffectContext->GetDebuffTag();
		}
	}
	return FGameplayTag();
}

bool URpgAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RpgEffectContext->IsRadialDamage();
	}
	return false;
}

float URpgAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (RpgEffectContext->IsRadialDamage())
		{
			return RpgEffectContext->GetRadialDamageInnerRadius();
		}
	}
	return 0.f;
}

float URpgAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (RpgEffectContext->IsRadialDamage())
		{
			return RpgEffectContext->GetRadialDamageOuterRadius();
		}
	}
	return 0.f;
}

FVector URpgAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRpgGameplayEffectContext* RpgEffectContext = static_cast<const FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (RpgEffectContext->IsRadialDamage())
		{
			return RpgEffectContext->GetRadialDamageOrigin();
		}
	}
	return FVector::ZeroVector;
}



/* ====================================================================================================================
 *	Effect Context Setter
 */

void URpgAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsBlockedHit)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetBlockedHit(bIsBlockedHit);
	}
}

void URpgAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsCriticalHit)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetCriticalHit(bIsCriticalHit);
	}
}

void URpgAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bIsSuccessfulDebuff)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetIsSuccessfulDebuff(bIsSuccessfulDebuff);
	}
}

void URpgAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,
	const float DebuffDamage)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetDebuffDamage(DebuffDamage);
	}
}

void URpgAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	const float DebuffDuration)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetDebuffDuration(DebuffDuration);
	}
}

void URpgAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	const float DebuffFrequency)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void URpgAbilitySystemLibrary::SetDebuffTag(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& DebuffTag)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetDebuffTag(MakeShared<FGameplayTag>(DebuffTag));
	}
}

void URpgAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bIsRadialDamage)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetIsRadialDamage(bIsRadialDamage);
	}
}

void URpgAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	const float InnerRadius)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetRadialDamageInnerRadius(InnerRadius);
	}
}

void URpgAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	const float OuterRadius)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetRadialDamageOuterRadius(OuterRadius);
	}
}

void URpgAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector Origin)
{
	if (FRpgGameplayEffectContext* RpgEffectContext = static_cast<FRpgGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RpgEffectContext->SetRadialDamageOrigin(Origin);
	}
}



/* ====================================================================================================================
 *	Gameplay Mechanics
 */

void URpgAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// query scene to see what we hit
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void URpgAbilitySystemLibrary::GetClosestTargets(const int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;
	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTargetActor : ActorsToCheck)
		{
			const double Distance = (PotentialTargetActor->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTargetActor;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool URpgAbilitySystemLibrary::IsNotFriendly(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool bFirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool bSecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));

	return !((bFirstIsPlayer && bSecondIsPlayer) || (bFirstIsEnemy && bSecondIsEnemy));
}

TArray<FRotator> URpgAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	if (NumRotators < 1) { return Rotators; }
	if (NumRotators == 1)
	{
		Rotators.Add(Forward.Rotation());
		return Rotators;
	}
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / (NumRotators - 1);

	for (int32 i = 0; i < NumRotators; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		Rotators.Add(Direction.Rotation());
	}
	
	return Rotators;
}

TArray<FVector> URpgAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	if (NumVectors < 1) { return Vectors; }
	if (NumVectors == 1)
	{
		Vectors.Add(Forward);
		return Vectors;
	}
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / (NumVectors - 1);

	for (int32 i = 0; i < NumVectors; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		Vectors.Add(Direction);
	}
	
	return Vectors;
}

int32 URpgAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
															const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	return static_cast<int32>(Info.XPReward.GetValueAtLevel(CharacterLevel));
}



/* ====================================================================================================================
 *	Apply Damage Effect
 */

FGameplayEffectContextHandle URpgAbilitySystemLibrary::ApplyDebuffEffects(const FDamageEffectParams& DamageEffectParams,
	FGameplayEffectContextHandle& EffectContextHandle)
{
	/* Assign Caller Magnitudes for DebuffInfo */
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
	for (const FDebuffInfo Info : DamageEffectParams.DebuffInfo)
	{
		if (Info.DebuffType == EDebuffType::Burn)
		{
			FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
			
			// Debuff Tag (pass value 1.f for a DebuffTag if assigned for ability)
			const float DebuffValid = 1.f;
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Info.BurnInfo.DebuffTag, DebuffValid);

			// Debuff Statistics
			const float ScaledDebuffDamage = Info.BurnInfo.DebuffDamageInfo.BaseDamage.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Info.BurnInfo.DebuffDamageInfo.DamageTypeTag, ScaledDebuffDamage);
		
			for (const TTuple<FGameplayTag, FScalableFloat> Pair : Info.BurnInfo.DebuffDamageInfo.DamageMultipliers)
			{
				const float ScaledMultiplier = Pair.Value.GetValueAtLevel(DamageEffectParams.AbilityLevel);
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledMultiplier);
			}
		
			const float ScaledDebuffChance = Info.BurnInfo.DebuffChance.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Burn_Chance, ScaledDebuffChance);

			const float ScaledDebuffFrequency = Info.BurnInfo.DebuffFrequency.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Burn_Frequency, ScaledDebuffFrequency);

			const float ScaledDebuffDuration = Info.BurnInfo.DebuffDuration.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Burn_Duration, ScaledDebuffDuration);

			DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
		else if (Info.DebuffType == EDebuffType::Stun)
		{
			FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

			// Debuff Tag (pass value 1.f for a DebuffTag if assigned for ability)
			const float DebuffValid = 1.f;
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Info.StunInfo.DebuffTag, DebuffValid);

			// Debuff Statistics
			const float ScaledDebuffChance = Info.StunInfo.DebuffChance.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Stun_Chance, ScaledDebuffChance);

			const float ScaledDebuffDuration = Info.StunInfo.DebuffDuration.GetValueAtLevel(DamageEffectParams.AbilityLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Stun_Duration, ScaledDebuffDuration);

			DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
	
	return EffectContextHandle;
}

FGameplayEffectContextHandle URpgAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	checkf(DamageEffectParams.TargetAbilitySystemComponent, TEXT("Target Ability System Component not set when URpgAbilitySystemLibrary::ApplyDamageEffect is Called!"));
	checkf(DamageEffectParams.SourceAbilitySystemComponent, TEXT("Source Ability System Component not set when URpgAbilitySystemLibrary::ApplyDamageEffect is Called!"));

	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	const FRpgGameplayTags& GameplayTags = FRpgGameplayTags::Get();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	
	FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	/* Assign Caller Magnitudes for DamageInfo */
	// Base Damage
	const float ScaledBaseDamage = DamageEffectParams.DamageInfo.BaseDamage.GetValueAtLevel(DamageEffectParams.AbilityLevel);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageInfo.DamageTypeTag, ScaledBaseDamage);

	// Multipliers
	for (const TTuple<FGameplayTag, FScalableFloat> Pair : DamageEffectParams.DamageInfo.DamageMultipliers)
	{
		const float ScaledMultiplier = Pair.Value.GetValueAtLevel(DamageEffectParams.AbilityLevel);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledMultiplier);
	}
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	
	/* Assign Caller Magnitudes for DebuffInfo */
	EffectContextHandle = ApplyDebuffEffects(DamageEffectParams, EffectContextHandle);


	/* Set EffectContext RadialDamage Info */
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.DamageInfo.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.DamageInfo.RadialDamageInfo.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.DamageInfo.RadialDamageInfo.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.DamageInfo.RadialDamageInfo.RadialDamageOrigin);
	
	
	return EffectContextHandle;
}

// TODO: Make this work on Client side so Multiplayer is possible (Replication is not happening from server to client I believe)
FActiveGameplayEffectHandle URpgAbilitySystemLibrary::CreateAndApplyAttributeModifierEffects(
	const AActor* TargetCharacter, const TMap<FGameplayAttribute, float> AttributeModifiers)
{
	FActiveGameplayEffectHandle EquipmentModGEHandle;
	if (UAbilitySystemComponent* ASC = Cast<ARpgCharacter>(TargetCharacter)->GetAbilitySystemComponent())
	{
		// Create the GE at runtime
		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage());

		GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		int32 Idx = GameplayEffect->Modifiers.Num();
		GameplayEffect->Modifiers.SetNum(Idx + AttributeModifiers.Num());

		int tempIdxDisplacement = 0;
		for (const TPair<FGameplayAttribute, float>& pair : AttributeModifiers)
		{
			// Add check to validate pair.key is a valid Attribute

			FGameplayTagRequirements reqs;
			reqs.IgnoreTags = FGameplayTagContainer();
			reqs. RequireTags = FGameplayTagContainer();

			FGameplayModifierInfo& Info = GameplayEffect->Modifiers[Idx + tempIdxDisplacement];
			Info.ModifierMagnitude = FScalableFloat(pair.Value);
			Info.ModifierOp = EGameplayModOp::Additive;
			Info.Attribute = pair.Key;
			Info.SourceTags = reqs;
			Info.TargetTags = reqs;

			tempIdxDisplacement += 1;
		}
		

		EquipmentModGEHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, ASC->MakeEffectContext());
		UE_LOG(LogRpg, Log, TEXT("New GE Handle: %s"), *EquipmentModGEHandle.ToString());
		
	}
	
	return EquipmentModGEHandle;
}

FActiveGameplayEffectHandle URpgAbilitySystemLibrary::RemoveAttributeModifierEffects(const AActor* TargetCharacter, FActiveGameplayEffectHandle ActiveEffectHandleToRemove)
{
	if (UAbilitySystemComponent* ASC = Cast<ARpgCharacter>(TargetCharacter)->GetAbilitySystemComponent())
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffectHandleToRemove);
		ActiveEffectHandleToRemove.Invalidate();
	}
	return ActiveEffectHandleToRemove;
}
