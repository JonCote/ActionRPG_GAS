// Copyright Jonathan Cote


#include "RpgGameplayTags.h"
#include "GameplayTagsManager.h"

FRpgGameplayTags FRpgGameplayTags::GameplayTags;

void FRpgGameplayTags::InitNativeGameplayTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Defense"), FString("Reduces damage taken"));
	
}
