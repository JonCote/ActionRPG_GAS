// Copyright Jonathan Cote


#include "UI/WidgetController/RpgWidgetController.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/RpgPlayerController.h"
#include "Player/RpgPlayerState.h"


void URpgWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void URpgWidgetController::BroadCastInitialValues()
{
}

void URpgWidgetController::BindCallbacksToDependencies()
{
}

ARpgPlayerController* URpgWidgetController::GetRpgPlayerController()
{
	if (RpgPlayerController == nullptr)
	{
		RpgPlayerController = Cast<ARpgPlayerController>(PlayerController);
	}
	return RpgPlayerController;
}

ARpgPlayerState* URpgWidgetController::GetRpgPlayerState()
{
	if (RpgPlayerState == nullptr)
	{
		RpgPlayerState = Cast<ARpgPlayerState>(PlayerState);
	}
	return RpgPlayerState;
}

URpgAbilitySystemComponent* URpgWidgetController::GetRpgAbilitySystemComponent()
{
	if (RpgAbilitySystemComponent == nullptr)
	{
		RpgAbilitySystemComponent = Cast<URpgAbilitySystemComponent>(AbilitySystemComponent);
	}
	return RpgAbilitySystemComponent;
}

URpgAttributeSet* URpgWidgetController::GetRpgAttributeSet()
{
	if (RpgAttributeSet == nullptr)
	{
		RpgAttributeSet = Cast<URpgAttributeSet>(AttributeSet);
	}
	return RpgAttributeSet;
}

void URpgWidgetController::BroadcastAbilityInfo()
{
	if (!GetRpgAbilitySystemComponent()->bStartupAbilitiesGiven) { return; }

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FRpgAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetRpgAbilitySystemComponent()->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag =  GetRpgAbilitySystemComponent()->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = GetRpgAbilitySystemComponent()->GetStatusTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});

	GetRpgAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}
