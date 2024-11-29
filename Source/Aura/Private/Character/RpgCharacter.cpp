// Copyright Jonathan Cote


#include "Character/RpgCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

ARpgCharacter::ARpgCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
