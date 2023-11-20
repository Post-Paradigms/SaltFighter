// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterMoveFunctions.h"

FighterMoveFunctions::FighterMoveFunctions()
{
}

FighterMoveFunctions::~FighterMoveFunctions()
{
}
/*

void FighterMoveFunctions::LightNormal(EFighterState CurrentState) {
    // string var name 
    FName AttkName = "";
    if (CurrentState == EFighterState::JUMPING) {
        AttkName = "jumpnameidk";
    }
    else {
        AttkName = "LightAttk";
    }
    PerformNormal(AttkName);
}

void FighterMoveFunctions::HeavyNormal(EFighterState CurrentState) {
    if (CurrentState == EFighterState::JUMPING) {
        // woof jump logic woof
    }
    else {

    }
}
*/
/* //input buffer sends in 236
void AFighter::meow(bool light) {
    if (!UpdateState(EFighterState::Startup)) { return; }
    if (IsLeftSide) {
        //we do 236, which is forward scoop special
        if (light) {
            PerformSpecial("LightForwardFireball");
        } else {
            PerformSpecial("HeavyForwardFireball");
        }
    } else {
        //we're on the right, so this is backwards scoop special, 214 technically relative to the player
            PerformSpecial("BackwardsFireball");
    }

}*/