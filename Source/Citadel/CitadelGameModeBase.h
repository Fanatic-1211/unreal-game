// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Dev/CustomTypes.h"
#include "CitadelGameModeBase.generated.h"

class AAIController;
UCLASS()
class CITADEL_API ACitadelGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACitadelGameModeBase();

    FOnMatchStateChangedSignature OnMatchStateChanged;

    // Default UE methods for override:
    virtual void StartPlay() override;  // calls Begin Play on all Actors
    virtual bool ClearPause() override;
    virtual bool SetPause(
        APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(
        AController* InController) override;

    //
    // CUSTOM METHODS:

    // Checks the circumstances of victioms death and decides whether to count the kill to the
    // killer
    virtual void ConfirmKill(AController* KillerController, AController* VictimController);

    // Public method for call private ResetOnePlayer method
    void RequestRespawn(AController* Controller);

    //
    // GETTERS:

    CitadelMatchState GetMatchState() { return MatchState; };
    int32 GetCurrentRoundNum() { return CurrentRound; };
    int32 GetRoundCountdown() { return RoundCountdown; };
    int32 GetTotalRoundsNum() { return GameData.RoundsNum; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    CitadelMatchState MatchState = CitadelMatchState::WaitingToStart;
    int32 CurrentRound = 1;
    int32 RoundCountdown = 0;
    FTimerHandle RoundTimerHandle;
    bool IsPauseSet = false;

    void SetMatchState(CitadelMatchState State);
    void StartNewRound();

    // Updates RoundTimer and checks is Round time's up
    void UpdateRoundTimer();

    // Spawns pawn for controller
    void ResetOnePlayer(AController* PlayerController);

    // Calls ResetOnePlayer for all controllers in the game
    void ResetPlayers();
    void SpawnBots();

    FLinearColor DetermineColorByTeamID(int32 TeamID) const;

    // Iterates through all Controllers and dispense them evenly into two teams
    void CreateTeamsInfo();

    // Calls SetPlayerColor in controller pawn
    void SetPlayerColor(AController* Controller);

    // Calls Respawn method from Player RespawnComponent
    void StartRespawnProcess(AController* Controller);

    void FinishGame();
    void DisableAllPawns();
};
