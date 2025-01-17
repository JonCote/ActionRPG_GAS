// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class ULoadScreenWidget;
class UMVVM_LoadScreen;
class UMainMenuWidgetController;
class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class AURA_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void OpenLoadScreen();

	UFUNCTION(BlueprintCallable)
	void CloseLoadScreen();

	UFUNCTION(BlueprintCallable)
	void OpenMainMenu();
	
	UFUNCTION(BlueprintCallable)
	void CloseMainMenu();
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
	
protected:
	virtual void BeginPlay() override;
	
};
