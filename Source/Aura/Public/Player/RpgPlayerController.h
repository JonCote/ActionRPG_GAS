// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "RpgPlayerController.generated.h"

class UDamageTextComponent;
class URpgInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class URpgAbilitySystemComponent;
class USplineComponent;

struct FInputActionValue;


UCLASS()
class AURA_API ARpgPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARpgPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCritHit);
	
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
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
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

	bool bTargeting = false;
	
	//~ Click To Move Stuff
	UPROPERTY(EditAnywhere, Category = "Input|ClickToMove")
	bool bClickToMove = false;
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input|ClickToMove")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "Input|ClickToMove")
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
	
	//~ end of click to move stuff

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};





