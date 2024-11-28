// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RpgCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API ARpgCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ARpgCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
