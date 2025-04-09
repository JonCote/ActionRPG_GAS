// Copyright Jonathan Cote


#include "Player/RpgPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Input/RpgInputComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/PlayerInterface.h"
#include "UI/Widget/DamageTextComponent.h"


ARpgPlayerController::ARpgPlayerController()
{
	bReplicates = true;
}

void ARpgPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	RotateToMouse();
	UpdateMagicCircleLocation();
	
}

void ARpgPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
	
}

void ARpgPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void ARpgPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCritHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		// Dynamic creation of component object
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCritHit);
	}
}


void ARpgPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorGroundHit.ImpactPoint);
	}
}

void ARpgPlayerController::UpdateMagicCircleLocation(FHitResult HitResult)
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(HitResult.ImpactPoint);
	}
}

void ARpgPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_CursorTrace))
	{
		return;
	}
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	GetHitResultUnderCursor(ECC_GROUND, false, CursorGroundHit);
	
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
		if (ThisActor->Implements<UEnemyInterface>()) { TargetingStatus = TargetingEnemy; }
		else if (ThisActor->Implements<UPlayerInterface>()) { TargetingStatus = TargetingPlayer; }
		else { TargetingStatus = TargetingInteractable; }
	}
	else
	{
		ThisActor = nullptr;
		TargetingStatus = NotTargeting;
	}
	
	
	if (LastActor == ThisActor) return;
	if (LastActor) IHighlightInterface::Execute_UnHighlightActor(LastActor);
	if (ThisActor) IHighlightInterface::Execute_HighlightActor(ThisActor);
}

void ARpgPlayerController::RotateToMouse()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_Rotation))
	{
		return;
	}
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector PawnLocation = GetFocalLocation();
		const FRotator ControllerRotator = GetControlRotation();
		const FRotator Rotation = (CursorGroundHit.Location - PawnLocation).Rotation();
		const FRotator YawRotation(ControllerRotator.Pitch, Rotation.Yaw, ControllerRotator.Roll);
		SetControlRotation(YawRotation);
	}
}

void ARpgPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	if (InputTag.MatchesTagExact(FRpgGameplayTags::Get().InputTag_LMB) && TargetingStatus == TargetingInteractable)
	{
		// TODO: Targeting a interactable object and clicked LMB on it, Do Some kind of interaction
		
	}
	
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
	
}

void ARpgPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
}

void ARpgPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	

	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	
}

URpgAbilitySystemComponent* ARpgPlayerController::GetASC()
{
	if (RpgAbilitySystemComponent == nullptr)
	{
		RpgAbilitySystemComponent = Cast<URpgAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return RpgAbilitySystemComponent;
}

void ARpgPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerInputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerInputContext, 0);	
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}


void ARpgPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	URpgInputComponent* RpgInputComponent = CastChecked<URpgInputComponent>(InputComponent);
	
	RpgInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARpgPlayerController::Move);
	RpgInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ARpgPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_Movement))
	{
		return;
	}
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FVector UpDirection(1.f, 0.f, 0.f);
	const FVector SideDirection(0.f, 1.f, 0.f);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(UpDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(SideDirection, InputAxisVector.X);
	}
}