// Copyright Jonathan Cote

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/LootableInterface.h"
#include "Inventory/ItemInfo.h"
#include "LootableItem.generated.h"

class USphereComponent;

UCLASS(BlueprintType, Blueprintable)
class AURA_API ALootableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ALootableItem();

	UPROPERTY(EditAnywhere, Category = "Interactable")
	TObjectPtr<UItemInfo> ItemInfo;
	

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
};
