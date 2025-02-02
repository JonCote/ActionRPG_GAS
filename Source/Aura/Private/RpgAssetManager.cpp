// Copyright Jonathan Cote


#include "RpgAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "RpgGameplayTags.h"

URpgAssetManager& URpgAssetManager::Get()
{
	check(GEngine);
	return *Cast<URpgAssetManager>(GEngine->AssetManager);
}

void URpgAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FRpgGameplayTags::InitNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
