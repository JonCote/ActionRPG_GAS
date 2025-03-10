// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Actor/LootableItem.h"
#include "RpgEquippableItem.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayAttribute, float> AttributeModifiers;
	
	
};

class ARpgCharacter;
/**
 * 
 */
UCLASS()
class AURA_API ARpgEquippableItem : public ALootableItem
{
	GENERATED_BODY()


public:
	ARpgEquippableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentData EquipmentData;
	
	UPROPERTY()
	FActiveGameplayEffectHandle EquipmentModGEHandle;
	
	void CreateAndApplyAttributeModifierEffects();
	void RemoveAttributeModifierEffects();


protected:
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	
	UPROPERTY()
	ARpgCharacter* OwningCharacter;

	UPROPERTY()
	int32 EquipmentGENameOffsetNumber = 0;
	
	
};
