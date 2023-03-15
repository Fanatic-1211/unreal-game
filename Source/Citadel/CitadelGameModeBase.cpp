// Fill out your copyright notice in the Description page of Project Settings.

#include "CitadelGameModeBase.h"

#include "AIController.h"
#include "EngineUtils.h"

#include "Components/PlayerStateBase.h"
#include "Players/PlayerGround.h"
#include "Components/RespawnComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Log_CitadelGameModeBase, All, All);

ACitadelGameModeBase::ACitadelGameModeBase()
{
    PlayerStateClass = APlayerStateBase::StaticClass();
}

void ACitadelGameModeBase::StartPlay()
{
    Super::StartPlay();

    StartNewRound();
    SpawnBots();
    CreateTeamsInfo();
    SetMatchState(CitadelMatchState::InProgress);
}

UClass* ACitadelGameModeBase::GetDefaultPawnClassForController_Implementation(
    AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACitadelGameModeBase::ConfirmKill(AController* KillerController, AController* VictimController)
{
    UE_LOG(Log_CitadelGameModeBase, Log, TEXT("%s has killed %s!"), *KillerController->GetName(),
        *VictimController->GetName());

    APlayerStateBase* VictimState = Cast<APlayerStateBase>(VictimController->PlayerState);

    // Check should we count kill or not:
    if (KillerController)
    {
        APlayerStateBase* KillerState = Cast<APlayerStateBase>(KillerController->PlayerState);

        if (KillerController != VictimController)  // is not suicide?
        {
            KillerState->AddKill();
            UE_LOG(Log_CitadelGameModeBase, Verbose, TEXT("Kill has counted."));
        }
        else
        {
            UE_LOG(Log_CitadelGameModeBase, Log,
                TEXT("ConfirmKill: Kill has not counted, because this was a suicide."));
        }
    }

    VictimState->AddDeath();
    StartRespawnProcess(VictimController);
}

void ACitadelGameModeBase::RequestRespawn(AController* Controller)
{
    if (!Controller) return;

    ResetOnePlayer(Controller);

    UE_LOG(Log_CitadelGameModeBase, Log, TEXT("%s has respawned."), *Controller->GetName());
}

bool ACitadelGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    IsPauseSet = Super::SetPause(PC, CanUnpauseDelegate);

    if (IsPauseSet) SetMatchState(CitadelMatchState::Pause);

    UE_LOG(Log_CitadelGameModeBase, Display, TEXT("Game paused by %s."), *PC->GetName());

    return IsPauseSet;
}

bool ACitadelGameModeBase::ClearPause()
{
    IsPauseSet = !Super::ClearPause();

    if (!IsPauseSet) SetMatchState(CitadelMatchState::InProgress);

    UE_LOG(Log_CitadelGameModeBase, Display, TEXT("Game resumed."));

    return !IsPauseSet;
}

void ACitadelGameModeBase::StartNewRound()
{
    RoundCountdown = GameData.RoundDuration;

    GetWorldTimerManager().SetTimer(
        RoundTimerHandle, this, &ACitadelGameModeBase::UpdateRoundTimer, 1.f, true);

    UE_LOG(Log_CitadelGameModeBase, Display, TEXT("New Round Started!"));
}

void ACitadelGameModeBase::UpdateRoundTimer()
{
    if (--RoundCountdown == 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            StartNewRound();
            ResetPlayers();
            ++CurrentRound;
        }
        else
        {
            FinishGame();
        }
    }
}

void ACitadelGameModeBase::SpawnBots()
{
    UE_LOG(Log_CitadelGameModeBase, Verbose, TEXT("SpawnBots: Starting spawn bots..."));
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AAIController* Controller =
            GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

        RestartPlayer(Controller);
    }

    UE_LOG(Log_CitadelGameModeBase, Verbose, TEXT("SpawnBots: Bots spawn completed."));
}

void ACitadelGameModeBase::ResetPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
        ResetOnePlayer(It->Get());
}

void ACitadelGameModeBase::ResetOnePlayer(AController* PlayerController)
{
    if (PlayerController && PlayerController->GetPawn()) PlayerController->GetPawn()->Reset();

    RestartPlayer(PlayerController);  // UE GamemodeBase default function
    SetPlayerColor(PlayerController);
}

void ACitadelGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 CurrentTeamID = 1;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        AController* Controller = It->Get();
        if (!Controller) continue;

        APlayerStateBase* PlayerState = Cast<APlayerStateBase>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        PlayerState->SetTeamID(CurrentTeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(CurrentTeamID));
        SetPlayerColor(Controller);

        CurrentTeamID = CurrentTeamID == 1 ? 2 : 1;
    }
}

FLinearColor ACitadelGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    else
    {
        UE_LOG(Log_CitadelGameModeBase, Warning,
            TEXT("There is no that index in GameData.TeamColors array! Seting default color."));
        return GameData.DefaultTeamColor;
    }
}

void ACitadelGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    APlayerGround* Pawn = Cast<APlayerGround>(Controller->GetPawn());
    if (!Pawn) return;

    APlayerStateBase* PlayerState = Cast<APlayerStateBase>(Controller->PlayerState);
    if (!PlayerState) return;

    Pawn->SetPlayerColor(PlayerState->GetTeamColor());
}

void ACitadelGameModeBase::StartRespawnProcess(AController* Controller)
{
    URespawnComponent* RespawnComponent = Controller->FindComponentByClass<URespawnComponent>();

    if (!RespawnComponent)
    {
        UE_LOG(Log_CitadelGameModeBase, Error,
            TEXT("StartRespawnProcess: %s - RespawnComponent doesn't found!"),
            *Controller->GetName());
        return;
    }
    RespawnComponent->Respawn(GameData.TimeToPlayerRespawn);
}

void ACitadelGameModeBase::FinishGame()
{
    DisableAllPawns();
    UE_LOG(Log_CitadelGameModeBase, Display, TEXT("FinishGame: GAME OVER!"));
    SetMatchState(CitadelMatchState::GameOver);
}

void ACitadelGameModeBase::DisableAllPawns()
{
    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (!Pawn) continue;

        Pawn->TurnOff();
        Pawn->DisableInput(nullptr);
    }

    UE_LOG(Log_CitadelGameModeBase, Log, TEXT("DisableAllPawns: All pawn inputs disabled."));
}

void ACitadelGameModeBase::SetMatchState(CitadelMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;

    OnMatchStateChanged.Broadcast(MatchState);

    UE_LOG(Log_CitadelGameModeBase, Log, TEXT("SetMatchState: MatchState has changed to '%s'"),
        *UEnum::GetValueAsString(MatchState));
}
