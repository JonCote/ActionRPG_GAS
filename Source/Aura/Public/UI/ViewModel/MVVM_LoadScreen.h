// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(const int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(const int32 Slot, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void LoadSaveButtonPressed(const int32 Slot);

	UFUNCTION(BlueprintCallable)
	void DeleteSaveButtonPressed(const int32 Slot);

	void LoadData();

	void SetGameInstance(const int32 Slot);

	void SetLoadScreenName(const FString& InLoadScreenName);

	FString GetLoadScreenName() const { return LoadScreenName; }
	
private:

	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString LoadScreenName;
	
	
};
