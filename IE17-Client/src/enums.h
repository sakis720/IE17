#ifndef ENUMS_E
#define ENUMS_E

#pragma once

enum EFlashlightMode
{
    eFlashlightModeOff = 0,
    eFlashlightModeNormal = 1,
    eFlashlightModeUVLight = 2,
    eFlashlightModeCheap = 3,
};

enum EPlayPattern
{
    ePlayPatternNone = 0,
    ePlayPatternSaw = 1,
    ePlayPatternTriangle = 2,
    ePlayPatternSine = 3,
    ePlayPatternMax = 4
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

enum ETeam
{
    eTeamAll = -1,
    eTeamSelf = 0,
    eTeamRanger = 1,
    eTeamInsurgent = 2,
    eTeamInsurgentMobFill = 3,
    eTeamGhost = 4,
    eTeamGhostbuster = 5,
    eTeamNpc = 6
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
    eInventoryMeter = 5, //pke
    eInventoryLight = 6, //pke anim for npc
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

enum ETextAlignX
{
    eTextAlignX_Left = 0,
    eTextAlignX_Center = 1,
    eTextAlignX_Right = 2,
};

enum ETextAlignY
{
    eTextAlignY_Top = 0,
    eTextAlignY_Center = 1,
};
#endif