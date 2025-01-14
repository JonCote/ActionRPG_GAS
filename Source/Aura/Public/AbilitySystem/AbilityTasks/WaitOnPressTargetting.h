// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitOnPressTargetting.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPressedMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 *  TODO: NOT WORKING DELETE OR FIX
 */
UCLASS()
class AURA_API UWaitOnPressTargetting : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName="WaitOnPressTargetting", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly = "true"))
	static UWaitOnPressTargetting* CreateWaitOnPressTargetting(UGameplayAbility* OwningAbility, const ECollisionChannel CollisionChannel);

	
	UPROPERTY(BlueprintAssignable)
	FOnPressedMouseTargetDataSignature TargetData;

private:

	virtual void Activate() override;
	void SendMouseCursorData() const;
	void OnPressedCallback() const;
	
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const;

	ECollisionChannel TraceCollisionChannel;
	
};
