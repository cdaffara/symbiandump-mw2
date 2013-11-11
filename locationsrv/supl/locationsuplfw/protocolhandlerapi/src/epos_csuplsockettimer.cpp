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
* Description:   Class to handle socket timeout 
*
*/



// INCLUDE FILES
#include <e32base.h>
#include "epos_csuplsockettimer.h"
#include "epos_csuplconnection.h"

#include "epos_csuplconntrace.h"
_LIT(KTraceFileName, "SUPL_PH_API::epos_csuplsockettimer.cpp");


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSocketTimer::CSuplSocketTimer(CSuplConnection* aConnection)
    : CTimer(EPriorityLow),
    iConnection(aConnection)
    {
    CActiveScheduler::Add(this);
    }

// Destructor
CSuplSocketTimer::~CSuplSocketTimer()
    {
    Cancel();
    delete iTrace;
    }

// Two-phased constructor
CSuplSocketTimer* CSuplSocketTimer::NewL(CSuplConnection* aConnection)
    {
    CSuplSocketTimer* self = new (ELeave) CSuplSocketTimer(aConnection);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// EPOC default constructor
void CSuplSocketTimer::ConstructL()
    {
    iTrace = CSuplConnTrace::NewL();            
    iTrace->Trace(_L("CSuplSocketTimer::ConstructL"), KTraceFileName, __LINE__);
    CTimer::ConstructL();
    }

// ---------------------------------------------------------
// CSuplSocketTimer::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSocketTimer::Start(TTimeIntervalMicroSeconds32 aDelay)
    {
    iTrace->Trace(_L("CSuplSocketTimer::Start"), KTraceFileName, __LINE__);
    After(aDelay);
    }

// ---------------------------------------------------------
// CSuplSocketTimer::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSocketTimer::RunL()
    {
    iTrace->Trace(_L("CSuplSocketTimer::RunL"), KTraceFileName, __LINE__);
    //iConnection->Cancel();
    iConnection->TimerExpired();
    }

// End of File
