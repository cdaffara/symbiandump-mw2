/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Common definition
*
*/


#ifndef MPXPLAYBACKFRAMEWORKDEFS_H
#define MPXPLAYBACKFRAMEWORKDEFS_H


#include <e32base.h>
#include <mpxplaybackcommanddefs.h> // TMPXPlaybackCommand. After all of clients
                                    // inlcude this file. this line can be removed.
#include <mpxplaybackplugin.hrh> // TMPXPlaybackPlayerType

// 2s - time into the track after which 'back' replays track, else previous track
const TInt KPbReplayThresholdMiliSeconds=2000;
// 4s - fades into track over KPbFadeInDurationMicroSeconds
const TInt KPbFadeInDurationMicroSeconds=4000000;
// 1s - updates position every KPbPosUpdateMicroSeconds
const TInt KPbPosUpdateMicroSeconds=1000000;
// 3s - time taken to fade in after a call
const TInt KPbResumeWaitTimeMicroSeconds=3000000;
// 0.5s - interval during which media keys will be buffered
const TInt KPbMediaKeyIntervalMicroSeconds=500000;
const TInt KPbSeekIntervalMicroSeconds=300000; // 0.3s  - how often to update
const TInt KPbInitialSeekStepMilliSeconds=1000; // 1s (1000 millisecond) - first seek step
const TInt KPbMaxSeekStepPercent=5; // Maximum step size is 5% of duration
const TInt KPbSeekAccelerationFactor=2; // Each step doubles acceleration speed
// Multiplier from second to microsecond
const TInt KPbMicroMultiplier = 1000000;
// Multiplier from second to millisecond
const TInt KPbMilliMultiplier = 1000;
// Minimum volume level
const TInt KPbPlaybackVolumeLevelMin = 0;
// Maximum volume level
const TInt KPbPlaybackVolumeLevelMax = 100;
// Default volume level
const TInt KMPXPlaybackDefaultVolume = 40;
// Position threshold for changes (in miliseconds)
const TInt KPbPositionChangeThreshold = 1500;
// Duration change threshold (in miliseconds)
const TInt KPbDurationChangeThreshold = 1500;

// UID definition for playback mode
// For application playback mode, application UID can be used
//
const TUid KPbModeDefault = {0x10282930}; // Uses same player as any other
                                          // player running in same
const TUid KPbModeActivePlayer = {0x10282931}; // Currently active player or any player if no active player.
const TUid KPbModeNewPlayer = {0x10282932}; // New player


// Category for playback utility creation.
enum TMPXCategory
    {
    EMPXCategoryUndefined = 0,
    EMPXCategoryMusic,
    EMPXCategoryVideo
    };

enum TMPXPlaybackState // State of current player
    {
    EPbStateNotInitialised, // 0
    EPbStateInitialising,   // 1
    EPbStatePlaying,        // 2
    EPbStatePaused,         // 3
    EPbStateStopped,        // 4
    EPbStateSeekingForward, // 5
    EPbStateSeekingBackward,// 6
    EPbStateShuttingDown,   // 7
    EPbStateBuffering,      // 8
    EPbStateDownloading,    // 9
    EPbStatePluginSeeking,  // 10	
    EPbStateInitialised    // 11    
    };

enum // Supported features
    {
    EPbFeatureBalance=0x01,
    EPbFeatureEmbeddedMode=0x04,
    EPbFeatureCrossFade=0x08,
    EPbFeatureFadeIn=0x10,
    EPbFeatureSeekable=0x20,
    EPbFeatureNetwork=0x40,
    EPbFeatureVolumeRamp=0x80,
    EPbFeaturePdPausable=0x1000 //progress download pausable
    };

/**
*  Properties that can be queried, but not all can be set.
*  Some properties pertain to the specific track, some to
*  the specific player, and others are general
*/
enum TMPXPlaybackProperty
    {
    EPbPropertyVolume,    // 0 volume level values 0-KPbPlaybackVolumeLevelMax
    EPbPropertyMaxVolume, // 1 read only, KPbPlaybackVolumeLevelMax
    EPbPropertyVolumeRamp,// 2
    EPbPropertyMute,      // 3 values 0(normal), 1(muted)
    EPbPropertyBalance,   // 4 values -100(left) - 0 - +100(right)
    EPbPropertyEmbeddedMode,// 5
    EPbPropertyCrossFade,   // 6
    EPbPropertyRandomMode,  // 7
    EPbPropertyRepeatMode,  // 8
    EPbPropertyAccessPoint, // 9
    EPbPropertyPosition,    // 10 depends on the plugin, it's safer to pause
                            // the playing item before setting position
    EPbPropertyDuration,    // 11 read only, millisecond
    EPbPropertySongValid,   // 12
    EPbPropertyRemote,      // 13
    EPbPropertySupportedFeatures,// 14 read only
    EPbPropertyNum               // 15 Last
    };

enum TMPXPlaybackRepeatMode // Repeat settings
    {
    EPbRepeatOff, // No repeat
    EPbRepeatOne, // Play one track over and over again
    EPbRepeatAll  // Repeat the whole play list
    };

enum // Balance settings
    {
    EPbBalanceMaxLeft=-100,
    EPbBalanceCenter=0,
    EPbBalanceMaxRight=100
    };

#ifdef __ACCESSORY_FW

