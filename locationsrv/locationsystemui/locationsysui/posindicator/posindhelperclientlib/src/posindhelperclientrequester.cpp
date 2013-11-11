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
* Description:  Positioning Indicator Helper Client Requestor
*
*/


// INCLUDE FILES

#include <e32base.h>
#include <s32strm.h>
#include <s32mem.h>

#include "posindhelperclientrequester.h"
#include "posindicatorhelper.h"
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosIndHelperClientRequester::CPosIndHelperClientRequester
//
// Default constructor
// ---------------------------------------------------------
//
CPosIndHelperClientRequester::CPosIndHelperClientRequester(
            MServerRequestCompletetionObserver& aServerRequestCompletetionObserver ):
                    CActive(EPriorityStandard),iServerRequestCompletetionObserver(aServerRequestCompletetionObserver)
    {
    FUNC("+ CPosIndHelperClientRequester::CPosIndHelperClientRequester");
    CActiveScheduler::Add(this);
    FUNC("- CPosIndHelperClientRequester::CPosIndHelperClientRequester");
    }

// ---------------------------------------------------------------------------
// CPosIndHelperClientRequester::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CPosIndHelperClientRequester::ConstructL()
    {
    FUNC("+ CPosIndHelperClientRequester::ConstructL");
    FUNC("- CPosIndHelperClientRequester::ConstructL");
    }

// ---------------------------------------------------------------------------
// CPosIndHelperClientRequester::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPosIndHelperClientRequester* CPosIndHelperClientRequester::NewL(
        MServerRequestCompletetionObserver& aServerRequestCompletetionObserver )
    {
    FUNC("+ CPosIndHelperClientRequester::NewL");
    CPosIndHelperClientRequester* self = new (ELeave) CPosIndHelperClientRequester(
                                aServerRequestCompletetionObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
   

// ---------------------------------------------------------------------------
// CPosIndHelperClientRequester::~CPosIndHelperClientRequester()
// Destructor
// ---------------------------------------------------------------------------
//
CPosIndHelperClientRequester::~CPosIndHelperClientRequester()
    {
    FUNC("+ CPosIndHelperClientRequester::~CPosIndHelperClientRequester");
    Cancel();
    FUNC("- CPosIndHelperClientRequester::~CPosIndHelperClientRequester");
    }


// ---------------------------------------------------------
// CPosIndHelperClientRequester::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosIndHelperClientRequester::Start(TServiceIds aServiceId )
    { 
    FUNC("+ CPosIndHelperClientRequester::Start");
    iServiceId = aServiceId;
 	SetActive();
    FUNC("- CPosIndHelperClientRequester::Start");
    }

// ---------------------------------------------------------
// CPosIndHelperClientRequester::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosIndHelperClientRequester::RunL()
   {
    FUNC("+ CPosIndHelperClientRequester::RunL");
    LOG("Error code : %d", iStatus.Int());
    iServerRequestCompletetionObserver.NotifyServerRequestCompletion( iServiceId, iStatus.Int() );
    FUNC("- CPosIndHelperClientRequester::RunL");
   }


// ---------------------------------------------------------
// CPosIndHelperClientRequester::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosIndHelperClientRequester::RunError(TInt /*aError*/)
    {
    FUNC("+ CPosIndHelperClientRequester::RunError");
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosIndHelperClientRequester::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosIndHelperClientRequester::DoCancel()
    {    
    FUNC("+ CPosIndHelperClientRequester::DoCancel");
   // iServerRequestCompletetionObserver.NotifyServerRequestCompletion( iServiceId, iStatus.Int() );
    FUNC("- CPosIndHelperClientRequester::DoCancel");
    }
//  End of File


