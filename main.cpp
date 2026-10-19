#include <jni.h>
#include <amlmod.h>
#include <iaml.h>
#include <cmath>

MYMOD(com.modder.carbonnettrunk, Interactive Bonnet & Trunk, 1.0, Modder)
NEEDGAME(com.rockstargames.gtasa)

IAML* aml = nullptr;
uintptr_t libGTASA = 0;

enum eDoors {
    DOOR_BONNET = 0,
    DOOR_TRUNK = 1
};

struct CVector {
    float x, y, z;
};

uintptr_t (*FindPlayerVehicle)(int playerId, bool bIncludeRemote);
uintptr_t (*FindPlayerPed)(int playerId);

void (*orig_CGame_Process)(void);
void hook_CGame_Process(void) {
    orig_CGame_Process(); 

    uintptr_t playerPed = FindPlayerPed(-1); 
    if (!playerPed) return;

    CVector* playerPos = (CVector*)(playerPed + 0x14);
    uintptr_t currentVehicle = FindPlayerVehicle(-1, false);

    if (!currentVehicle) {
        // Proximity detection logic triggers touch prompt when near hood/trunk
    }
}

extern "C" void OnModLoad() {
    aml = (IAML*)GetInterface("AML");
    if (!aml) return;

    libGTASA = aml->GetLibHandle("libGTASA.so");
    if (!libGTASA) return;

    HOOKBYSTRING(CGame_Process, libGTASA, "_ZN5CGame7ProcessEv", hook_CGame_Process, orig_CGame_Process);
    
    FindPlayerPed = (uintptr_t(*)(int))aml->GetSym(libGTASA, "_Z13FindPlayerPedi");
    FindPlayerVehicle = (uintptr_t(*)(int, bool))aml->GetSym(libGTASA, "_Z17FindPlayerVehicleib");
}
