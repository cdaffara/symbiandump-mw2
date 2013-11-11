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
* Description:  Harvester Server
*
*/


#include <e32std.h>
#include <e32svr.h>

#include "mpxharvestercommon.h"
#include "mpxharvesterserver.h"
#include "mpxharvesterengine.h"
#include "mpxharvestersession.h"
#include "mpxharvesterserverdefs.h"

// CONSTANTS

// Server Security Policy
const TUint KMPXHarvesterServerRangeCount = 2;
const TInt KMPXHarvesterServerRanges[KMPXHarvesterServerRangeCount] = 
    {
    0,                  //range is [0-EHvsServerOpCount)
    EHvsServerOpCount   //range is [EHvsServerOpCount-KMaxTInt] 
    };
const TUint8 KMPXHarvesterSeverElementsIndex[KMPXHarvesterServerRangeCount] = 
    {
    0,                            
    CPolicyServer::ENotSupported
    };
    
const CPolicyServer::TPolicyElement KMPXHarvesterServerPolicyElements[] = 
    {
    {_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), 
                              CPolicyServer::EFailClient}
    };
    
const CPolicyServer::TPolicy KMPXHarvesterServerPolicy =
    {
    CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
    KMPXHarvesterServerRangeCount,                   
    KMPXHarvesterServerRanges,
    KMPXHarvesterSeverElementsIndex,
    KMPXHarvesterServerPolicyElements
    };


// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Start Harvester Server
// ----------------------------------------------------------------------------
//
LOCAL_C void StartServerL()
    {
    User::LeaveIfError(User::RenameThread(KMPXHarvesterServerName));
    CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
    CleanupStack::PushL(scheduler);    
    CActiveScheduler::Install(scheduler);
    CMPXHarvesterServer* server = CMPXHarvesterServer::NewL();
    CleanupStack::PushL(server);    
    RProcess::Rendezvous(KErrNone);
    CActiveScheduler::Start();    
    CleanupStack::PopAndDestroy(server);
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXHarvesterServer* CMPXHarvesterServer::NewL()
    {
    CMPXHarvesterServer *server = new(ELeave) CMPXHarvesterServer(
                                                EMPXHarvesterServerPriority, 
                                                KMPXHarvesterServerPolicy);
    CleanupStack::PushL(server);
    server->ConstructL();
    CleanupStack::Pop(server);
    return server;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXHarvesterServer::CMPXHarvesterServer(TInt aPriority, const TPolicy &aPolicy)
    : CPolicyServer(aPriority, aPolicy) 
    {}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXHarvesterServer::~CMPXHarvesterServer()
    {
    delete iEngine;
    }
    
// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXHarvesterServer::ConstructL()
    {
    StartL(KMPXHarvesterServerName);
    RProcess().SetPriority(::EPriorityBackground);
    iEngine = CMPXHarvesterEngine::NewL();    
    }
    
// ----------------------------------------------------------------------------
// Increments number of sessions this server holds
// ----------------------------------------------------------------------------
//
void CMPXHarvesterServer::AddClient()
    {
    iClients++;
    }
    
 
// ----------------------------------------------------------------------------
// Decrement the number of clients currently connected to the harvester server
// ----------------------------------------------------------------------------
//
void CMPXHarvesterServer::RemoveClient()
    { 
    iClients--; 
    ASSERT(iClients>=0); 
    }

// ----------------------------------------------------------------------------
// Create a new session
// ----------------------------------------------------------------------------
//
CSession2* CMPXHarvesterServer::NewSessionL(const TVersion& aVersion,
                                           const RMessage2& /*aMessage*/) const
    {
    TVersion v(KMPXHarvesterServerMajorVersionNumber,
               KMPXHarvesterServerMinorVersionNumber,
               KMPXHarvesterServerBuildVersionNumber);
    if (!User::QueryVersionSupported(v,aVersion))
        {
        User::Leave(KErrNotSupported);
        }
    return CMPXHarvesterSession::NewL(*(const_cast<CMPXHarvesterServer*>(this)),
                                        *iEngine);
    }

// ----------------------------------------------------------------------------
// Server exe entry
// ----------------------------------------------------------------------------
//
TInt E32Main()
    {
    __UHEAP_MARK;
    //
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt r(KErrNoMemory);
    if (cleanup)
        {
        TRAP(r,StartServerL());
        }
        
    delete cleanup;
    __UHEAP_MARKEND;
    return r;
    }

