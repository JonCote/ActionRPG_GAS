// Copyright Jonathan Cote


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/RpgGameInstance.h"
#include "Game/RpgGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
	
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(const int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(const int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitchIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(const int32 Slot, const FString& EnteredName)
{
	const ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetMapName(RpgGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->PlayerStartTag = RpgGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->SlotStatus = Taken;

	RpgGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	SetGameInstance(Slot);
}

void UMVVM_LoadScreen::LoadSaveButtonPressed(const int32 Slot)
{
	ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this));
	SetGameInstance(Slot);
	RpgGameMode->TravelToMap(LoadSlots[Slot]);
}

void UMVVM_LoadScreen::DeleteSaveButtonPressed(const int32 Slot)
{
	ARpgGameModeBase::DeleteSlotData(LoadSlots[Slot], Slot);

	LoadSlots[Slot]->SlotStatus = Vacant;
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::LoadData()
{
	const ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	for (const TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		const ULoadScreenSaveGame* SaveObject = RpgGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(),LoadSlot.Key);

		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		LoadSlot.Value->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SetGameInstance(const int32 Slot)
{
	ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this));
	URpgGameInstance* RpgGameInstance = Cast<URpgGameInstance>(RpgGameMode->GetGameInstance());
	RpgGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	RpgGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	RpgGameInstance->PlayerStartTag = LoadSlots[Slot]->PlayerStartTag;
}

void UMVVM_LoadScreen::SetLoadScreenName(const FString& InLoadScreenName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadScreenName, InLoadScreenName);
}
