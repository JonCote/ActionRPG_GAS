// Copyright Jonathan Cote


#include "Player/RpgPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


ARpgPlayerController::ARpgPlayerController()
{
	bReplicates = true;
	
}

void ARpgPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void ARpgPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. Cases:
	 *	1. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	2. LastActor is valid && ThisActor is valid && LastActor == ThisActor
	 *		- Do nothing
	 *	3. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	4. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	5. LastActor is valid && ThisActor is valid && LastActor != ThisActor
	 *		- Highlight ThisActor
	 *		- UnHighlight LastActor
	 */
	if (LastActor == nullptr && ThisActor == nullptr) return;
	if (LastActor == ThisActor) return;

	if (LastActor == nullptr && ThisActor != nullptr)
	{
		ThisActor->HighlightActor();
	}
	else if (LastActor != nullptr && ThisActor == nullptr)
	{
		LastActor->UnHighlightActor();
	}
	else
	{
		ThisActor->HighlightActor();
		LastActor->UnHighlightActor();
	}
	
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARpgPlayerController::Move);
	EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &ARpgPlayerController::RotateCamera);
}

void ARpgPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ARpgPlayerController::RotateCamera(const FInputActionValue& InputActionValue)
{
	/**
	 * Can't seem to figure out why camera wont rotate at all...
	 */
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y);
	}
}

