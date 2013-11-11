/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: IPC message constants
*
*
*/


#ifndef POSLMSERVERCOMMON_H
#define POSLMSERVERCOMMON_H

#include <epos_poslmserverdefs.h>

const TUint KPosLmServerEventArg =      0;
const TUint KPosLmServerDbManEventArg = 0;
const TUint KPosLmServerUriArg =        0;
const TUint KPosLmServerUriLengthArg =  1;
const TUint KPosLmServerDbReplaceArg =  1;
const TUint KPosLmServerProtocolArg  =  0;
const TUint KPosLmServerSourceUriArg =  0;
const TUint KPosLmServerDbInfoArg    =  0;
const TUint KPosLmServerTargetUriArg =  1;

const TUint KPosLmServerDbManUriArg  =  1;
const TUint KPosLmServerDriveArg     =  1;
const TUint KPosLmServerBufferArg    =  1;
const TUint KPosLmServerDbEventArg   =  1;

const TUint KPosLmServerAllocLengthArg =2;
const TUint KPosLmServerDbSettingsArg  =2;

const TUint KPosLmServerStatusArg   =   0;
const TUint KPosLmServerProgressArg =   1;
const TUint KPosLmServerInitFlagArg =   2;

const TUint KPosLmServerDbLockSetOnArg =    0;
const TUint KPosLmServerDbLockSetWriteArg = 1;

const TUint KPosLmServerIdArray         = 1;
const TUint KPosLmServerIdArrayParam    = 0;

/**
* Possible messages to be sent between client and server.
*/
enum TPosLmServerMessage
    {
    // NOTE!
    // If you add a new message here, it must also be added
    // to the policy handling in the CPosLmServer class.

    EPosLmServerInitializeSync = 0,
    EPosLmServerInitializeAsync,
    EPosLmServerInitializeCancel,
    EPosLmServerLangSwitchSync,
    EPosLmServerLangSwitchAsync,
    EPosLmServerLangSwitchCancel,
    EPosLmServerRecoverSync,
    EPosLmServerRecoverAsync,
    EPosLmServerRecoverCancel,
    EPosLmServerListenForLmEvent,
    EPosLmServerCancelListenForLmEvent,
    EPosLmServerListenForDbEvent,
    EPosLmServerCancelListenForDbEvent,
    EPosLmServerUriFromLastEvent,
    EPosLmServerCreateDefaultDb,
    EPosLmServerListDatabaseUris,
    EPosLmServerRemainingDatabaseUris,
    EPosLmServerListDatabaseInfo,
    EPosLmServerRemainingDatabaseInfo,
    EPosLmServerReadDatabaseSettings,
    // The IPC messages above requires capability ReadUserData.

    EPosLmServerRegisterSession,
    EPosLmServerRegisterLocalAccessSubsession,
    EPosLmServerRegisterEventListenerSubsession,
    EPosLmServerRegisterLocalDbManSubsession,
    EPosLmServerUnregisterSubsession,
    // The IPC messages above requires NO capabilities.

    EPosLmServerDbLock,
    // The IPC message above requires a custom check.
    //  Read lock requires:     ReadUserData
    //  Write lock requires:    ReadUserData, WriteUserData

    EPosLmServerCreateDatabase,
    EPosLmServerDeleteDatabase,
    EPosLmServerCopyDatabase,
    EPosLmServerRegisterDatabase,
    EPosLmServerUnregisterDatabase,
    EPosLmServerUnregisterAllDatabases,
    EPosLmServerModifyDatabaseSettings,
    EPosLmServerReportLmEvent,
    EPosLmServerReportDbEvent,
    EPosLmServerFreeResources,
    // The IPC messages above requires capability ReadUserData and WriteUserData

    // Extend IPC from here
    
    // These IPC messages require NO capabilities.
    EPosLmServerFreeRange2 = 100,
    EPosLmServerOpenNameIndex,
    EPosLmServerCloseNameIndex,
    
    // These IPC messages require Custom Check.
    EPosLmServerCustomCheckRange2 = 200,

    // These IPC messages require capability ReadUserData.
    EPosLmServerReadRange2 = 300,
    EPosLmServerNameIndexStatus,
    EPosLmServerReadNameIndex,
    EPosLmServerReadSortedIds,

    // These IPC messages require ReadUserData and WriteUserData.
    EPosLmServerWriteRange2 = 400,
    EPosLmServerIndexTransaction, // only used with attempt to modify index
    EPosLmServerUpdateNameIndex,
    
    EPosLmServerLast
    };

struct TPosLmServerReadArrayParam
    {
    TInt iFirst;
    TInt iCount;
    TInt iRemainder;
    TInt iActualCount;      // out
    TInt iMinBufferNeeded;  // out
    };
    
enum TPosLmServerUpdateLmNameIndexOp
    {
    EPosLmServerAddLandmarks = 1,
    EPosLmServerRemoveLandmarks,
    EPosLmServerUpdateLandmarks,
    };
    
enum TPosLmServerTransactionOp 
    {
    EPosLmServerTransactionUnknown,
    EPosLmServerTransactionStart,
    EPosLmServerTransactionCommit,
    EPosLmServerTransactionRollback,
    };

#endif      // POSLMSERVERCOMMON_H


