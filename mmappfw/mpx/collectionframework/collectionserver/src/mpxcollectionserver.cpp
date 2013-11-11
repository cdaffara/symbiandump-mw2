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
* Description:  Collection server
*
*/


#include <e32std.h>
#include <e32svr.h>
#include "mpxcollectionframeworkdefs.h"
#include "mpxcollectionserverdefs.h"
#include "mpxcollectionserversession.h"
#include "mpxcollectionengine.h"
#include "mpxcollectionserver.h"


// CONSTANTS

// Server Security Policy
const TUint KMPXCollectionServerRangeCount = 2;
const TInt KMPXCollectionServerRanges[KMPXCollectionServerRangeCount] = 
    {
    0,                  //range is [0-EMcsServerOpEnd)
    EMcsServerOpEnd,    //range is [EMcsServerOpEnd-KMaxTInt] 
    };
const TUint8 KMPXCollectionSeverElementsIndex[KMPXCollectionServerRangeCount] = 
    {
    0,                            //applies to range [0-EMcsServerOpEnd)
    CPolicyServer::ENotSupported, //applies to range [EMcsServerOpEnd-KMaxTInt]
    };
    
const CPolicyServer::TPolicyElement KMPXCollectionServerPolicyElements[] = 
    {
    {_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), 
                              CPolicyServer::EFailClient},
    };
    
const CPolicyServer::TPolicy KMPXCollectionServerPolicy =
    {
    CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
    KMPXCollectionServerRangeCount,                   
    KMPXCollectionServerRanges,
    KMPXCollectionSeverElementsIndex,
    KMPXCollectionServerPolicyElements,
    };


// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Start collection server
// ----------------------------------------------------------------------------
//
LOCAL_C void StartServerL()
    {
    User::LeaveIfError(User::RenameThread(KMPXCollectionServerName));
    CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
    CleanupStack::PushL(scheduler);    
    CActiveScheduler::Install(scheduler);
    CMPXCollectionServer* server = CMPXCollectionServer::NewL();
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
CMPXCollectionServer* CMPXCollectionServer::NewL()
    {
    CMPXCollectionServer *pS = new(ELeave) CMPXCollectionServer(
                                                CActive::EPriorityStandard,
                                                KMPXCollectionServerPolicy);
    CleanupStack::PushL(pS);
    pS->ConstructL();
    CleanupStack::Pop(pS);
    return pS;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXCollectionServer::CMPXCollectionServer(TInt aPriority, const TPolicy &aPolicy)
    : CPolicyServer(aPriority, aPolicy) 
    {}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXCollectionServer::~CMPXCollectionServer()
    {
    delete iEngine;
    }
    
// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXCollectionServer::ConstructL()
    {
    StartL(KMPXCollectionServerName);    	
    iEngine = CMPXCollectionEngine::NewL();
    }
 
// ----------------------------------------------------------------------------
// Find the collection that has client aName, and remove it from that collection; 
// if that collection has no clients, then delete that collection. If the server 
// has no more client sessions, then stop the server.
// ----------------------------------------------------------------------------
//
void CMPXCollectionServer::RemoveClient()
    { 
    iClients--; 
    MPX_ASSERT(iClients>=0); 
    if (iClients==0)
        {
        CActiveScheduler::Stop(); 
        }
    }

// ----------------------------------------------------------------------------
// Create a new session
// ----------------------------------------------------------------------------
//
CSession2* CMPXCollectionServer::NewSessionL(const TVersion& aVersion,
                                             const RMessage2& /*aMessage*/) const
    {
    TVersion v(KMPXCollectionServerMajorVersionNumber,
               KMPXCollectionServerMinorVersionNumber,
               KMPXCollectionServerBuildVersionNumber);
    if (!User::QueryVersionSupported(v,aVersion))
        User::Leave(KErrNotSupported);
    CSession2* session = CMPXCollectionSession::NewL(*iEngine);
    const_cast<CMPXCollectionServer*>(this)->iClients++;;
    return session;
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
    TInt r=KErrNoMemory;
    if (cleanup)
        {
        TRAP(r,StartServerL());
        }
        
    delete cleanup;
    __UHEAP_MARKEND;
    return r;
    }

// End of file
