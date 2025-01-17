// Copyright Jonathan Cote


#include "Character/RpgCharacter.h"

#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/RpgPlayerState.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/RpgGameInstance.h"
#include "Game/RpgGameModeBase.h"
#include "Kismet/GameplayStatics.h"
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
	LoadProgress();
}

void ARpgCharacter::LoadProgress()
{
	if (!HasAuthority()) { return; }
	if (const ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = RpgGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) { return; }
		if (SaveData->bFirstTimeLoadIn)
		{
			InitDefaultAttributes();
			AddCharacterAbilities();
			return;
		}

		// Load PlayerState Data
		if (RpgPlayerState)
		{
			RpgPlayerState->SetLevel(SaveData->PlayerLevel);
			RpgPlayerState->SetXP(SaveData->XP);
			RpgPlayerState->SetAttributePoints(SaveData->AttributePoints);
			RpgPlayerState->SetSpellPoints(SaveData->SpellPoints);
		}

		// Load Attribute Data
		URpgAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);

		// Load Ability Data
		if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(AbilitySystemComponent))
		{
			RpgASC->AddCharacterAbilitiesFromSaveData(SaveData);
		}
		
	}
}

void ARpgCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	if (!HasAuthority()) { return; }
	if (const ARpgGameModeBase* RpgGameMode = Cast<ARpgGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = RpgGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) { return; }

		SaveData->PlayerStartTag = CheckpointTag;
		
		SaveData->PlayerLevel = RpgPlayerState->GetPlayerLevel();
		SaveData->XP = RpgPlayerState->GetPlayerXP();
		SaveData->AttributePoints = RpgPlayerState->GetAttributePoints();
		SaveData->SpellPoints = RpgPlayerState->GetSpellPoints();
		
		SaveData->Strength = URpgAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = URpgAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = URpgAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Dexterity = URpgAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
		SaveData->bFirstTimeLoadIn = false;


		URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, RpgASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = RpgASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = URpgAbilitySystemLibrary::GetAbilityInfo(this);
			FRpgAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbilityClass = Info.AbilityClass;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityTypeTag;
			SavedAbility.AbilitySlot = RpgASC->GetSlotTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = RpgASC->GetStatusFromAbilityTag(AbilityTag);
			
		SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		RpgASC->ForEachAbility(SaveAbilityDelegate);
		
		RpgGameMode->SaveInGameProgressData(SaveData);
	}
	
}

void ARpgCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
	InitPlayerHUD();
}

void ARpgCharacter::OnRep_Stunned()
{
	// Apply Gameplay Tags locally for Debuff
	if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_Movement);
		BlockedTags.AddTag(GameplayTags.Player_Block_Rotation);
		BlockedTags.AddTag(GameplayTags.Debuff_Stun);
		if (bIsStunned)
		{
			RpgASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			RpgASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
		
	}
}

void ARpgCharacter::OnRep_Burned()
{
	// Apply Gameplay Tags locally for Debuff
	if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FRpgGameplayTags GameplayTags = FRpgGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Debuff_Burn);
		if (bIsBurned)
		{
			RpgASC->AddLooseGameplayTags(BlockedTags);
			BurnDebuffComponent->Activate();
		}
		else
		{
			RpgASC->RemoveLooseGameplayTags(BlockedTags);
			BurnDebuffComponent->Deactivate();
		}
	}
}


void ARpgCharacter::InitAbilityActorInfo()
{
	RpgPlayerState = GetPlayerState<ARpgPlayerState>();
	check(RpgPlayerState);
	RpgPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RpgPlayerState, this);
	Cast<URpgAbilitySystemComponent>(RpgPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = RpgPlayerState->GetAbilitySystemComponent();
	AttributeSet = RpgPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FRpgGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ARpgCharacter::StunTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FRpgGameplayTags::Get().Debuff_Burn, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ARpgCharacter::BurnTagChanged);
	
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


void ARpgCharacter::AddToXP_Implementation(const int32 InXP)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToXP(InXP);
}

void ARpgCharacter::AddToPlayerLevel_Implementation(const int32 InLevels)
{
	check(RpgPlayerState);
	RpgPlayerState->AddToLevel(InLevels);

	if (URpgAbilitySystemComponent* RpgASC = Cast<URpgAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		RpgASC->UpdateAbilityStatuses(RpgPlayerState->GetPlayerLevel());
	}
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

void ARpgCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	check(RpgPlayerController);
	RpgPlayerController->ShowMagicCircle(DecalMaterial);
	RpgPlayerController->bShowMouseCursor = false;
}

void ARpgCharacter::HideMagicCircle_Implementation()
{
	check(RpgPlayerController);
	RpgPlayerController->HideMagicCircle();
	RpgPlayerController->bShowMouseCursor = true;
}

void ARpgCharacter::UpdateMagicCircleLocation_Implementation(FHitResult HitResult)
{
	check(RpgPlayerController);
	RpgPlayerController->UpdateMagicCircleLocation(HitResult);
}


int32 ARpgCharacter::GetCharacterLevel_Implementation()
{
	check(RpgPlayerState);
	return RpgPlayerState->GetPlayerLevel();
}






