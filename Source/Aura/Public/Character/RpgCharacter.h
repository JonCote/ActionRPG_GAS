// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Character/RpgCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "RpgCharacter.generated.h"


class ARpgPlayerController;
class ARpgPlayerState;

UCLASS()
class AURA_API ARpgCharacter : public ARpgCharacterBase
{
	GENERATED_BODY()

public:
	ARpgCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

protected:

	UPROPERTY()
	TObjectPtr<ARpgPlayerState> RpgPlayerState;

	UPROPERTY()
	TObjectPtr<ARpgPlayerController> RpgPlayerController;

private:
	void InitAbilityActorInfo();
	void InitPlayerHUD();
};
