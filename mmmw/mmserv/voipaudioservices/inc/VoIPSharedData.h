/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Declares constants, structures and enumerated list of
 *                commands shared between VoIP client and the server.
 *
 */

#ifndef VOIPSHAREDDATA_H
#define VOIPSHAREDDATA_H

// INCLUDES
#include <e32std.h>
#include <sounddevice.h>
#include <audio.h>
#include <voipjitterbufferintfc.h>
#include <voipaudiocommon.h>
#include <voipformatintfc.h>

#if defined(__SERIES60_31__)
// s60 3.1 mapping of FourCC values for supported codecs
const TUint32 KMccFourCCIdAMRNB = 0x524d4120; // (' ', 'A', 'M', 'R')
const TUint32 KMccFourCCIdG711 = 0x31313747;  // ('G', '7', '1', '1')
const TUint32 KMccFourCCIdG729 = 0x39323747;  // ('G', '7', '2', '9')
const TUint32 KMccFourCCIdILBC = 0x43424c49;  // ('i', 'L', 'B', 'C')
#else
#include <mmcccodecinformation.h>
#endif

#include "VoIPAudioClientServer.h"

// CONSTANTS
const TUint KQueueCreateWaitTime = 10; //10ms
const TUint KVoIPG711FrameLen10ms = 82; //  80 + 2 for header
const TUint KVoIPG711FrameLen20ms = 162; // 160 + 2 for header
const TInt KVoIPAMRNBFrameLen = 32;
const TInt KVoIPILBCFrameLen = 52; //  40 in 20ms; 52 in 30ms
const TInt KVoIPG729FrameLen = 24; //  10 octets + 2 for header
const TUint KVoIPPCM16FrameLen = 4096;

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

    ECmdDnLinkError,
    ECmdUpLinkError,
    ECmdDnLinkJBError,

    ECmdSetG711CodecMode,
    ECmdGetG711CodecMode,
    ECmdSetIlbcCodecMode,
    ECmdGetIlbcCodecMode,
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
    ECmdVoIPConfigJB,
    ECmdVoIPResetJB,
    ECmdVoIPJBDelayDown,
    ECmdVoIPJBDelayUp,

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
    ECmdFillBuffer,
    ECmdEmptyBuffer,
    ECmdDTMFOpenDnlinkComplete,
    ECmdDTMFOpenUplinkComplete,
    ECmdDTMFTonePlayFinished,
    ECmdRingToneOpenComplete,
    ECmdRingTonePlayComplete,
    ECmdDownlinkThreadClosed,
    ECmdUplinkThreadClosed
    };

// -----------------------------------------------------------------------------
//  Class Name:  TVoIPCodecSettings
//
//  Structure used for setting codec configuration.
// -----------------------------------------------------------------------------
class TVoIPCodecSettings
    {
public:
    enum TG711FrameRate
        {
        E10MS,
        E20MS
        };

public:
    CVoIPFormatIntfc::TG711CodecMode iG711CodecMode;
    CVoIPFormatIntfc::TILBCCodecMode iILBCCodecMode;
    TUint32 iFourCC;
    TG711FrameRate iG711FrameRate;
    TBool iVad;
    TBool iCng;
    TBool iPlc;
    TBool iFrameMode;
    TBool iFrameModeReqForEC;
    TUint iBitrate;
    RArray<TUint>* iArrBitrates;
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
    TInt iPriority;
    TInt iPreference;
    TUint iAudioDevice;
    TInt iMaxVolume;
    TInt iMaxGain;

    // Codec configuration settings
    TVoIPCodecSettings iCodecSettings;

    // Jitter Buffer configuration
    TVoIPJBConfig iJBConfig;

    // Command parameter for signalling between the threads via A/O
    TUserCommand iCmd;

    // Size of the data buffer and frame sequence number
    TInt iBufferSize;
    TUint iSequenceNum;

    // Any data sent between both threads
    TInt iInt;
    TUint iUint;
    TBool iBool;
    };

#endif  //VOIPSHAREDDATA_H

// End of file
