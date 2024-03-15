// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class AGun;

UCLASS(config = Game)
class BOSSFIGHT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Camera things
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	// Turn rate
	UPROPERTY(EditAnywhere, Category = Camera)
	float RotationRate;
	
	// Speed rate
	UPROPERTY(EditAnywhere)
	float SpeedRate;

	// Use item
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem(class UItem* Item);

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Check if dead
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Return CameraBoom and Camera subobject
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Shoot
	void Shoot();

private:
	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void TurnAtRate(float Rate);
	void LookUp(float Value);
	void LookUpAtRate(float Rate);
	void BeginSprint();
	void StopSprint();
	bool bIsSprinting = false;

	// Camera zoom
	void CameraZoomIn();
	void CameraZoomOut();
	float CameraZoom;

	// Handler when a touch input begins
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	// Gun
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	
};
