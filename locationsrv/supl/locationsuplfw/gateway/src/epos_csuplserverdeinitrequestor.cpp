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

#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplglobal.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csupldeinitrequestor.h"



// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplserverdeinitrequestor.cpp");
//#endif

// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CSuplServerDeInitRequest::CSuplServerDeInitRequest(CSuplSessionManager& aSessionManager):
    CActive(EPriorityStandard),
    iSessionManager(aSessionManager)
    {
    DEBUG_TRACE("CSuplServerDeInitRequest::CSuplServerDeInitRequest", __LINE__)
    CActiveScheduler::Add(this);
    }

// EPOC default constructor can leave.
void CSuplServerDeInitRequest::ConstructL()
    {
    }

// Two-phased constructor.
CSuplServerDeInitRequest* CSuplServerDeInitRequest::NewL(CSuplSessionManager& aSessionManager)
    {
    DEBUG_TRACE("CSuplServerDeInitRequest::NewL", __LINE__)
    CSuplServerDeInitRequest* self = new (ELeave) CSuplServerDeInitRequest(aSessionManager);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CSuplServerDeInitRequest::~CSuplServerDeInitRequest()
    {
    DEBUG_TRACE("CSuplServerDeInitRequest::~CSuplServerDeInitRequest", __LINE__)
	if (IsActive())      
      	Cancel();
    }

// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplSessionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerDeInitRequest::DeInitialize()
  {
    DEBUG_TRACE("CSuplServerDeInitRequest::DeInitialize", __LINE__)
   	iStatus = KRequestPending;
	iSessionManager.DeInitialize(iStatus);
	SetActive();
}

// ---------------------------------------------------------
// CSuplSessionRequest::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerDeInitRequest::RunL()
    {
    DEBUG_TRACE("CSuplServerDeInitRequest::RunL", __LINE__)
    // Shutdown the server by shutting down the active scheduler.
    CActiveScheduler::Stop();
    }

// ---------------------------------------------------------
// CSuplSessionRequest::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplServerDeInitRequest::RunError(TInt /*aError*/)
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
void CSuplServerDeInitRequest::DoCancel()
    {
	DEBUG_TRACE("CSuplServerDeInitRequest::DoCancel", __LINE__)
   	iSessionManager.CancelDeInitialize();
    }
//  End of File
