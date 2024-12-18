// Copyright Jonathan Cote


#include "Character/RpgEnemy.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "AbilitySystem/RpgAttributeSet.h"
#include "Aura/Aura.h"
#include "RpgGameplayTags.h"
#include "AI/RpgAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/RpgUserWidget.h"


ARpgEnemy::ARpgEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);

	AbilitySystemComponent = CreateDefaultSubobject<URpgAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<URpgAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ARpgEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) { return; }
	RpgAIController = Cast<ARpgAIController>(NewController);
	RpgAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	RpgAIController->RunBehaviorTree(BehaviorTree);
	RpgAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	RpgAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	
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

void ARpgEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void ARpgEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ARpgEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ARpgEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		URpgAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);	
	}

	if (URpgUserWidget* RpgUserWidget = Cast<URpgUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		RpgUserWidget->SetWidgetController(this);
	}
	
	if (const URpgAttributeSet* RpgAS = CastChecked<URpgAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RpgAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RpgAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FRpgGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ARpgEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(RpgAS->GetHealth());
		OnMaxHealthChanged.Broadcast(RpgAS->GetMaxHealth());
	}
}

void ARpgEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (RpgAIController && RpgAIController->GetBlackboardComponent())
	{
		RpgAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ARpgEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URpgAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitDefaultAttributes();	
	}
}

void ARpgEnemy::InitDefaultAttributes() const
{
	URpgAbilitySystemLibrary::InitDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
