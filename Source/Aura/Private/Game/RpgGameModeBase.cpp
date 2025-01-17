// Copyright Jonathan Cote


#include "Game/RpgGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Game/RpgGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"


AActor* ARpgGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(GetGameInstance());
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		AActor* SelectedActor = FoundActors[0];
		for (AActor* Actor : FoundActors)
		{
			if (const APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == RpgGameInstance->PlayerStartTag)
				{
					SelectedActor = Actor;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void ARpgGameModeBase::TravelToMap(const UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, GameMaps.FindChecked(Slot->GetMapName()));
}

void ARpgGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const
{
	DeleteSlotData(LoadSlot, SlotIndex);
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->SaveSlotStatus = LoadSlot->SlotStatus;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

void ARpgGameModeBase::DeleteSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
}

ULoadScreenSaveGame* ARpgGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
 
	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

ULoadScreenSaveGame* ARpgGameModeBase::RetrieveInGameSaveData() const
{
	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = RpgGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = RpgGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ARpgGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const
{
	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = RpgGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = RpgGameInstance->LoadSlotIndex;

	RpgGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ARpgGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameMaps.Add(DefaultMapName, DefaultMap);
}
