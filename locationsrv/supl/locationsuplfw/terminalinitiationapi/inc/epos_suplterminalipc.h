/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Client-Server Request codes
*
*/



#ifndef __EPOS_SUPLTERMINALIPC_H__
#define __EPOS_SUPLTERMINALIPC_H__

// 0x00000000 to 0x00000FFF not used
#define ESuplIpcOffset           0x1000

// 0x00001000 to 0x00001FFF reserved for RSuplServer requests
#define ESuplServerIpcOffset        (ESuplIpcOffset)

// 0x00002000 to 0x00002FFF reserved for RSuplTerminalIPCSubSession requests
#define ESuplSubsessnIpcOffset      (ESuplServerIpcOffset + 0x1000)

// 0x40000000 to 0x7FFFFFFF reserved for 3rd party extensions to the SUPL API.
// All extensions implemented by 3rd parties should be implemented in this range.
#define ESuplCustomIpcOffset      0x40000000

// This position in the IPC message is used by the server to write fire info objects
const TInt KParamTriggerFireInfo = 2;

enum TSuplIpcId
    {
    //
    // Request IDs
    ESuplTerminalSubssnOpen                     = ESuplSubsessnIpcOffset,
    ESuplTerminalSubssnOpenTriggerSession,
    ESuplTerminalSubssnClose,
    ESuplTerminalSubssnSyncRunsession,
    ESuplTerminalSubssnAsyncRunsession,
    ESuplTerminalSubssnSyncRunsessionWithQop,
    ESuplTerminalSubssnAsyncRunsessionWithQop,
    ESuplTerminalSubssnGetPosition,
    ESuplTerminalSubssnCancelRunsession,

    ESuplTerminalSubssnSyncRunsessionWithServernameQop,
    ESuplTerminalSubssnASyncRunsessionWithServernameQop,
    ESuplTerminalSubssnSyncRunsessionServernameWithoutQop,
    ESuplTerminalSubssnASyncRunsessionServernameWithoutQop,
    
    ESuplTerminalSubssnGetServerAddress,
    
	ESuplTerminalSubssnStartPeriodicTrigger,
	ESuplTerminalSubssnStartPeriodicTriggerWithServer,	
	ESuplTerminalSubssnNotifyTriggerFired,
	ESuplTerminalSubssnStopTriggerSession,
	ESuplTerminalSubssnLocationConversion,
	ESuplTerminalSubssnCancelLocationConversion,
    ESuplSubssnCustomCodesBegin         = ESuplCustomIpcOffset,
    };

#endif // __EPOS_SUPLTERMINALIPC_H__
