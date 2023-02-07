// Fill out your copyright notice in the Description page of Project Settings.


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

#include "Citadel/Players/PlayerGround.h"

APlayerGround::APlayerGround(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
        ACharacter::CharacterMovementComponentName)) // overriding CharacterMovementComponent
{
	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateAbstractDefaultSubobject<UHealthComponent>(
            TEXT("Health"));
    
    HealthTextRender = CreateAbstractDefaultSubobject<UTextRenderComponent>(
            TEXT("HealthRenderer"));
    HealthTextRender->SetupAttachment(RootComponent);

    WeaponComponent = CreateAbstractDefaultSubobject<UWeaponComponent>(
            TEXT("WeaponComponent"));
    
}

void APlayerGround::BeginPlay()
{
    Super::BeginPlay(); 
    PlayerController = GetWorld()->GetFirstPlayerController();
    PlayerPawn = PlayerController->GetPawn();

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
    HealthComponent->OnDeath.AddUObject(this, &APlayerGround::OnDeath); // Subscribe on C++ only delegate 
    HealthComponent->OnDamage.AddDynamic(this,
             &APlayerGround::UpdateHealthRenderText); // Subscribe on universal delegate
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
    PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this,
            &APlayerGround::ToggleCrouch);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this,
            &APlayerGround::ToggleRun);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this,
            &APlayerGround::ToggleRun);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, WeaponComponent,
            &UWeaponComponent::Shoot);
    PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed, WeaponComponent,
            &UWeaponComponent::SwitchWeapon);

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

void APlayerGround::ToggleCrouch()
{
    if (IsRunning == true) 
    IsRunning = false;

    (IsCrouching == true) ? IsCrouching = false : IsCrouching = true; 
}

void APlayerGround::ToggleRun()
{
    if (IsCrouching == true) 
    IsCrouching = false;

    (IsRunning == true) ? IsRunning = false : IsRunning = true; 
}


// --------------------------------------------------


void APlayerGround::UpdateHealthRenderText()
{
        HealthTextRender->SetText(FString::SanitizeFloat(HealthComponent->GetHealth()));
}

void APlayerGround::OnDeath()
{
    //PlayAnimMontage(DeathAnimMontage);

    if(GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }
    
    
    if (PlayerPawn && PlayerController)
    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Ignore);

    if (Controller)
    Controller->ChangeState(NAME_Spectating);

    SetLifeSpan(5.f);
}
