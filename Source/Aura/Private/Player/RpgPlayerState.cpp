// Copyright Jonathan Cote


#include "Player/RpgPlayerState.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "Net/UnrealNetwork.h"


ARpgPlayerState::ARpgPlayerState()
{
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
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<URpgAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ARpgPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARpgPlayerState, Level);
}

UAbilitySystemComponent* ARpgPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARpgPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
