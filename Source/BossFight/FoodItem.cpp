// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "InventoryComponent.h"
#include "MainCharacter.h"

void UFoodItem::Use(class AMainCharacter* Character)
{
	if (Character) {
		Character->Health += HealthToHeal;
		if (OwnInventory) {
			OwnInventory->RemoveItem(this);
		}
	}
}
