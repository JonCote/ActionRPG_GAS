// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveSpellNiagaraComponent.generated.h"


class URpgAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API UPassiveSpellNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveSpellNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;

	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	
	void ActivateIfEquipped(URpgAbilitySystemComponent* RpgASC);
	
};