/**
* accessory modes:
* EPbAccessoryHandPortable       Hand-portable
* EPbAccessoryWiredHeadset       Wired headset
* EPbAccessoryWirelessHeadset    Wireless headset
* EPbAccessoryWiredCarKit        Wired carkit
* EPbAccessoryWirelessCarKit     Wireless carkit
* EPbAccessoryTextDevice         TTY
* EPbAccessoryLoopset            Loopset
* EPbAccessoryMusicStand         Musicstand
*/
enum TMPXPlaybackAccessoryMode
    {
    EPbAccessoryModeUnknown        = -1,
    EPbAccessoryHandPortable       = 0x00000000,
    EPbAccessoryWiredHeadset       = 0x00000001,
    EPbAccessoryWirelessHeadset    = 0x00000002,
    EPbAccessoryWiredCarKit        = 0x00000004,
    EPbAccessoryWirelessCarKit     = 0x00000008,
    EPbAccessoryTextDevice         = 0x00000010,
    EPbAccessoryLoopset            = 0x00000020,
    EPbAccessoryMusicStand         = 0x00000040,
    EPbAccessoryTVOut              = 0x00000080,
    EPbAccessoryHeadphones         = 0x00000100,
    EPbAccessoryHDMI               = 0x00000200
    };
#else
/**
* accessory modes:
* EPbAccessoryUnknown          Accessory mode is unknown
* EPbAccessoryNone             No external accessories connected.
* EPbAccessoryUnsupported      Unknown accessory connected.
* EPbAccessoryDataCable        Connected with data cable.
* EPbAccessoryHeadset          Headset connected.
* EPbAccessoryLoopset          Loopset connected.
* EPbAccessoryCarKit           CarKit connected.
* EPbAccessoryMic              External mic connected.
* EPbAccessoryTty              TTY connected.
* EPbAccessoryBtHeadset        BT Headset connected.
* EPbAccessoryBtCarKit         BT CarKit connected.
*/
enum TMPXPlaybackAccessoryMode
    {
    EPbAccessoryModeUnknown  = -1,
    EPbAccessoryNone         = 0x00000000,
    EPbAccessoryUnsupported  = 0x00000001,
    EPbAccessoryDataCable    = 0x00000002,
    EPbAccessoryHeadset      = 0x00000004,
    EPbAccessoryLoopset      = 0x00000008,
    EPbAccessoryCarKit       = 0x00000010,
    EPbAccessoryMic          = 0x00000020,
    EPbAccessoryTty          = 0x00000040,
    EPbAccessoryBtHeadset    = 0x00000080,
    EPbAccessoryBtCarKit     = 0x00000100,
    };
#endif

// Server function codes
enum TMPXPlaybackServerOp
    {
    EPbsGetNextMessage,           // 0 Must be 0
    EPbsCancelGetMessage,         // 1 Must be 1
    EPbsSetMode,                  // 2 Must be called before any other following op
    EPbsGetClients,               // 3
    EPbsInitFromCollection,       // 4
    EPbsInitFromUri,              // 5
    EPbsInitFromFile,             // 6
    EPbsCancelRequest,            // 7
    EPbsCommand,                  // 8
    EPbsCommandExt = EPbsCommand, // DEPRECATED to be removed
    EPbsGetState,                 // 9
    EPbsSetProperty,              // 10
    EPbsGetProperty,              // 11
    EPbsGetPlayerTypes,           // 12
    EPbsGetPlayerTypeDisplayName, // 13
    EPbsGetAllPlayersUids,        // 14
    EPbsGetPlayersUidsForType,    // 15
    EPbsGetSubPlayerNamesByUid,   // 16
    EPbsSelectPlayerByType,       // 17
    EPbsSelectPlayerByUid,        // 18
    EPbsSelectSubPlayer,          // 19
    EPbsClearPlayerSelection,     // 20
    EPbsGetSelection,             // 21
    EPbsPlayerFound,              // 22
    EPbsGetPlayerType,            // 23
    EPbsGetTypeName,              // 24
    EPbsGetSubPlayerIndex,        // 25
    EPbsGetPlayerUid,             // 26
    EPbsGetCollectionPlaylist,    // 27
    EPbsGetFile,                  // 28
    EPbsGetUri,                   // 29
    EPbsGetMedia,                 // 30
    EPbsGetSupportedMimeTypes,    // 31
    EPbsGetSupportedExtensions,   // 32
    EPbsGetSupportedSchemas,      // 33
    EPbsGetSyncBuffer, // 34 Gets server side buffer following client sync request
    EPbsGetAsyncBuffer, // 35 Gets server side buffer following client async request
    EPbsCustomCommandSyncReceive,   // 36
    EPbsCustomCommandSyncNoReceive, // 37
    EPbsCustomCommandAsyncReceive,  // 38
    EPbsCustomCommandAsyncNoReceive,// 39
    EPbsInitStreamingFromUri,       // 40
    EPbsInitStreamingFromFile,      // 41
    EPbsInitFromFile64,             // 42
    EPbsInitStreamingFromFile64,    // 43
    EPbsGetFile64,                  // 44
    EPbsSetPrimaryClient,            //45 set primary client
    EPbsServerOpEnd                 // End of operation     
    };

#endif // MPXPLAYBACKFRAMEWORKDEFS_H

// End of file
