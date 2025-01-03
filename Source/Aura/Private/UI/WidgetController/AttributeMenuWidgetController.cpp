// Copyright Jonathan Cote


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/RpgPlayerState.h"

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	URpgAttributeSet* AS = Cast<URpgAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	ARpgPlayerState* RpgPlayerState = CastChecked<ARpgPlayerState>(PlayerState);
	OnPlayerAttributePointsChangedDelegate.Broadcast(RpgPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	URpgAttributeSet* AS = CastChecked<URpgAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	ARpgPlayerState* RpgPlayerState = CastChecked<ARpgPlayerState>(PlayerState);
	RpgPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerAttributePointsChangedDelegate.Broadcast(NewValue);
		}
	);
	RpgPlayerState->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerSpellPointsChangedDelegate.Broadcast(NewValue);
		}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	URpgAbilitySystemComponent* RpgASC = CastChecked<URpgAbilitySystemComponent>(AbilitySystemComponent);
	RpgASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FRpgAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


