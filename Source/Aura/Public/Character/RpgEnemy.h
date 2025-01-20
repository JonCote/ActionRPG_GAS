// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Character/RpgCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "RpgEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ARpgAIController;

/**
 * 
 */
UCLASS()
class AURA_API ARpgEnemy : public ARpgCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ARpgEnemy();
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Die_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	//~ End Combat Interface

	//~ Begin Highlight Interface
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	//~ End Highlight Interface

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

	void SetLevel(const int32 InLevel) { Level = InLevel; }
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ARpgAIController> RpgAIController;
	
};
