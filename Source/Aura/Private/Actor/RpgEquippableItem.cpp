// Copyright Jonathan Cote


#include "Actor/RpgEquippableItem.h"

#include "AbilitySystemComponent.h"
#include "Aura/RpgLogChannels.h"
#include "Character/RpgCharacter.h"

ARpgEquippableItem::ARpgEquippableItem()
{
}

void ARpgEquippableItem::CreateAndApplyAttributeModifierEffects()
{
	if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
	{
		// Create the GE at runtime
		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName("RuntimeGE_" + GetNameSafe(this) + FString::FromInt(EquipmentGENameOffsetNumber)));

		GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		EquipmentGENameOffsetNumber++;

		int32 Idx = GameplayEffect->Modifiers.Num();
		GameplayEffect->Modifiers.SetNum(Idx + EquipmentData.AttributeModifiers.Num());

		int tempIdxDisplacement = 0;
		for (const TPair<FGameplayAttribute, float>& pair : EquipmentData.AttributeModifiers)
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
}

void ARpgEquippableItem::RemoveAttributeModifierEffects()
{
	if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
	{
		ASC->RemoveActiveGameplayEffect(EquipmentModGEHandle);
		EquipmentModGEHandle.Invalidate();
	}
}

void ARpgEquippableItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
			OwningCharacter = Cast<ARpgCharacter>(OtherActor);
		//	CreateAndApplyAttributeModifierEffects();
			Destroy();
	}
}
