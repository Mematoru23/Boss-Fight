// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems) {
		AddItem(Item);
	}

}

bool UInventoryComponent::AddItem(class UItem* Item)
{
	// Not allowing to store more than max capacity or if the item is null
	if (Items.Num() >= Capacity || !Item) {
		return false;
	}

	// "Telling" the item what world it's in 
	Item->OwnInventory = this;
	Item->World = GetWorld();

	// Add the item and update UI
	Items.Add(Item);
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(class UItem* Item)
{
	if (Item) {
		// Not allowing the item in the inventory and world
		Item->OwnInventory = nullptr;
		Item->World = nullptr;

		// Remove the item and update UI
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}


