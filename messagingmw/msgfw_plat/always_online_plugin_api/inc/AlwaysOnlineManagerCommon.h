/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnlineManagerCommon
*
*/


#ifndef __ALWAYSONLINEMANAGERCOMMON_H__
#define __ALWAYSONLINEMANAGERCOMMON_H__

#include <e32base.h>

// CONSTANTS
// For example email can quickly fill the space if we use only "official" 
// limits, so we need this. This should be set so that we get low 
// diskspace event before those annoying system "memory low" start popping up.
const TInt KAOSafetyMargin = 204800;//200KB
const TUid KCEComInterfaceUid = {0x101F85EC};
const TInt KAOMailboxAttributeLength = 500;
const TInt KAOFinalProgressLength = 100;
_LIT(KAlwaysOnlineManagerServerName,"!AlwaysOnlineManagerServer");

struct TEMNElement
    {
    //required
    //two possible formats:
    //1) "Mailat URI scheme": mailbox="mailat:user@wapforum.org"
    //2) Protocol specific URI scheme: 
    // mailbox="pop://userxyz;auth=3598302@pop.wapforum.org"
    TBuf<KAOMailboxAttributeLength> mailbox;

    //implied, EMN may or may not include this
    //format: YYYY-MM-DDThh:mm:ssZ
    //Note: 'T' and 'Z' appear literally in the string.
    //EMN does not allow use of time zones; the time MUST always be 
    //expressed in Co-ordinated Universal Time (UTC),
    
    TTime timestamp;
    };

// Enumerations

enum TAlwaysOnlineManagerPanic
	{
    EAOMServerStartFail,
    ENoPluginsFound,
    EPluginLoadFailed,
    EPluginStartFailed,
    EBadRequest,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ESvrStartServer,
    ECreateTrapCleanup,
    ENotImplemented,
    EBadSubsessionHandle,
    EBadCounterRemove,
    ETooManyPlugins
	};


enum TManagerServerCommands
    {
    EAOManagerPluginStart = 0,
    EAOManagerPluginStop,               //1
    EAOManagerStartedRoaming,           //2
    EAOManagerStoppedRoaming,           //3
    EAOManagerNWOpsAllowed,             //4
    EAOManagerNWOpsNotAllowed,          //5
    EAOManagerDiskSpaceAboveCritical,   //6
    EAOManagerDiskSpaceBelowCritical,   //7
    EAOManagerSuicideQuery,             //8
    EAOManagerAOSchdulerError = 199,    //199
    EAOManagerEmailCommandBase = 1000,
    EAOManagerMailboxAgentSuspend,  // 1001
    EAOManagerMailboxAgentResume,   // 1002
    EAOManagerMailboxAgentRemove,   // 1003
    EAOManagerEMNReceived,          // 1004
    EAOManagerMailboxAgentUpdateMailWhileConnected, // 1005
    EAOManagerRegisterObserver,  // 1006
    EAOManagerUnRegisterObserver, // 1007
    EAOManagerQueryState, // 1008
    EAOManagerStartSync, // 1009
    EAOManagerCancelAllAndDisconnect, // 1010
    EAOManagerCancelAllAndDoNotDisconnect, // 1011
    EAOManagerSuspend, // 1012
    EAOManagerContinue, // 1013
    EAOManagerDoNotDisconnect, // 1014
    EAOManagerSessionClosed
    };



enum TManagerStatusQueries
    {
    // deprecated, do not use, use EAOManagerStatusQueryNetwork insted
    EAOManagerStatusQueryRoaming = 0,

    // ETrue if phone is on offline mode
    EAOManagerStatusQueryOffline,

    // Current network status, see TConnMonNetworkRegistration in rcommon.h
    EAOManagerStatusQueryNetwork
    };

enum TIpsPluginObserverStates
    {
    EIpsAOPluginStatusSyncStarted = 99,
    EIpsAOPluginStatusSyncCompleted
    };
  

#endif
//EOF
