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
* Description:  Implementation class for supltiapi
*
*/

#include <ecom/ecom.h>
#include <badesca.h>

#include "lbssupltiapiimplementation.h"     
#include "epos_tiapiimpltrace.h"
#include "lbssupltirequestmanager.h"

_LIT(KTraceFileName,"lbssupltiapiimplementation.cpp");


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::CLbsSuplTiApiImplementation
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CLbsSuplTiApiImplementation::CLbsSuplTiApiImplementation(MLbsSuplTiObserver& aObserver):CLbsSuplTiApi(aObserver)
        { 

        }

// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::~CLbsSuplTiApiImplementation
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
EXPORT_C CLbsSuplTiApiImplementation::~CLbsSuplTiApiImplementation()
    {
    if(iTrace)
        {
        iTrace->Trace(_L("CLbsSuplTiApiImplementation::~CLbsSuplTiApiImplementation"), KTraceFileName, __LINE__); 
        delete iTrace;
        iTrace = NULL;
        }
    if(iRequestManager)
        {
        delete iRequestManager;
        iRequestManager = NULL;
        }

    }

// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::NewL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//

EXPORT_C  CLbsSuplTiApiImplementation* CLbsSuplTiApiImplementation::NewL(MLbsSuplTiObserver& aObserver)
    {
    CLbsSuplTiApiImplementation* self = new(ELeave) CLbsSuplTiApiImplementation(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();//remove observer
    CleanupStack::Pop(self);
    return self;
    }           

// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::ConstructL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CLbsSuplTiApiImplementation::ConstructL() 
    {
    iRequestManager = CCLbsSuplTiRequestManager::NewL(iSuplTiObserver);
    iTrace = COMASuplTrace::NewL();	
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::ConstructL()"), KTraceFileName, __LINE__); 
    }           

// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::RequestLocation
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CLbsSuplTiApiImplementation::RequestLocation(const TLbsNetSessionId& aSessionId,
                                  const TLbsNetPosRequestOptionsBase& aOptions, const TLbsNetPosRequestMethod& aMethod)
    {
    //delegate request to request manager
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::Requesting location"), KTraceFileName, __LINE__); 
    iRequestManager->RequestLocation(aSessionId,aOptions,aMethod);
    }
// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::CancelRequest
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CLbsSuplTiApiImplementation::CancelRequest(const TLbsNetSessionId& aSessionId)
    {
    //delegate request to request manager
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::Cancelling request location"), KTraceFileName, __LINE__);
    return iRequestManager->CancelRequest(aSessionId);
    }
// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::CloseSession
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CLbsSuplTiApiImplementation::CloseSession()
    {
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::CloseSession called"), KTraceFileName, __LINE__);
    iRequestManager->CloseSession();
    }
// -----------------------------------------------------------------------------
// CLbsSuplTiApiImplementation::GetPosition
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CLbsSuplTiApiImplementation::GetPosition(const TLbsNetSessionId& aSessionId, TPositionInfoBase& aPositionInfo)
    {
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::GetPosition called"), KTraceFileName, __LINE__);
    //delegate request to request manager
    return iRequestManager->GetPosition(aSessionId,aPositionInfo);
    }
