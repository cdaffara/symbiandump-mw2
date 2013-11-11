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
#include <centralrepository.h>

#include "epos_csuplserverdelayedshutdown.h"
#include "epos_csuplserver.h"
#include "epos_csuplglobal.h"

//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_cposserverdelayedshutdown.cpp");
//#endif


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplServerDelayedShutdown::CSuplServerDelayedShutdown(CSuplServer& aServer)
    : CTimer(EPriorityLow),
      iSuplServer(aServer)
    {
    DEBUG_TRACE("CSuplServerDelayedShutdown", __LINE__)
    CActiveScheduler::Add(this);
    }

// Destructor
CSuplServerDelayedShutdown::~CSuplServerDelayedShutdown()
    {
    DEBUG_TRACE("~CSuplServerDelayedShutdown", __LINE__)
    Cancel();
    }

// Two-phased constructor
CSuplServerDelayedShutdown* CSuplServerDelayedShutdown::NewL(CSuplServer& aServer)
    {
    DEBUG_TRACE("NewL", __LINE__)
    CSuplServerDelayedShutdown* self = new (ELeave) CSuplServerDelayedShutdown(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// EPOC default constructor
void CSuplServerDelayedShutdown::ConstructL()
    {
    DEBUG_TRACE("ConstructL", __LINE__)
    CTimer::ConstructL();
    }

// ---------------------------------------------------------
// CSuplServerDelayedShutdown::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerDelayedShutdown::Start(TTimeIntervalMicroSeconds32 aDelay)
    {
    DEBUG_TRACE("Start", __LINE__)
    After(aDelay);
    }

// ---------------------------------------------------------
// CSuplServerDelayedShutdown::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServerDelayedShutdown::RunL()
    {
    DEBUG_TRACE("RunL", __LINE__)
    iSuplServer.DeInitialize(); 
    }

// End of File
