/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 * This file defines common types and constants used by the STS server
 * and sessions.
 */
#ifndef STSCLIENTSERVERCOMMON_H_
#define STSCLIENTSERVERCOMMON_H_

// SYMBIAN INCLUDES
#include <e32std.h>
#include <systemtoneservice.h>

// CONSTANTS
_LIT( KStsServerFile, "StsServer" );
_LIT( KStsServerName, "!StsServer" );

const TInt KStsServerMajorVersion = 0;
const TInt KStsServerMinorVersion = 1;
const TInt KStsServerBuild = 0;

// This type enumerates the client/server messages for the STS server.
enum TStsServerCommandType
    {
    StsMsg_RegisterMsgQueue,
    StsMsg_StopAlarm,
    StsMsg_PlayTone,
    StsMsg_PlayAlarm,
    StsMsg_PlayToneAlarm,
    StsMsg_ENDMARKER
    };

const TStsServerCommandType KStsCmdLast =
        (TStsServerCommandType) ((int) StsMsg_ENDMARKER - 1);

enum TStsCallBackType
    {
    EStsShutdown, EStsPlayAlarmComplete
    };

struct TStsCallBack
    {
    TStsCallBackType callBackType;
    MStsPlayAlarmObserver* observer;
    unsigned int alarmContext;
    };

#endif // STSCLIENTSERVERCOMMON_H_
