// Copyright Jonathan Cote


#include "Game/RpgGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/RpgLogChannels.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/RpgGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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

void ARpgGameModeBase::SaveWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(GetGameInstance());
	check(RpgGameInstance);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(RpgGameInstance->LoadSlotName, RpgGameInstance->LoadSlotIndex))
	{
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSaveMap;
			NewSaveMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSaveMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		// TODO: this can be optimized by marking actors for save when changed instead of iterating over all actors in the world
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) { continue; }

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, RpgGameInstance->LoadSlotName, RpgGameInstance->LoadSlotIndex);
	}
}

void ARpgGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(GetGameInstance());
	check(RpgGameInstance);

	if (!UGameplayStatics::DoesSaveGameExist(RpgGameInstance->LoadSlotName, RpgGameInstance->LoadSlotIndex)) { return; }
	ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(RpgGameInstance->LoadSlotName, RpgGameInstance->LoadSlotIndex));
	if (SaveGame == nullptr)
	{
		UE_LOG(LogRpg, Error, TEXT("Failed to load slot!"));
		return;
	}
	
	// TODO: this can be optimized by marking actors for save when changed instead of iterating over all actors in the world
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USaveInterface>()) { continue; }

		for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
		{
			if (SavedActor.ActorName == Actor->GetFName())
			{
				if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
				{
					Actor->SetActorTransform(SavedActor.Transform);
				}

				FMemoryReader MemoryReader(SavedActor.Bytes);
				FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
				Archive.ArIsSaveGame = true;
				Actor->Serialize(Archive);	// convert binary bytes back to into variables

				ISaveInterface::Execute_LoadActor(Actor);
			}
		}
	}
}

void ARpgGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameMaps.Add(DefaultMapName, DefaultMap);
}
