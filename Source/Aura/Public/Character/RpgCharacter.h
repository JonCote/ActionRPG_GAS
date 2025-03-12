// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/RpgCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PlayerInterface.h"
#include "RpgCharacter.generated.h"


class UItemInfo;
class UInventory;
class ARpgPlayerController;
class ARpgPlayerState;

UCLASS()
class AURA_API ARpgCharacter : public ARpgCharacterBase, public IPlayerInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ARpgCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventory> Inventory;
	
	
	//~ Begin Player Interface
	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(const int32 InLevels) override;
	virtual void AddToAttributePoints_Implementation(const int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(const int32 InSpellPoints) override;
	
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(const int32 InXP) const override;

	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void UpdateMagicCircleLocation_Implementation() override;

	virtual void SaveProgress_Implementation(const FName& CheckpointTag, UWorld* World) override;

	virtual bool LootItem_Implementation(UItemInfo* NewItem) override;
	//~ End Player Interface
	
	//~ Begin Highlight Interface
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	//~ End Highlight Interface
	
	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	//~ End Combat Interface

protected:

	UPROPERTY()
	TObjectPtr<ARpgPlayerState> RpgPlayerState;

	UPROPERTY()
	TObjectPtr<ARpgPlayerController> RpgPlayerController;

	void LoadProgress();

private:
	virtual void InitAbilityActorInfo() override;
	void InitPlayerHUD();

	UFUNCTION(NetMulticast, reliable)
	void MulticastLevelUpParticles() const;
};
