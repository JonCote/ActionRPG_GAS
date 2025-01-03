// Copyright Jonathan Cote


#include "Character/RpgCharacter.h"

#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RpgPlayerState.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
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
	CameraBoom->bDoCollisionTest = false;  // Might want this true later to prevent camera going into scene objects

	PlayerCamera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Mage;
}

void ARpgCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	InitPlayerHUD();
	InitDefaultAttributes();
	AddCharacterAbilities();
}

void ARpgCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
	InitPlayerHUD();
	InitDefaultAttributes();
}

void ARpgCharacter::AddToXP_Implementation(const int32 InXP)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToXP(InXP);
}

void ARpgCharacter::AddToPlayerLevel_Implementation(const int32 InLevels)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToLevel(InLevels);
}

void ARpgCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToAttributePoints(InAttributePoints);
}

void ARpgCharacter::AddToSpellPoints_Implementation(const int32 InSpellPoints)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 ARpgCharacter::GetXP_Implementation() const
{
	check(RpgPlayerState);
	return RpgPlayerState->GetPlayerXP();
}

int32 ARpgCharacter::GetAttributePointsReward_Implementation(const int32 Level) const
{
	check(RpgPlayerState);
	return RpgPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 ARpgCharacter::GetSpellPointsReward_Implementation(const int32 Level) const
{
	check(RpgPlayerState);
	return RpgPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

int32 ARpgCharacter::GetAttributePoints_Implementation() const
{
	check(RpgPlayerState);
	return RpgPlayerState->GetAttributePoints();
}

int32 ARpgCharacter::GetSpellPoints_Implementation() const
{
	check(RpgPlayerState);
	return RpgPlayerState->GetSpellPoints();
}

void ARpgCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ARpgCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (!IsValid(LevelUpNiagaraComponent)) { return; }

	const FVector CameraLocation = PlayerCamera->GetComponentLocation();
	const FVector NiagaraLocation = LevelUpNiagaraComponent->GetComponentLocation();
	const FRotator ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
	LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
	LevelUpNiagaraComponent->Activate(true);
	
}

int32 ARpgCharacter::FindLevelForXP_Implementation(const int32 InXP) const
{
	check(RpgPlayerState);
	return RpgPlayerState->LevelUpInfo->FindLevelForGivenXP(InXP);
}

int32 ARpgCharacter::GetCharacterLevel_Implementation()
{
	check(RpgPlayerState);
	return RpgPlayerState->GetPlayerLevel();
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




