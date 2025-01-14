// Copyright Jonathan Cote


#include "Actor/PointCollection.h"

#include "AbilitySystem/RpgAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Kismet/KismetMathLibrary.h"


APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Points.SetNum(11);
	int i = 0;
	for (auto& Pt : Points)
	{
		FString Name = FString::Printf(TEXT("Point_%d"), i);
		Pt = CreateDefaultSubobject<USceneComponent>(FName(Name));

		if (i == 0)
		{
			SetRootComponent(Pt);
		}
		else
		{
			Pt->SetupAttachment(GetRootComponent());
		}
		i++;

	}
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, const int32 NumPoints, const float YawOverride)
{
	checkf(Points.Num() >= NumPoints, TEXT("Attempted to access Points out of bounds."));
	TArray<USceneComponent*> ArrayCopy;
	for (USceneComponent* Pt : Points)
	{
		if (ArrayCopy.Num() >= NumPoints)
		{
			return ArrayCopy;
		}

		if (Pt != Points[0])
		{
			FVector ToPoint = Pt->GetComponentLocation() - Points[0]->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Points[0]->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LowerLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		TArray<AActor*> IgnoreActors;
		URpgAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LowerLocation, FName("BlockAll"), QueryParams);
		//GetWorld()->LineTraceSingleByChannel(HitResult, RaisedLocation, LowerLocation, ECC_GROUND, QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
		
	}

	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}


