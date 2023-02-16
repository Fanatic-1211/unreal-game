#pragma once

// #include "CustomUtils.generated.h"

#include "Components/PlayerStateBase.h"

class CustomUtils
{
public:
    bool static AreEnemies(
        const AController* Controller1, const AController* Controller2) 
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

        APlayerStateBase* PlayerState1 = Cast<APlayerStateBase>(Controller1->PlayerState);
        APlayerStateBase* PlayerState2 = Cast<APlayerStateBase>(Controller2->PlayerState);
        if (!PlayerState1 || !PlayerState2) return false;

        if (PlayerState1->GetTeamID() != PlayerState2->GetTeamID())
        return true;
        else
        return false;
    }
};