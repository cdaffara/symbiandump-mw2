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
* Description:  Definitions for command type for playback framework
*
*/



#ifndef MPXPLAYBACKCOMMANDDEFS_H
#define MPXPLAYBACKCOMMANDDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/******************************************************
 *  Commands supported for KMPXCommandIdPlaybackGeneral
 ******************************************************/
/**
* General Commands for playback framework
*/
const TInt KMPXCommandIdPlaybackGeneral = 0x10282971;

///////////////////////////////////////////////////////////////////////////////
// Attributes supported by KMPXCommandIdPlaybackGeneral
// Attributes besides KMPXCommandContentIdGeneral
// The value of attribute KMPXCommandGeneralId is KMPXCommandIdPlaybackGeneral
///////////////////////////////////////////////////////////////////////////////

/**
 *  General command type
 *  TInt value. value of TMPXPlaybackCommand
 */
const TMPXAttributeData KMPXCommandPlaybackGeneralType =
                                    {KMPXCommandIdPlaybackGeneral, 0x01};

/** Optional
 *  Parameter data for the general command
 *  TInt value
 */
const TMPXAttributeData KMPXCommandPlaybackGeneralData =
                                    {KMPXCommandIdPlaybackGeneral, 0x02};

/** Optional
 *  Tells whether or not to use command buffering for this command
 *  TBool value
 */
const TMPXAttributeData KMPXCommandPlaybackGeneralNoBuffer =
                                    {KMPXCommandIdPlaybackGeneral, 0x04};

/** Optional
 *  The process id of the client which sends the command
 *  TProcessId value
 */
const TMPXAttributeData KMPXCommandPlaybackGeneralClientPid =
                                    {KMPXCommandIdPlaybackGeneral, 0x08};

/**
 * Playback general command
 */
enum TMPXPlaybackCommand
    {
    EPbCmdPlay, // 0 Play which ever track is currently stopped, paused etc.
    EPbCmdPlayWithFadeIn, // 1 uses KPbFadeInDuration
    EPbCmdPause,    // 2
    EPbCmdStop,     // 3
    EPbCmdNext,     // 4 No effect if no next track
    EPbCmdPrevious, // 5 No effect if no previous track;
                    //   if pos>KPbReplayThreshold, replays current track
    EPbCmdReplay,   // 6 Restart current track at beginning
    EPbCmdStartSeekForward,  // 7
    EPbCmdStartSeekBackward, // 8
    EPbCmdStopSeeking,       // 9
    EPbCmdIncreaseVolume,    // 10
    EPbCmdDecreaseVolume,    // 11
    EPbCmdMuteVolume,        // 12
    EPbCmdUnMuteVolume,      // 13
    EPbCmdClose,             // 14
    EPbApplyEffect,          // 15 Apply an effect to the current playback,
                             //    aData used to identify effect
    EPbCmdPreservePosition,  // 16 Preserve playback position
    EPbCmdPreserveState,     // 17 Preserve playback state
    EPbCmdCloseItem,         // 18 Request to close the file handle
    EPbCmdPlayPause,         // 19 Play/Pause toggle command
    EPbCmdCancelInit,         // 20
    EPbCmdResetPreserveState, // 21
    EPbCmdUnloadNonActivePlugin, // 22 KMPXCommandPlaybackGeneralData is TUid.iUid
    EPbCmdClearKeyBuffer,        // 23 Clear all media key commands
    EPbCmdDisableEffect,
    EPbCmdSetVolume,             // 25 Set volume to the value specified by aData
    EPbCmdSetAutoResume,         // 26 Set autoresume value (true/false)
    EPbCmdSyncMsgComplete,      //27 Synchronous message is complete  
    EPbCmdEnd                    // Mark for the end of command
    };

/******************************************************
 *  Commands supported for KMPXCommandIdPlaybackPD
 ******************************************************/
/**
* Progressive Download Commands for playback framework
*/
const TInt KMPXCommandIdPlaybackPD = 0x10282972;

////////////////////////////////////////////////////////////////////////////
//  Attributes supported by KMPXCommandIdPlaybackPD
//  Attributes besides KMPXCommandContentIdGeneral and
//  KMPXCommandIdPlaybackGeneral
//  The value of attribute KMPXCommandGeneralId is KMPXCommandIdPlaybackPD
//  The value of attribute KMPXCommandPlaybackGeneralType is the value of
//  TMPXPlaybackPdCommand
////////////////////////////////////////////////////////////////////////////

/** Optional
 *  Parameter for PD transaction ID
 *  TUInt value
 */
const TMPXAttributeData KMPXCommandPlaybackPDTransactionID = {
                        KMPXCommandIdPlaybackPD, 0x01};

/** Optional, TInt
 *  Parameter for PD Event ID
 *  TInt value
 */
const TMPXAttributeData KMPXCommandPlaybackPDEvent = {
                        KMPXCommandIdPlaybackPD, 0x02};

/** Optional, TInt
 *  Parameter for PD State
 *  TInt value, value of TMPXPlaybackPdDownloadState
 */
const TMPXAttributeData KMPXCommandPlaybackPDState = {
                        KMPXCommandIdPlaybackPD, 0x04};

/** Optional, TInt
 *  Parameter for PD Downloaded Bytes
 *  TInt value
 */
const TMPXAttributeData KMPXCommandPlaybackPDDownloadedBytes = {
                        KMPXCommandIdPlaybackPD, 0x08};

/** Optional, TInt
 *  Parameter for PD Total Bytes
 *  TInt value
 */
const TMPXAttributeData KMPXCommandPlaybackPDTotalBytes = {
                        KMPXCommandIdPlaybackPD, 0x10};

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
/** Optional, TInt64
 *  Parameter for PD Downloaded Bytes
 *  TInt64 value
 */
const TMPXAttributeData KMPXCommandPlaybackPDDownloadedBytes64 = {
                        KMPXCommandIdPlaybackPD, 0x20};

/** Optional, TInt64
 *  Parameter for PD Total Bytes
 *  TInt64 value
 */
const TMPXAttributeData KMPXCommandPlaybackPDTotalBytes64 = {
                        KMPXCommandIdPlaybackPD, 0x40};
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

/**
 * Progress download command
 */
enum TMPXPlaybackPdCommand
    {
    EPbCmdStartPd,
    EPbCmdFinishPd,
    EPbCmdHandlePdEvent,
    EPbCmdGetPdStatus,
    EPbCmdPausePd,
    EPbCmdResumePd,
    EPbCmdCancelPd
    };

enum TMPXPlaybackPdDownloadState  // Progressive download state
    {
    EPbDlStateNotDownloading,     // PD not in progress
    EPbDlStateBuffering,          // Download in progress, not downloaded enough to start playback
    EPbDlStateDownloading,        // Download in progress, playback can start/has started
    EPbDlStateDownloadPaused,     // Download paused
    EPbDlStateDownloadCompleted,  // Download completed
    EPbDlStateDownloadError,      // Download error
    EPbDlStateDownloadCanceled    // Download canceled
    };

#endif // MPXPLAYBACKCOMMANDDEFS_H

// End of file
