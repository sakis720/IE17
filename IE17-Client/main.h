#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>
#include <string>
#define STR_(X) #X
#define IE17ver v0.06
#define STR(X) STR_(X)

//most simple vector
struct Vector
{
    float x;
    float y;
    float z;
};

enum EFlashlightMode
{
    eFlashlightModeOff = 0,
    eFlashlightModeNormal = 1,
    eFlashlightModeUVLight = 2,
    eFlashlightModeCheap = 3,
};

enum EGhostbusterFacialExpression
{
    eGBFacialExpression_Normal = 0,
    eGBFacialExpression_Sad = 1,
    eGBFacialExpression_Angry = 2,
    eGBFacialExpression_Happy = 3,
    eGBFacialExpression_Fear = 4,
    eGBFacialExpression_Disgust = 5,
    eGBFacialExpression_Surprise = 6,
    eGBFacialExpression_Grimace = 7,
    eGBFacialExpression_Hurt = 8,
    eGBFacialExpression_Unconscious = 9,
    eGBFacialExpression_BrowUp = 10,
    eGBFacialExpression_BrowDown = 11,
};

enum EGoggles
{
    eGogglesOnHead = 0,
    eGogglesOnFace = 1,
    eGogglesOnBelt = 2,
};

enum EInventoryItem
{
    eInventoryInvalid = -1,
    eInventoryNothing = 0,
    eInventoryProtonGun = 1,
    eInventoryShotgun = 2,
    eInventoryRailgun = 3,
    eInventoryTrap = 4,
    eInventoryMeter = 5,
    eInventoryLight = 6,
    eInventorySlimeGun = 7
};


enum ETextHudTypes
{
    TEXT_DebugText = 0,
    TEXT_DebugText2,
    TEXT_SpiritGuideUpdated,
    TEXT_ObjectivesUpdated,
    TEXT_Top = 11,
    TEXT_Subtitle,
    TEXT_HelpMessage,
    TEXT_QuitWarning
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

enum eButtonAction
{
    eButtonAction_WrangleSlam = 0,
    eButtonAction_BlastBeam,
    eButtonAction_ContainBeam,
    eButtonAction_BosonDart,
    eButtonAction_Shotgun,
    eButtonAction_IceBeam,
    eButtonAction_Railgun,
    eButtonAction_RailgunAlt,
    eButtonAction_SlimeGun,
    eButtonAction_TetherGun,
    eButtonAction_Compass,
    eButtonAction_ManualVent = 11,
    eButtonAction_DeployTrap,
    eButtonAction_RecoverTrap,
    eButtonAction_PKEScan,
    eButtonAction_PKEEquip,
    eButtonAction_PKEChirpA,
    eButtonAction_PKEChirpB,
    eButtonAction_PKEChirpC,
    eButtonAction_Medic = 19,
    eButtonAction_InteractNode,
    eButtonAction_Jump,
    eButtonAction_Pole,
    eButtonAction_AdvanceTutorial,
    eButtonAction_Quit = 24,
    eButtonAction_RestartCheckpoint,
    eButtonAction_AccessSpiritGuide,
    eButtonAction_AccessUpgradeMenu,
    eButtonAction_AccessArtifactsMenu,
    eButtonAction_AccessContainmentViewer,
    eButtonAction_NewEquipment,
    eButtonAction_ConfirmQuit,
    eButtonAction_CancelQuit = 32
};


enum ETextLegacyPrintTypes
{
    TEXTL_Default = 1u,
    TEXTL_YesCancelOption = 2,
};

extern bool holsterBool;

extern bool m_about;
extern bool m_legacycrash;
extern bool b_spawnactor;
extern bool g_fRestartLevel;
extern char* g_modBase;
extern bool keyholsterPressed;
extern bool wasQPressed;

extern Vector playerPos;

extern bool (*isTrapDeployed)(unsigned __int64);
extern void (*gatherAllDeployedInventoryItems)(unsigned __int64);
extern void (*readyInventoryItem)(unsigned __int64, int, bool);
extern void (*enableInventoryItem)(unsigned __int64, int, bool);
extern void (*isPackOverheated)(unsigned __int64);
extern void (*setGoggleLocation)(unsigned __int64, int);
extern void (*setFacialExpression)(unsigned __int64, int);
extern void (*stopControllingActor)(unsigned __int64);
extern int (*warpTo)(unsigned __int64, Vector, Vector);
extern void (*fakePossession)(unsigned __int64, bool);
extern void (*setFlashlightMode)(unsigned __int64, int);
extern void (*commitSuicide)(unsigned __int64);
extern void (*setHealth)(unsigned __int64, float);
extern void (*setNothingEquipped)(unsigned __int64, bool);
extern void (*enableAllLights)(bool*);
extern void (*DanteVMaddExport)(const char*, const char*, int);
extern void (*loadcheckpoint)(const char*);
extern void (*buttonPrompt)(int, float);
extern void (*setAllowDamageTally)(bool*);
extern void (*fade)(float, float, float, float, float);
extern void (*displaySplashScreen)(const char*, float, bool, bool);
extern void (*CacheEffect)(const char**);
extern int (*StartEffect)(const char*, Vector, Vector);
extern void (*CreateExplosion)(Vector, float, float, float);
extern void (*SetGravity)(Vector);
extern void (*AddLight)(Vector, float, Vector, float, float, float, float);
extern int (*DisplayText)(int, const char*, float);
extern int (*DisplayTextLegacy)(int, const char*, const char*, char);

DWORD WINAPI DLLAttach(HMODULE hModule);
void HandleInput();
void AboutMod();
void OHKO();
void GodMode(bool state);
void ResLevel();
void TestLegacyText();
void SpawnActor();
std::string GetCurLevel();
void fadein();
void OpenShop(Vector);
void HandleShopChoice(int, Vector);
int CheckPlayerInput();
void RunMod();