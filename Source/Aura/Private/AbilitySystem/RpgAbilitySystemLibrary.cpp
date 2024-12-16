// Copyright Jonathan Cote


#include "AbilitySystem/RpgAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "RpgAbilityTypes.h"
#include "Game/RpgGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RpgPlayerState.h"
#include "UI/HUD/RpgHUD.h"
#include "UI/WidgetController/RpgWidgetController.h"

UOverlayWidgetController* URpgAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARpgHUD* RpgHUD = Cast<ARpgHUD>(PC->GetHUD()))
		{
			ARpgPlayerState* PS = PC->GetPlayerState<ARpgPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return RpgHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* URpgAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARpgHUD* RpgHUD = Cast<ARpgHUD>(PC->GetHUD()))
		{
			ARpgPlayerState* PS = PC->GetPlayerState<ARpgPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return RpgHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void URpgAbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC)
{
	if (const ARpgGameModeBase* RpgGM = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();
		
		const UCharacterClassInfo* CharacterClassInfo = RpgGM->CharacterClassInfo;
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
}

void URpgAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	if (const ARpgGameModeBase* RpgGM = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		const UCharacterClassInfo* CharacterClassInfo = RpgGM->CharacterClassInfo;
		if (CharacterClassInfo == nullptr) return;
		
		for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
		
		const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetCharacterLevel());
				ASC->GiveAbility(AbilitySpec);
			}
		}
		
	}
}

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
