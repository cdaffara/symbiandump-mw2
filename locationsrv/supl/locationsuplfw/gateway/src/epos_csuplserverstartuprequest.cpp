/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Asynchronous SUPL session request
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <flogger.h>

#include "epos_suplterminalerrors.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplsessionbase.h"
#include "epos_csuplglobal.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplserverpanic.h"
#include "epos_msuplsessionobserver.h"
#include "epos_csuplserverstartuprequest.h"
#include "epos_mconnectobserver.h"
#include "epos_sessionmanagerlogging.h" 



// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::EPos_CSuplServerStartupRequest.cpp");
//#endif

// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CSuplServerStartupRequest::CSuplServerStartupRequest(MConnectObserver* aObserver,CSuplSessionManager& aSessionManager):
    CActive(EPriorityStandard),
    iReqPhase(ESuplReqInactive),
    iSessionManager(aSessionManager)
    {
    DEBUG_TRACE("CSuplServerStartupRequest::CSuplServerStartupRequest", __LINE__)
    CActiveScheduler::Add(this);
    iObserver=aObserver;
    }

// EPOC default constructor can leave.
void CSuplServerStartupRequest::ConstructL()
    {
    }

// Two-phased constructor.
CSuplServerStartupRequest* CSuplServerStartupRequest::NewL(MConnectObserver* aObserver,CSuplSessionManager& aSessionManager)
    {
    DEBUG_TRACE("CSuplServerStartupRequest::NewL", __LINE__)
    CSuplServerStartupRequest* self = new (ELeave) CSuplServerStartupRequest( aObserver, aSessionManager);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CSuplServerStartupRequest::~CSuplServerStartupRequest()
    {
    DEBUG_TRACE("CSuplServerStartupRequest::~CSuplServerStartupRequest", __LINE__)
	if (IsActive())      
      	Cancel();
    }

// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplSessionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerStartupRequest::MakeSuplServerStartupRequestL(CSuplSessionManager* aSessionManager,TUid aPluginUid,MSuplEcomEventObserver& aSuplEcomEventObserver)
  {
    DEBUG_TRACE("CSuplServerStartupRequest::MakeSuplSessionRequestL", __LINE__)
    iReqPhase = ESuplServerStartup;
#ifdef DEBUG
	    TBuf<255> tempBuf;
	    tempBuf.Append(_L("Plugin UID is : "));
	    tempBuf.AppendNum(aPluginUid.iUid);
	    TRACETEXT(tempBuf);
#endif
    
   if (aPluginUid.iUid != 0)
   {
   	iStatus = KRequestPending;
	
	
	aSessionManager->InitializeL(aPluginUid,iStatus,aSuplEcomEventObserver);

	SetActive();
   }
    	
//	    User::WaitForRequest(iStatus);
}


         



// ---------------------------------------------------------
// CSuplSessionRequest::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerStartupRequest::RunL()
    {
    DEBUG_TRACE("CSuplServerStartupRequest::RunL", __LINE__)
    TInt err = iStatus.Int();
    switch (iReqPhase)
        {
        case ESuplServerStartup:
            {
			iReqPhase = ESuplReqInactive;
			iObserver->CompleteConnect(err);
			break;
            }

        default :
            DebugPanic(EPosSuplServerPanicRequestInconsistency);
        }
        
    }

// ---------------------------------------------------------
// CSuplSessionRequest::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplServerStartupRequest::RunError(TInt /*aError*/)
    {
    DEBUG_TRACE("CSuplSessionRequest::RunError", __LINE__)
    return KErrNone;
    }

// ---------------------------------------------------------
// CSuplSessionRequest::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerStartupRequest::DoCancel()
    {
	DEBUG_TRACE("CSuplServerStartupRequest::DoCancel", __LINE__)
   	iSessionManager.CancelInitialize();
    }


    
//  End of File
