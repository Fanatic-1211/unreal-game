// Fill out your copyright notice in the Description page of Project Settings.

#include "Citadel/Players/PlayerGround.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CustomCharacterMovementComponent.h"
#include "Weapons/WeaponRifle.h"

APlayerGround::APlayerGround(const class FObjectInitializer& ObjectInitializer)
    // overriding CharacterMovement:
    : Super(ObjectInitializer
                .SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(
                    ACharacter::CharacterMovementComponentName)),
      // binding to create session delegate (multiplayer):
      CreateSessionCompleteDelegate(
          FOnCreateSessionCompleteDelegate::CreateUObject(
              this, &APlayerGround::OnCreateSessionComplete)),
      // binding to find session delegate (multiplayer):
      FindSessionsCompleteDelegate(
          FOnFindSessionsCompleteDelegate::CreateUObject(
              this, &APlayerGround::OnFindSessionsComplete)),
      // binding to join session delegate (multiplayer):
      JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(
          this, &APlayerGround::OnJoinSessionComplete))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent =
        CreateAbstractDefaultSubobject<UHealthComponent>(TEXT("Health"));

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

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
    }
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
void APlayerGround::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(
        TEXT("MoveForward"), this, &APlayerGround::MoveForward);
    PlayerInputComponent->BindAxis(
        TEXT("MoveRight"), this, &APlayerGround::MoveRight);
    PlayerInputComponent->BindAxis(
        TEXT("LookUp"), this, &APlayerGround::LookUp);
    PlayerInputComponent->BindAxis(
        TEXT("LookRight"), this, &APlayerGround::LookRight);
    PlayerInputComponent->BindAction(
        TEXT("Crouch"), IE_Pressed, this, &APlayerGround::ToggleCrouch);
    PlayerInputComponent->BindAction(
        TEXT("Run"), IE_Pressed, this, &APlayerGround::ToggleRun);
    PlayerInputComponent->BindAction(
        TEXT("Fire"), IE_Pressed, WeaponComponent, &UWeaponComponent::Shoot);
    PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed,
        WeaponComponent, &UWeaponComponent::SwitchWeapon);

    DECLARE_DELEGATE_OneParam(
        FZoomInputParams, bool);  // delegate is just for pass parametr into
                                  // Zoom-function below
    PlayerInputComponent->BindAction<FZoomInputParams>(TEXT("ToggleZoom"),
        IE_Pressed, WeaponComponent, &UWeaponComponent::ToggleZoom, true);
    PlayerInputComponent->BindAction<FZoomInputParams>(TEXT("ToggleZoom"),
        IE_Released, WeaponComponent, &UWeaponComponent::ToggleZoom, false);
}

void APlayerGround::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector(), AxisValue);
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
    if (IsCrouching)
    {
        IsCrouching = false;
        IsRunning = true;
    }
    else
    {
        IsCrouching = true;
        IsRunning = false;
    }
}

void APlayerGround::ToggleRun()
{
    if (IsCrouching == true) IsCrouching = false;

    (IsRunning == true) ? IsRunning = false : IsRunning = true;
}

// --------------------------------------------------

void APlayerGround::UpdateHealthRenderText()
{
    HealthTextRender->SetText(
        FString::SanitizeFloat(HealthComponent->GetHealth()));
}

void APlayerGround::OnDeath()
{
    // PlayAnimMontage(DeathAnimMontage);

    if (GetMesh())
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }

    if (PlayerPawn && PlayerController)
        GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Ignore);

    if (Controller) Controller->ChangeState(NAME_Spectating);

    SetLifeSpan(5.f);
}

void APlayerGround::SetPlayerColor(FLinearColor Color)
{
    auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

// --------------------------------------------------
// MULTIPLAYER FUNCTIONS
// --------------------------------------------------

void APlayerGround::CreateGameSession()
{
    if (!OnlineSessionInterface.IsValid()) return;

    // Check if old gamesession exists:
    auto ExistingSession =
        OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession)
        OnlineSessionInterface->DestroySession(NAME_GameSession);

    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
        CreateSessionCompleteDelegate);

    TSharedPtr<FOnlineSessionSettings> SessionSettings =
        MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = false;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bAllowJoinViaPresence =
        true;  // check world region for join game
    SessionSettings->bUsesPresence =
        true;  // use player world region for search game
    SessionSettings->bShouldAdvertise =
        true;  // allow find your session in public
    SessionSettings->Set(FName("MatchType"), FString("FreeForAllCepk"),
        EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);  // set match
                                                                 // type

    const ULocalPlayer* LocalPlayer =
        GetWorld()->GetFirstLocalPlayerFromController();

    OnlineSessionInterface->CreateSession(
        *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession,
        *SessionSettings);
}

void APlayerGround::JoinGameSession()
{
    GEngine->AddOnScreenDebugMessage(
        -1, 3.f, FColor::Yellow, TEXT("Trying to connect..."), true);

    if (!OnlineSessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 3.f, FColor::Red, TEXT("Connection failed :("), true);
        return;
    }

    

    OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
        FindSessionsCompleteDelegate);

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->bIsLanQuery = false;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true,
        EOnlineComparisonOp::Equals);  // search only in player world region

    const ULocalPlayer* LocalPlayer =
        GetWorld()->GetFirstLocalPlayerFromController();

    OnlineSessionInterface->FindSessions(
        *LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void APlayerGround::OnCreateSessionComplete(
    FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 3.f, FColor::Green, TEXT("Session created successfully!"), true);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 3.f, FColor::Red, TEXT("Session creation failed!"), true);
        return;
    }

    if (GetWorld())
    {
        GetWorld()->ServerTravel(LobbyLevelPath + "?listen");
    }
}

void APlayerGround::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (!OnlineSessionInterface.IsValid()) return;

    for (auto Result : SessionSearch->SearchResults)
    {
        FString Id = Result.GetSessionIdStr();
        FString User = Result.Session.OwningUserName;
        FString MatchType;
        Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
            FString::Printf(TEXT("Id: %s, User: %s"), *Id, *User));

        if (MatchType == FString("FreeForAllCepk"))
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 3.f, FColor::Green, TEXT("Game found!"), true);

            const ULocalPlayer* LocalPlayer =
                GetWorld()->GetFirstLocalPlayerFromController();

            OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
                JoinSessionCompleteDelegate);

            OnlineSessionInterface->JoinSession(
                *LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession,
                Result);

            return;
        }
    }
    GEngine->AddOnScreenDebugMessage(
        -1, 3.f, FColor::Red, TEXT("Can't find any games!"), true);
}
void APlayerGround::OnJoinSessionComplete(
    FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!OnlineSessionInterface.IsValid()) return;

    FString Address;
    if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
    {
        APlayerController* PController =
            GetGameInstance()->GetFirstLocalPlayerController();
        if (PController)
        {
            PController->ClientTravel(
                Address, ETravelType::TRAVEL_Absolute);
        }
    }
}
