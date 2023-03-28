// Fill out your copyright notice in the Description page of Project Settings.

#include "Citadel/Players/PlayerGround.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CustomCharacterMovementComponent.h"
#include "Weapons/WeaponRifle.h"

APlayerGround::APlayerGround(const class FObjectInitializer& ObjectInitializer)
    // overriding CharacterMovement:
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
          ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateAbstractDefaultSubobject<UHealthComponent>(TEXT("Health"));

    HealthTextRender = CreateAbstractDefaultSubobject<UTextRenderComponent>(TEXT("HealthRenderer"));
    HealthTextRender->SetupAttachment(RootComponent);

    WeaponComponent = CreateAbstractDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void APlayerGround::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = GetWorld()->GetFirstPlayerController();

    SetupHealthComponent();
}

// Called every frame
void APlayerGround::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// -------------------------------------------------------------------

void APlayerGround::SetupHealthComponent()
{
    UpdateHealthRenderText();
    HealthComponent->OnDeath.AddUObject(
        this, &APlayerGround::OnDeath);  // Subscribe on C++ only delegate
    HealthComponent->OnDamage.AddDynamic(this,
        &APlayerGround::UpdateHealthRenderText);  // Subscribe on universal
                                                  // delegate
}

// --------------------------------------------------
// INPUT FUNCTIONS
// --------------------------------------------------
void APlayerGround::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerGround::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerGround::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerGround::LookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerGround::LookRight);

    PlayerInputComponent->BindAction(
        TEXT("Fire"), IE_Pressed, WeaponComponent, &UWeaponComponent::Shoot);
    PlayerInputComponent->BindAction(
        TEXT("SwitchWeapon"), IE_Pressed, WeaponComponent, &UWeaponComponent::SwitchWeapon);

    // Stance toggling:
    DECLARE_DELEGATE_OneParam(
        FToggleStanceInputParams, PlayerStances);  // for pass parametr into function below
    PlayerInputComponent->BindAction<FToggleStanceInputParams>(
        TEXT("Crouch"), IE_Pressed, this, &APlayerGround::ToggleStance, PlayerStances::Crouching);
    DECLARE_DELEGATE_OneParam(
        FSprintInputParams, PlayerStances);  // for pass parametr into function below
    PlayerInputComponent->BindAction<FSprintInputParams>(
        TEXT("Sprint"), IE_Pressed, this, &APlayerGround::ToggleStance, PlayerStances::Sprinting);

    // Zoom:
    DECLARE_DELEGATE_OneParam(FZoomInputParams, bool);  // for pass parametr into function below
    PlayerInputComponent->BindAction<FZoomInputParams>(
        TEXT("ToggleZoom"), IE_Pressed, WeaponComponent, &UWeaponComponent::ToggleZoom, true);
    PlayerInputComponent->BindAction<FZoomInputParams>(
        TEXT("ToggleZoom"), IE_Released, WeaponComponent, &UWeaponComponent::ToggleZoom, false);
}

void APlayerGround::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerGround::MoveRight(float AxisValue)
{
    if (bSprinting) return;  // player can't strafe while sprinting

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

void APlayerGround::ToggleStance(PlayerStances Stance)
{
    if (Stance == PlayerStances::Crouching)
    {
        if (bCrouching)
        {
            bCrouching = false;
            bJogging = true;
            bSprinting = false;
        }
        else
        {
            bCrouching = true;
            bJogging = false;
            bSprinting = false;
        }
    }

    if (Stance == PlayerStances::Sprinting)
    {
        if (bSprinting)
        {
            bCrouching = false;
            bJogging = true;
            bSprinting = false;
        }
        else
        {
            bCrouching = false;
            bJogging = false;
            bSprinting = true;
            ;
        }
    }
}

// --------------------------------------------------

void APlayerGround::UpdateHealthRenderText()
{
    HealthTextRender->SetText(FString::SanitizeFloat(HealthComponent->GetHealth()));
}

void APlayerGround::OnDeath()
{
    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }

    if (PlayerController) GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (Controller) Controller->ChangeState(NAME_Spectating);

    SetLifeSpan(5.f);
}

void APlayerGround::SetPlayerColor(FLinearColor Color)
{
    auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}
