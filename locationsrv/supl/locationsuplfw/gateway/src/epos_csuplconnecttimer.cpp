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
* Description:   Class to handle server delayed shutdown
*
*/



// INCLUDE FILES
#include <e32base.h>

#include "epos_csuplconnecttimer.h"
#include "epos_csuplglobal.h"

//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplconnecttimer.cpp");
//#endif


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplConnectTimer::CSuplConnectTimer(MConnectTimerObserver *aObserver)
    : CTimer(EPriorityLow),
    iObserver(aObserver)
    {
    DEBUG_TRACE("CSuplConnectTimer", __LINE__)
    CActiveScheduler::Add(this);
    }

// Destructor
CSuplConnectTimer::~CSuplConnectTimer()
    {
    DEBUG_TRACE("~CSuplConnectTimer", __LINE__)
    Cancel();
    }

// Two-phased constructor
CSuplConnectTimer* CSuplConnectTimer::NewL(MConnectTimerObserver *aObserver)
    {
    DEBUG_TRACE("NewL", __LINE__)
    CSuplConnectTimer* self = new (ELeave) CSuplConnectTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// EPOC default constructor
void CSuplConnectTimer::ConstructL()
    {
    DEBUG_TRACE("ConstructL", __LINE__)
    CTimer::ConstructL();
    }

// ---------------------------------------------------------
// CSuplConnectTimer::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplConnectTimer::Start(TTimeIntervalMicroSeconds32 aDelay)
    {
    DEBUG_TRACE("Start", __LINE__)
    After(aDelay);
    DEBUG_TRACE("End of Start", __LINE__)
    }

// ---------------------------------------------------------
// CSuplConnectTimer::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplConnectTimer::RunL()
    {
    DEBUG_TRACE("RunL", __LINE__)
    
    iObserver->CompleteConnectTimerL();
    }

// End of File
