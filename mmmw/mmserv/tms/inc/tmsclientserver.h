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

#ifndef TMS_CLIENTSERVER_H
#define TMS_CLIENTSERVER_H

#include <e32base.h>
#include <f32file.h> //for RFile
#include <glib.h>

namespace TMS {

// Used for client-server communication
enum TMSClientServerRequest
    {
    ETMSCallSessionHandle = 0,
    ETMSGetSupportedDecodersCount,
    ETMSGetSupportedEncodersCount,
    ETMSGetSupportedDecoders,
    ETMSGetSupportedEncoders,
    ETMSSetOutput,
    ETMSGetOutput,
    ETMSGetPreviousOutput,
    ETMSGetAvailableOutputs,
    ETMSStartRoutingNotifier,
    ETMSCancelRoutingNotifier,
    ETMSSetGlobalVol,
    ETMSGetGlobalVol,
    ETMSGetMaxGlobalVol,
    ETMSSetGlobalGain,
    ETMSGetGlobalGain,
    ETMSGetMaxGlobalGain,
    ETMSStartGlobalEffectNotifier,
    ETMSCancelGlobalEffectNotifier,
    ETMSSetMsgQueueHandle,
    ETMSRingToneInitDefault,
    ETMSRingToneInitFile,
    ETMSRingToneInitSequence,
    ETMSRingToneInitBeepOnce,
    ETMSRingToneInitSilent,
    ETMSRingToneInitUnsecureVoIP,
    ETMSRingToneDeinit,
    ETMSRingTonePlay,
    ETMSRingTonePause,
    ETMSRingToneStop,
    ETMSRingToneMute,
    ETMSInitDTMF,
    ETMSStartDTMF,
    ETMSStopDTMF,
    ETMSContinueDTMF,
    ETMSStartInbandTone,
    ETMSStopInbandTone,
    ETMSTermSrv
    };

// Ring tone open methods
enum TMSToneTypeRequest
    {
    EUseProfiles,
    EFileName,
    EFileHandle,
    EDescriptor
    };

// TMSServer Global Effects and Routing Publish & Subscribe property sub-key
enum TMSGlobalEffectsPsKey
    {
    EGlobalEffectsPsVolume = 10,
    EGlobalEffectsPsGain,
    ERoutingPs,
    EDtmfPs,
    ESyncVolume
    };

// TMSServer message queue notifier type keys
enum TMSMsgQueueNotifierType
    {
    EMsgQueueRingtoneType,
    EMsgQueueDTMFType,
    EMsgQueueGlobalRoutingType,
    EMsgQueueGlobalVolumeType,
    EMsgQueueGlobalGainType,
    EMsgQueueInbandToneType
    };

// CONSTANTS
// TMSServer Effects and Routing Publish & Subscribe property category
const TUid KTMSPropertyCategory = {0x10207C7C};

// server name and UID
_LIT(KTMSServerFile, "tmsserver");
_LIT(KTMSServerName, "!TmsServer");
const TUid KTMSServerUid3 = {0x10207C7C};
const TUid KUidTMSCallServerUid3 = {0x10207C78};

// TMS Server version
const guint KTMSServMajorVersionNumber = 1;
const guint KTMSServMinorVersionNumber = 0;
const guint KTMSServBuildVersionNumber = 0;

const guint KTMSMsgQSlots = 8;
const guint KTonesBufferSize = KMaxFileName;

/**
 * TmsMsgBuf
 * Used to pass async requests from the TMS client to the server and
 * callback messages from the server back to the client via MsgQueues.
 *
 * Note: RMsgQueue will raise KERN-EXEC 48 if sizeof(TmsMsgBuf):
 *       1. is not a multiple of 4 bytes
 *       2. is greater than KMaxLength (256 bytes)
 */
struct TmsMsgBuf //max 240
    {
    gint iRequest;
    gint iStatus;
    gboolean iBool;
    gint iInt;
    gint iInt2;
    gint64 iInt64;
    guint iUint;
    guint32 iUint32;
    };
typedef TPckgBuf<TmsMsgBuf> TmsMsgBufPckg;

/*
 * TRoutingMsgBuf
 */
struct TRoutingMsgBuf
    {
    gint iOutput;
    gint iEvent;
    };
typedef TPckgBuf<TRoutingMsgBuf> TRoutingMsgBufPckg;

} //namespace TMS

#endif //TMS_CLIENTSERVER_H

// End of file
