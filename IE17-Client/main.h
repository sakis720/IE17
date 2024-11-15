#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>
#define STR_(X) #X
#define IE17ver v0.03
#define STR(X) STR_(X)

//most simple vector
struct Vector
{
    float x;
    float y;
    float z;
};

enum ETextHudTypes
{
    TEXT_DebugText = 0,
    TEXT_DebugText2,
    TEXT_SpiritGuideUpdated,
    TEXT_ObjectivesUpdated,
    TEXT_Top = 11,
    TEXT_Subtittle,
    TEXT_HelpMessage,
    TEXT_QuitWarning,
};

/*// This is all Classes for Ghosts,Ghostbuster etc..
enum Classes
{

#pragma region Ghosts
    CArchitect,
    CBiped,
    CFlyerMedium,
    CFlyerSmall,
    CGargoyle,
    CGhost,
    CBlinkers,
    CFloater,
    CNightmare,
    CSlimer,
    CGolem,
    CGolemBase,
    CAzetlor,
    CBipedLarge,
    CLibrarian,
    COrreryBoss,
    CPhantom,
    CScuttler,
    CSpiderWitch,
    CStayPuftBoss,
    CStayPuftMinion,
    CGargoyleGrabber,
    CGolemPartBase,
    CGolemPart,
    CBipedShield,
    CBipedWeapon,
#pragma endregion

#pragma region Actors
    CActor,
    CBirdFlock,
    CBoneSimActor,
    CBoneSimActorBase,
    CTreeActor,
    CBendableActor,
    CCinematicSkeletonModel,
    CHuman,
    CNPC,
    CGhostbuster,
    CPedestrian,
    CFleeingPeople,
    CCrowd,
    CUniversalJointActor,
    CHingeActor,
    CMediumPeople,
    CRats,
    CRatPeople,
#pragma endregion

#pragma region Misc
    CAINode,
    CBlackSlime,
    CGrabbable,
    CBreakable,
    CBarrier,
    CBookshelf,
    CBoxTest,
    CBookStack,
    CCable,
    CCar,
    CCarEffects,
    CEctoCarEffects,
    CVendingMachine,
    CCrushDfm,
    CCurtain,
    CDoor,
    CEctoBarrier,
    CEmitter,
    CFluid,
    CGear,
    CGlass,
    CGhostEffects,
    CGravityShift,
    CInventoryItem,
    CGhostTrap,
    CPKEMeter,
    CLightShafts,
    CLevitate,
    CLiteBulb,
    CTableCloth,
    CLookAtPoint,
    CProtonBeam,
    CViewPortal
#pragma endregion

};
*/

/*
enum ETextHudTypes
{
    TEXT_DebugText = 0,
    TEXT_GenericText = 1,
    TEXT_SpiritGuideUpdated = 2,
    TEXT_ObjectivesUpdated = 3,
    TEXT_HelpMessage = 13,
};
*/

enum ETextLegacyPrintTypes
{
    TEXTL_Default = 1u,
    TEXTL_YesCancelOption = 2,
};


extern bool g_fSlew;
extern bool m_about;
extern bool g_fGhostViewer;
extern bool m_legacycrash;
extern bool b_spawnactor;
extern bool g_fRestartLevel;
extern char* g_modBase;

extern void (*CreateActor)(const char*, Vector);
extern int (*DisplayText)(int, const char*, float);
extern int (*DisplayTextLegacy)(int, const char*, const char*, char);

DWORD WINAPI DLLAttach(HMODULE hModule);
//void SlewEnableDisable();
void AboutMod();
void ResLevel();
void TestLegacyText();
void SpawnActor();
//void GhostViewerFun();
void RunMod();