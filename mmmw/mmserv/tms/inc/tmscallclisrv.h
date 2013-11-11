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

#ifndef TMSCALLCLISRV_H
#define TMSCALLCLISRV_H

#include <tms.h>

namespace TMS {

// Used for client-server communication
enum TMSCallClientServerRequest
    {
    TMS_CREATE_CALL = 1,
    TMS_CREATE_STREAM,
    TMS_INIT_STREAM,
    TMS_START_STREAM,
    TMS_PAUSE_STREAM,
    TMS_STOP_STREAM,
    TMS_DEINIT_STREAM,
    TMS_DELETE_STREAM,
    TMS_DELETE_CALL,
    TMS_DATA_XFER_BUFFER_EMPTIED,
    TMS_DATA_XFER_BUFFER_FILLED,
    TMS_DATA_XFER_BUFFER_GET_HNDL,
    TMS_EFFECT_VOLUME_GETMAX,
    TMS_EFFECT_VOLUME_SET,
    TMS_EFFECT_VOLUME_GET,
    TMS_EFFECT_GAIN_GETMAX,
    TMS_EFFECT_GAIN_SET,
    TMS_EFFECT_GAIN_GET,
    TMS_EFFECT_GLOBAL_VOL_SET,
    TMS_EFFECT_GLOBAL_VOL_GET,
    TMS_EFFECT_GLOBAL_VOL_GETMAX,
    TMS_EFFECT_GLOBAL_GAIN_SET,
    TMS_EFFECT_GLOBAL_GAIN_GET,
    TMS_EFFECT_GLOBAL_GAIN_GETMAX,
    TMS_ROUTING_OUTPUT_SET,
    TMS_ROUTING_OUTPUT_GET,
    TMS_ROUTING_PREVIOUSOUTPUT_GET,
    TMS_ROUTING_AVAILABLE_OUTPUT_GET,
    TMS_GET_ACTIVE_CALL_PARAMS,
    TMS_TERM_CALL_SRV,

    //For TMS Format
    TMS_FORMAT_SETCODECMODE,
    TMS_FORMAT_GETCODECMODE,
    TMS_FORMAT_GETSUPPORTEDBITRATESCOUNT,
    TMS_FORMAT_GETSUPPORTEDBITRATES,
    TMS_FORMAT_SETBITRATE,
    TMS_FORMAT_GETBITRATE,
    TMS_FORMAT_FRAMEMODERQRDFOREC,
    TMS_FORMAT_SETFRAMEMODE,
    TMS_FORMAT_GETFRAMEMODE,
    TMS_FORMAT_SETVAD,
    TMS_FORMAT_GETVAD,
    TMS_FORMAT_SETCNG,
    TMS_FORMAT_GETCNG,
    TMS_FORMAT_SETPLC,
    TMS_FORMAT_GETPLC
    };

struct TMSCliSrvStreamCreateDataStruct
    {
    TMSCallType CallType;
    TMSStreamType StreamType;
    };
typedef TPckgBuf<TMSCliSrvStreamCreateDataStruct>
        TMSCliSrvStreamCreateDataStructBufPckg;

struct TMSCliSrvStreamInitDataStruct
    {
    TMSCallType CallType;
    TMSStreamType StreamType;
    gint StreamId;
    TMSFormatType FormatType;
    gint RetryTime;
    };
typedef TPckgBuf<TMSCliSrvStreamInitDataStruct>
        TMSCliSrvStreamInitDataStructBufPckg;

struct TMSCliSrvStreamOpDataStruct
    {
    TMSCallType CallType;
    TMSStreamType StreamType;
    gint StreamId;
    gint RetryTime;
    };
typedef TPckgBuf<TMSCliSrvStreamOpDataStruct>
        TMSCliSrvStreamOpDataStructBufPckg;

struct TMSCliSrvDataXferDataStruct
    {
    TMSCallType CallType;
    TMSStreamType StreamType;
    gint StreamId;
    guint DataSize;
    };
typedef TPckgBuf<TMSCliSrvDataXferDataStruct>
        TMSCliSrvDataXferDataStructBufPckg;

struct TMSCliSrvDataXferChunkHndlDataStruct
    {
    TMSCallType CallType;
    TMSStreamType StreamType;
    gint StreamId;
    guint32 Key;
    };
typedef TPckgBuf<TMSCliSrvDataXferChunkHndlDataStruct>
        TMSCliSrvDataXferChunkHndlDataStructBufPckg;

struct TmsCallMsgBuf
    {
    gint iRequest;
    gint iStatus;
    gboolean iBool;
    gint iInt;
    };
typedef TPckgBuf<TmsCallMsgBuf> TmsCallMsgBufPckg;

} //namespace TMS

#endif // TMSCALLCLISRV_H

// End of file
