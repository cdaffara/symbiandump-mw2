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
 * Description:  Defines constants, structs and enumerated list of opcodes
 *                used for message passing between the client and the server.
 *
 */

#ifndef VOIPAUDIOCLIENTSERVER_H
#define VOIPAUDIOCLIENTSERVER_H

#include <e32base.h>
#include <f32file.h> //for RFile
#include <voipaudiocommon.h>

// Used for client-server communication
enum TVoIPAudioClientServerRequest
    {
    EVoIPSetDecoderPriority,
    EVoIPSetEncoderPriority,
    EVoIPGetSupportedDecoders,
    EVoIPGetSupportedEncoders,
    EVoIPGetSupportedDecodersCount,
    EVoIPGetSupportedEncodersCount,
    EVoIPSetDecoder,
    EVoIPSetEncoder,
    EVoIPOpenDownlink,
    EVoIPOpenUplink,
    EVoIPStartDownlink,
    EVoIPStartUplink,
    EVoIPStopDownlink,
    EVoIPStopUplink,
    EVoIPCloseDownlink,
    EVoIPCloseUplink,
    EVoIPGetMaxVolume,
    EVoIPGetVolume,
    EVoIPSetVolume,
    EVoIPGetMaxGain,
    EVoIPSetGain,
    EVoIPGetGain,
    EVoIPSetAudioDevice,
    EVoIPGetAudioDevice,
    EVoIPBufferFilled,
    EVoIPBufferEmptied,

    //added for CI support
    EVoIPSetIlbcCodecMode,
    EVoIPGetIlbcCodecMode,
    EVoIPSetG711CodecMode,
    EVoIPGetG711CodecMode,
    EVoIPGetSupportedBitratesCount,
    EVoIPGetSupportedBitrates,
    EVoIPSetBitrate,
    EVoIPGetBitrate,
    EVoIPFrameModeRqrdForEC,
    EVoIPSetFrameMode,
    EVoIPGetFrameMode,
    EVoIPConcealErrForNextBuf,
    EVoIPSetVad,
    EVoIPGetVad,
    EVoIPSetCng,
    EVoIPGetCng,
    EVoIPSetPlc,
    EVoIPGetPlc,
    EVoIPBadLsfNextBuffer,
    EVoIPOpenDTMFTonePlayer,
    EVoIPPlayDTMFTone,
    EVoIPStopDTMFTone,
    EVoIPCloseDTMFTonePlayer,
    EVoIPOpenRingTonePlayerFromFile,
    EVoIPOpenRingTonePlayerFromRFile,
    EVoIPOpenRingTonePlayerFromProfile,
    EVoIPOpenRingTonePlayerFromDes,
    EVoIPPlayRingTone,
    EVoIPPauseRingTone,
    EVoIPResumeRingTone,
    EVoIPStopRingTone,
    EVoIPCloseRingTonePlayer,
    EVoIPConfigJitterBuffer,
    EVoIPResetJitterBuffer,
    EVoIPJBDelayDown,
    EVoIPJBDelayUp,
    EVoIPSessionClose
    };

// Ring tone open methods
enum TVoIPToneTypeRequest
    {
    EUseProfiles,
    EFileName,
    EFileHandle,
    EDescriptor
    };

// CONSTANTS
_LIT(KDnLinkQueue, "VoIPDnLinkQueue");
_LIT(KUpLinkQueue, "VoIPUpLinkQueue");
_LIT(KDnLinkComQueue, "VoIPDnLinkComQueue"); //ITC to dnlink client API
_LIT(KUpLinkComQueue, "VoIPUpLinkComQueue"); //ITC to uplink client API
_LIT(KDnLinkThreadComQueue, "VoIPDnLinkThreadComQueue"); //ITC to dnlink thread
_LIT(KUpLinkThreadComQueue, "VoIPUpLinkThreadComQueue"); //ITC to uplink thread
_LIT(KRingToneComQueue, "VoIPRingToneComQueue");
_LIT(KDTMFToneComQueueUPL, "VoIPDTMFToneComQueueUPL");
_LIT(KDTMFToneComQueueDNL, "VoIPDTMFToneComQueueDNL");
_LIT(KChunkDNL, "VoIP_DNL");
_LIT(KChunkUPL, "VoIP_UPL");

// server name and UID
_LIT(KVoIPServerName,"VoIPAudioServer");
const TUid KVoIPServerUid3 = {0x10207B9E};

// VoIP Server version
const TUint KVoIPServMajorVersionNumber = 1;
const TUint KVoIPServMinorVersionNumber = 0;
const TUint KVoIPServBuildVersionNumber = 0;

const TUint KVoIPMsgQSlots = 1;
const TUint KVoIPMsgComQSlots = 4;

const TUint KTonesBufferSize = KMaxFileName;

/**
 * TVoIPMsgBuf
 * Used to pass async requests from the voip client to the server and
 * callback messages from the server back to the client via MsgQueues.
 *
 * Note: RMsgQueue will raise KERN-EXEC 48 if sizeof(TVoIPMsgBuf):
 *       1. is not a multiple of 4 bytes
 *       2. is greater than KMaxLength (256 bytes)
 */
struct TVoIPMsgBuf //max 240
    {
    TInt iRequest;
    TInt iStatus;
    TBool iBool;
    TInt iInt; //bufferSize
    TInt64 iInt64; //time stamp
    TUint iUint;
    TUint32 iUint32;
    };

typedef TPckgBuf<TVoIPMsgBuf> TVoIPMsgBufPckg;

/**
 * TTonesMsgBuf
 * Used to pass tone data from the voip client to the server.
 * Callback messages from the server back to the client go through
 * MsgQueue using TVoIPMsgBuf.
 */
struct TTonesMsgBuf
    {
    TInt iRequest;
    TInt iStatus;
    TInt64 iDuration;
    RFile iFileHandle;
    TBuf<KTonesBufferSize> iFileName;
    };

typedef TPckgBuf<TTonesMsgBuf> TTonesMsgBufPckg;

#endif //VOIPAUDIOCLIENTSERVER_H

// End of file
