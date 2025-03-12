// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverlayData.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UOverlayData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimInstance> OverlayAnimationBlueprint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	
};
