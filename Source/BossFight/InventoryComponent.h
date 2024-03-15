// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Blueprints will bind to this to update the UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOSSFIGHT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void BeginPlay() override;

	// Add/Remove items
	bool AddItem(class UItem* Item);
	bool RemoveItem(class UItem* Item);

	// Default items that u start with
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class UItem*> DefaultItems;

	// Capacity of the inventory
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 Capacity;

	// The delegate that updates the UI
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryUpdated;

	// The array of items
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<class UItem*> Items;
		
};
