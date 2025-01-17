// Copyright Jonathan Cote


#include "UI/HUD/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "UI/Widget/MainMenuWidget.h"

void AMainMenuHUD::OpenLoadScreen()
{
	CloseMainMenu();
	
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->SetLoadScreenName(FString("Load Screen"));
	LoadScreenViewModel->InitializeLoadSlots();
	
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();

	LoadScreenViewModel->LoadData();
	
}

void AMainMenuHUD::CloseLoadScreen()
{
	LoadScreenWidget->RemoveFromParent();

	OpenMainMenu();
}

void AMainMenuHUD::OpenMainMenu()
{
	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void AMainMenuHUD::CloseMainMenu()
{
	MainMenuWidget->RemoveFromParent();
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	OpenMainMenu();
	
}
