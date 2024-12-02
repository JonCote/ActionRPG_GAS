// Copyright Jonathan Cote


#include "Character/RpgCharacter.h"

#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RpgPlayerState.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "Player/RpgPlayerController.h"
#include "UI/HUD/RpgHUD.h"


ARpgCharacter::ARpgCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	CameraBoom->SetupAttachment(GetRootComponent(), FName("CameraBoom"));
	PlayerCamera->SetupAttachment(CameraBoom, FName("PlayerCamera"));

	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bUsePawnControlRotation = true;

	PlayerCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void ARpgCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	InitPlayerHUD();
}

void ARpgCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
	InitPlayerHUD();
}

void ARpgCharacter::InitAbilityActorInfo()
{
	RpgPlayerState = GetPlayerState<ARpgPlayerState>();
	check(RpgPlayerState);
	RpgPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RpgPlayerState, this);
	Cast<URpgAbilitySystemComponent>(RpgPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = RpgPlayerState->GetAbilitySystemComponent();
	AttributeSet = RpgPlayerState->GetAttributeSet();
}

void ARpgCharacter::InitPlayerHUD()
{
	RpgPlayerController = Cast<ARpgPlayerController>(GetController());
	if (RpgPlayerController)
	{
		if (ARpgHUD* RpgHUD = Cast<ARpgHUD>(RpgPlayerController->GetHUD()))
		{
			RpgHUD->InitOverlay(RpgPlayerController, RpgPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}



