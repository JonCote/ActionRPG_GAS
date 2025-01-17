// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RpgGameModeBase.generated.h"


class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API ARpgGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> GameMaps;

	
	void TravelToMap(const UMVVM_LoadSlot* Slot);
	
	/*
	 *	Manage Save Data
	 */
	void SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const;
	static void DeleteSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex);

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	ULoadScreenSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const;
	void SaveWorldState(UWorld* World) const;
	void LoadWorldState(UWorld* World) const;
	
	
	
protected:
	virtual void BeginPlay() override;
	
};
