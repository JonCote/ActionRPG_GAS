// Copyright Jonathan Cote


#include "Player/RpgPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "RpgGameplayTags.h"
#include "AbilitySystem/RpgAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/RpgInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"


ARpgPlayerController::ARpgPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ARpgPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	RotateToMouse();
	AutoRun();
	
}

void ARpgPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		// Dynamic creation of component object
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

void ARpgPlayerController::AutoRun()
{
	if (!bClickToMove || !bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ARpgPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor == ThisActor) return;
	if (LastActor) LastActor->UnHighlightActor();
	if (ThisActor) ThisActor->HighlightActor();
}

void ARpgPlayerController::RotateToMouse()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector PawnLocation = GetFocalLocation();
		const FRotator ControllerRotator = GetControlRotation();
		const FRotator Rotation = (CursorHit.Location - PawnLocation).Rotation();
		const FRotator YawRotation(ControllerRotator.Pitch, Rotation.Yaw, ControllerRotator.Roll);
		SetControlRotation(YawRotation);
	}
}

void ARpgPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FRpgGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		
		if (bClickToMove)
		{
			bAutoRunning = false;
		}
	}
	
}

void ARpgPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FRpgGameplayTags::Get().InputTag_LMB) || bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else if (bClickToMove)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				CachedDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	bTargeting = false;
}

void ARpgPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	
	if (!InputTag.MatchesTagExact(FRpgGameplayTags::Get().InputTag_LMB) || bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else if (bClickToMove)
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	
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

	if (!bClickToMove)
	{
		RpgInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARpgPlayerController::Move);
	}
	
	RpgInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ARpgPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FVector UpDirection(1.f, 0.f, 0.f);
	const FVector SideDirection(0.f, 1.f, 0.f);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(UpDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(SideDirection, InputAxisVector.X);
	}
}

