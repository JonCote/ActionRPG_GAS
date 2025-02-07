// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RpgHUD.generated.h"

struct FGameplayTag;
class ULayoutWidget;
class ULayerWidget;
class UInventoryWidgetController;
class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class URpgUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;


UCLASS()
class AURA_API ARpgHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:

	
	UPROPERTY()
	TObjectPtr<URpgUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URpgUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
	
	
};
