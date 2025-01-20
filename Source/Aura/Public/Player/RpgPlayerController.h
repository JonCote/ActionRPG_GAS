// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "RpgPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UDamageTextComponent;
class URpgInputConfig;
class UInputMappingContext;
class UInputAction;
class URpgAbilitySystemComponent;

struct FInputActionValue;

enum ETargetingStatus
{
	TargetingEnemy,
	TargetingPlayer,
	TargetingInteractable,
	NotTargeting
	
};


UCLASS()
class AURA_API ARpgPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARpgPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCritHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintCallable)
	void UpdateMagicCircleLocation(FHitResult HitResult);

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	void RotateToMouse();
	UPROPERTY()
	TObjectPtr<AActor> LastActor;
	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	ETargetingStatus TargetingStatus = NotTargeting;
	FHitResult CursorHit;
	FHitResult CursorGroundHit;
	

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<URpgInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<URpgAbilitySystemComponent> RpgAbilitySystemComponent;

	URpgAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();
	
};





