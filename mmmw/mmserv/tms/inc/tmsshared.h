/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMS_SHARED_H
#define TMS_SHARED_H

// INCLUDES
#include <glib.h>
#include <e32std.h>
#include "tmsclientserver.h"
#include "tmsgstreamer.h"

namespace TMS {

// CONSTANTS
const guint KQueueCreateWaitTime = 10; //10ms

const guint KTMSG711FrameLen10ms = 82;  //10ms ->  80 + 2 for header
const guint KTMSG711FrameLen20ms = 162; //20ms -> 160 + 2 for header

const gint KTMSAMRNBFrameLen   = 14; //frame-mode OFF
const gint KTMSAMRNBFrameLenFM = 32; //frame-mode ON

const gint KTMSILBCFrameLen20ms = 40; //20ms
const gint KTMSILBCFrameLen30ms = 52; //30ms

const gint KTMSG729FrameLen   = 24; //10 octets + 2 for header
const guint KTMSPCM16FrameLen = 4096;

// ENUMERATIONS

// ITC communication commands between the threads
enum TUserCommand
    {
    ECmdStartDownlink = 0,
    ECmdStartUplink,
    ECmdInitDownlink,
    ECmdInitUplink,
    ECmdStopDownlink,
    ECmdStopUplink,
    ECmdBufferFilled,
    ECmdBufferEmptied,
    ECmdGetVolume,
    ECmdSetVolume,
    ECmdGetGain,
    ECmdSetGain,
    ECmdSetAudioDevice,
    ECmdGetAudioDevice,
    ECmdTerminateThread,
    ECmdGetDataXferChunkHndl,

    ECmdDnLinkError,
    ECmdUpLinkError,

    ECmdSetG711CodecMode,
    ECmdGetG711CodecMode,
    ECmdSetIlbcCodecMode,
    ECmdGetIlbcCodecMode,
    ECmdSetG711CodecModeUPL,
    ECmdGetG711CodecModeUPL,
    ECmdSetIlbcCodecModeUPL,
    ECmdGetIlbcCodecModeUPL,
    ECmdGetSupportedBitrates,
    ECmdSetBitrate,
    ECmdGetBitrate,
    ECmdSetFrameMode,
    ECmdGetFrameMode,
    ECmdFrameModeRqrdForEC,
    ECmdConcealErrForNextBuf,
    ECmdSetVad,
    ECmdGetVad,
    ECmdSetCng,
    ECmdGetCng,
    ECmdSetPlc,
    ECmdGetPlc,
    ECmdBadLsfNextBuffer,

    // Event returned from the work thread upon async request complete
    ECmdGetVolumeComplete,
    ECmdGetGainComplete,
    ECmdGetAudioDeviceComplete,
    ECmdGetG711CodecModeComplete,
    ECmdGetIlbcCodecModeComplete,
    ECmdGetFrameModeComplete,
    ECmdGetFrameModeReqForECComplete,
    ECmdGetVadComplete,
    ECmdGetCngComplete,
    ECmdGetPlcComplete,
    ECmdGetSupportedBitratesComplete,
    ECmdGetBitrateComplete,

    // Server side events sent to the client
    ECmdDownlinkInitComplete,
    ECmdUplinkInitComplete,
    ECmdDownlinkDeInitComplete,
    ECmdUplinkDeInitComplete,
    ECmdDownlinkStarted,
    ECmdUplinkStarted,
    ECmdDownlinkPaused,
    ECmdUplinkPaused,
    ECmdFillBuffer,
    ECmdEmptyBuffer,
    ECmdDTMFToneDnlPlayStarted,
    ECmdDTMFToneUplPlayStarted,
    ECmdDTMFTonePlayFinished,
    ECmdRingToneOpenComplete,
    ECmdRingTonePlayComplete,
    ECmdRingToneDeinitComplete,
    ECmdDownlinkClosed,
    ECmdUplinkClosed,
    ECmdGlobalRoutingChange,
    ECmdGlobalEffectChange,
    ECmdInbandToneStarted,
    ECmdInbandToneStopped
    };

// -----------------------------------------------------------------------------
//  Class Name:  TMSCodecSettings
//
//  Structure used for setting codec configuration.
// -----------------------------------------------------------------------------
class TMSCodecSettings
    {
public:
    enum TG711FrameRate
        {
        E10MS,
        E20MS
        };

public:
    gint iG711CodecMode;
    gint iILBCCodecMode;
    guint32 iFourCC;
    TG711FrameRate iG711FrameRate;
    gboolean iVad;
    gboolean iCng;
    gboolean iPlc;
    gboolean iFrameMode;
    gboolean iFrameModeReqForEC;
    guint iBitrate;
    RArray<guint> iArrBitrates;
    };

// -----------------------------------------------------------------------------
//  Class Name:  TSharedData
//
//  Structure used for ITC communication and data transfer.
// -----------------------------------------------------------------------------
class TSharedData
    {
public:
    // Main thread's ID
    TThreadId iMainThreadID;

    // For thread end signalling
    RMutex iMutex;

    // Request status pointer for signalling thread via A/O
    TRequestStatus* iThreadStatus;
    TRequestStatus* iMnThreadStatus;

    // Audio configuration settings
    guint iAudioDevice;
    gint iMaxVolume;
    gint iMaxGain;

    // Codec configuration settings
    TMSCodecSettings iCodecSettings;

    // Command parameter for signalling between the threads via A/O
    TUserCommand iCmd;

    // Size of the data buffer and frame sequence number
    gint iBufferSize;
    guint iSequenceNum;

    // Any data sent between both threads
    gint iInt;
    guint iUint;
    gboolean iBool;
    guint32 iKeyValue;
    };

} //namespace TMS

#endif  // TMS_SHARED_H

// End of file
