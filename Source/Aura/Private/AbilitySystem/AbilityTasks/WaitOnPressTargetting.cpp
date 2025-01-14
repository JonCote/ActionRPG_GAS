// Copyright Jonathan Cote


#include "AbilitySystem/AbilityTasks/WaitOnPressTargetting.h"

#include "AbilitySystemComponent.h"
#include "Character/RpgCharacter.h"

UWaitOnPressTargetting* UWaitOnPressTargetting::CreateWaitOnPressTargetting(UGameplayAbility* OwningAbility, const ECollisionChannel CollisionChannel)
{
	UWaitOnPressTargetting* MyObj = NewAbilityTask<UWaitOnPressTargetting>(OwningAbility);
	MyObj->TraceCollisionChannel = CollisionChannel;
	return MyObj;
}

void UWaitOnPressTargetting::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (IsLocallyControlled())
		{
			FGameplayAbilitySpec *Spec = Ability->GetCurrentAbilitySpec();
			if (Spec && Spec->InputPressed)
			{
				OnPressedCallback();
				return;
			}
			else
			{
				SendMouseCursorData();
			}
		}
		else
		{
			const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
			const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
			AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UWaitOnPressTargetting::OnTargetDataReplicatedCallback);
			const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
			if (!bCalledDelegate)
			{
				SetWaitingOnRemotePlayerData();
			
			}
		}
	}
}

void UWaitOnPressTargetting::SendMouseCursorData() const
{
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	FHitResult CursorVisibilityHit;
	bool bValidHitResult = PC->GetHitResultUnderCursor(TraceCollisionChannel, false, CursorHit);
	//PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorVisibilityHit);
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	if (ARpgCharacter* RpgCharacter = Cast<ARpgCharacter>(Ability->GetAvatarActorFromActorInfo()))
	{
		RpgCharacter->UpdateMagicCircleLocation_Implementation(CursorHit);
	}
	
}

void UWaitOnPressTargetting::OnPressedCallback() const
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	FHitResult CursorVisibilityHit;
	bool bValidHitResult = PC->GetHitResultUnderCursor(TraceCollisionChannel, false, CursorHit);
	//PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorVisibilityHit);
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TargetData.Broadcast(DataHandle);
	}

}

void UWaitOnPressTargetting::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                            FGameplayTag ActivationTag) const
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TargetData.Broadcast(DataHandle);
	}
}


