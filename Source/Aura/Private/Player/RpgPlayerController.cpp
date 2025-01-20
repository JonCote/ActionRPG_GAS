// Copyright Jonathan Cote


#include "Player/RpgPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Character/RpgCharacter.h"
#include "Components/DecalComponent.h"
#include "Input/RpgInputComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/DamageTextComponent.h"


ARpgPlayerController::ARpgPlayerController()
{
	bReplicates = true;
}

void ARpgPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	LineTrace();
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

void ARpgPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(LineTraceHit.ImpactPoint);
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

void ARpgPlayerController::LineTrace()
{

	const ARpgCharacter* PlayerCharacter = Cast<ARpgCharacter>(GetCharacter());
	
	const FVector CameraLocation = PlayerCharacter->PlayerCamera->GetComponentLocation();
	const FVector ForwardVector = PlayerCharacter->PlayerCamera->GetForwardVector();

	UKismetSystemLibrary::LineTraceSingle(
		GetCharacter(),
		CameraLocation,
		CameraLocation + ForwardVector * 6000.f,
		TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		LineTraceHit,
		true
	);

	if (!LineTraceHit.bBlockingHit) return;

	LastActor = ThisActor;
	
	if (IsValid(LineTraceHit.GetActor()) && LineTraceHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = LineTraceHit.GetActor();
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

	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;

	const FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
}

void ARpgPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	URpgInputComponent* RpgInputComponent = CastChecked<URpgInputComponent>(InputComponent);
	
	RpgInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARpgPlayerController::Move);
	RpgInputComponent->BindAction(LookAround, ETriggerEvent::Triggered, this, &ARpgPlayerController::Look);
	RpgInputComponent->BindAction(DisplayCursor, ETriggerEvent::Triggered, this, &ARpgPlayerController::CursorMode);

	RpgInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void ARpgPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRpgGameplayTags::Get().Player_Block_Movement))
	{
		return;
	}

	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FRotator Rotation = ControlledPawn->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARpgPlayerController::Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(-LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARpgPlayerController::CursorMode(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		bShouldShowCursor = !bShouldShowCursor;
	}
	if (bShouldShowCursor)
	{
		bShowMouseCursor = true;

		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		SetInputMode(InputModeData);
	}
	else
	{
		bShowMouseCursor = false;

		const FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);
	}
}



