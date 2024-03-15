// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "Gun.h"
#include "MainBossFightGameMode.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Setting turn rates
	RotationRate = 10.f;

	/*
	// Don't rotate when the controller rotates so it affects only the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	*/

	// Movement config
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f); // at this rate
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.05f;
	SpeedRate = 1.7f;

	// Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent); // Attach camera boom to root component
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraZoom = CameraBoom->TargetArmLength; // Initial zoom length

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to camera boom
	Camera->bUsePawnControlRotation = false; // Camera doesn't move relative to arm

	// Health
	Health = 100;
	MaxHealth = 100;

	// Inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* apc = GetWorld()->GetFirstPlayerController();
	apc->SetInputMode(FInputModeGameOnly());

	// Gun Actor
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket");
	Gun->SetOwner(this);
	
}

bool AMainCharacter::IsDead() const
{
	return Health <= 0;
}

float AMainCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Use item
void AMainCharacter::UseItem(class UItem* Item)
{
	if (Item) {
		Item->Use(this);
		Item->OnUse(this); // BP event
	}
}

// Movement
void AMainCharacter::MoveForward(float Value)
{
	if (Controller && Value) {
		if (bIsSprinting)
			Value *= SpeedRate;
		
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

		// Add movement in that direction 
		AddMovementInput(Direction, Value / SpeedRate);

	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller && Value) {
		if (bIsSprinting)
			Value *= SpeedRate;
		
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		// Add movement in that direction 
		AddMovementInput(Direction, Value / SpeedRate);

	}
}

void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * RotationRate * GetWorld()->GetDeltaSeconds());
}

// Sprint
void AMainCharacter::BeginSprint()
{
	bIsSprinting = true;
}

void AMainCharacter::StopSprint()
{
	bIsSprinting = false;
}

// Camera zoom
void AMainCharacter::CameraZoomIn()
{
	float var = 10.f;
	if (CameraZoom >= 250.f) {
		CameraBoom->TargetArmLength = CameraZoom;
		CameraZoom -= var;
	}
	else {
		CameraBoom->TargetArmLength = CameraZoom;
	}
}

void AMainCharacter::CameraZoomOut()
{
	float var = 10.f;
	if (CameraZoom <= 350.f) {
		CameraBoom->TargetArmLength = CameraZoom;
		CameraZoom += var;
	}
	else {
		CameraBoom->TargetArmLength = CameraZoom;
	}
}

// Jump
void AMainCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1) {
		Jump();
	}
}


void AMainCharacter::Shoot()
{
	Gun->PullTrigger();
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health); // Just Log output

	if (IsDead())
	{
		AMainBossFightGameMode* GameMode = GetWorld()->GetAuthGameMode<AMainBossFightGameMode>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Gameplay key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// "Turn" handles devices that provide an absolute delta, like a mouse
	// "TurnAtRate" handles devices that we choose to treat as a rate of change, like a joystick
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainCharacter::LookUpAtRate);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AMainCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AMainCharacter::StopSprint);

	// Jump
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// Handle touch inputs, like jump
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMainCharacter::TouchStarted);

	// Zoom
	PlayerInputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &AMainCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", EInputEvent::IE_Released, this, &AMainCharacter::CameraZoomOut);

	// Shoot
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AMainCharacter::Shoot);
}

