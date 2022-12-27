// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// #include "Engine/Engine.h"
#include "Components/HealthComponent.h"

#include "Citadel/Players/PlayerGround.h"

APlayerGround::APlayerGround()
{
	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateAbstractDefaultSubobject<UHealthComponent>(
            TEXT("Health"));
    
    HealthTextRender = CreateAbstractDefaultSubobject<UTextRenderComponent>(
            TEXT("HealthRenderer"));
    HealthTextRender->SetupAttachment(RootComponent);
    
    
}

void APlayerGround::BeginPlay()
{
    Super::BeginPlay(); 
    PlayerController = GetWorld()->GetFirstPlayerController();
    PlayerPawn = PlayerController->GetPawn();
    SetupHealthComponent();
    SetupWeapon();
}

// Called every frame
void APlayerGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// -------------------------------------------------------------------

void APlayerGround::SetupWeapon()
{
    Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
    Weapon->AttachToComponent(GetMesh(), 
        FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket_r"));
    Weapon->SetOwner(this);
}

void APlayerGround::SetupHealthComponent()
{
    UpdateHealthRenderText(); 
    HealthComponent->OnDeath.AddUObject(this, &APlayerGround::OnDeath); // Subscribe on C++ only delegate 
    HealthComponent->OnDamage.AddUObject(this,
             &APlayerGround::UpdateHealthRenderText);
}

// --------------------------------------------------
// INPUT FUNCTIONS
// --------------------------------------------------
void APlayerGround::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, 
            &APlayerGround::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, 
            &APlayerGround::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, 
            &APlayerGround::LookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, 
            &APlayerGround::LookRight);

}

void APlayerGround::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector(),  AxisValue);
}

void APlayerGround::MoveRight(float AxisValue)
{
    AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerGround::LookUp(float AxisValue)
{
    AddControllerPitchInput(AxisValue);
}

void APlayerGround::LookRight(float AxisValue)
{
    AddControllerYawInput(AxisValue);
}

// --------------------------------------------------


void APlayerGround::UpdateHealthRenderText()
{
	HealthTextRender->SetText(FString::SanitizeFloat(HealthComponent->GetHealth()));
}

void APlayerGround::OnDeath()
{
    PlayAnimMontage(DeathAnimMontage);
    
    if (PlayerPawn && PlayerController)
    PlayerPawn->DisableInput(PlayerController);

    SetLifeSpan(5.f);
}