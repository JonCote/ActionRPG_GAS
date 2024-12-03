// Copyright Jonathan Cote


#include "Character/RpgEnemy.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "Aura/Aura.h"


ARpgEnemy::ARpgEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<URpgAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/** EGameplayEffectReplicationMode 
	 *		Full:
	 *			Use Case: Single Player
	 *			Description: Gameplay Effects are replicated to all clients
	 *
	 *		Mixed:
	 *			Use Case: Multiplayer, Player-Controlled
	 *			Description: GameplayEffects are replicated to the owning client only
	 *						 Gameplay Cues and Gameplay Tags replicated to all clients
	 *
	 *		Minimal:
	 *			Use Case: Multiplayer, AI-Controlled
	 *			Description: Gameplay Effects are not replicated
	 *						 Gameplay Cues and Gameplay Tags replicated to all clients
	 */
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<URpgAttributeSet>("AttributeSet");
}

void ARpgEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ARpgEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 ARpgEnemy::GetCharacterLevel()
{
	return Level;
}

void ARpgEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ARpgEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URpgAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
