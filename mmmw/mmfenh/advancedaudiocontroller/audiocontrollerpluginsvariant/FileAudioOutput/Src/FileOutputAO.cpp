/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the implementation of the File Audio Output Event
*		 		 Generator.
*
*/



// INCLUDE FILES
#include 	"FileOutputAO.h"
#ifdef _DEBUG
#include 	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFAOEventGenerator::CFAOEventGenerator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFAOEventGenerator::CFAOEventGenerator(MFAOEventGeneratorObserver& aObserver)
    :   CActive(CActive::EPriorityStandard),
        iEvent(0),
        iObserver(&aObserver)
    {
    }

// -----------------------------------------------------------------------------
// CFAOEventGenerator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFAOEventGenerator::ConstructL()
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CFAOEventGenerator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFAOEventGenerator* CFAOEventGenerator::NewL(
    MFAOEventGeneratorObserver& aObserver )
    {
    CFAOEventGenerator* self = new(ELeave) CFAOEventGenerator(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CFAOEventGenerator::~CFAOEventGenerator
// Destructor
// -----------------------------------------------------------------------------
//
CFAOEventGenerator::~CFAOEventGenerator()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CFAOEventGenerator::GenerateEvent
// Generate the specified event.
// -----------------------------------------------------------------------------
//
void CFAOEventGenerator::GenerateEvent(
    TInt aEvent, TInt aError)
    {
#ifdef _DEBUG
    RDebug::Print(_L("[%x] CFAOEventGenerator::GenerateEvent()\n"), this);
#endif
    // save arguments and set code
    if (!IsActive())
        {
        iEvent = aEvent;
        iError = aError;
        iStatus = KRequestPending; // service request would be made here and pending set by service provider
        SetActive();
        Ready(KErrNone);
        }
    }


// -----------------------------------------------------------------------------
// CFAOEventGenerator::Ready
// Utility function to post a request complete
// -----------------------------------------------------------------------------
//
void CFAOEventGenerator::Ready(
    const TInt aStatus)
    {
    TRequestStatus* stat = &iStatus;
    User::RequestComplete(stat, aStatus);
    }


// -----------------------------------------------------------------------------
// CFAOEventGenerator::RunL
// Invoke by the active scheduler when a request completes
// -----------------------------------------------------------------------------
//
void CFAOEventGenerator::RunL()
    {
 	switch(iEvent)
		{
		case KIOError:
			iObserver->IOError(iError);
			break;

		default:
			break;
		};

    }

// -----------------------------------------------------------------------------
// CFAOEventGenerator::DoCancel
// Cancels the current and any on going requests/tasks.
// -----------------------------------------------------------------------------
//
void CFAOEventGenerator::DoCancel()
    {
#ifdef _DEBUG
    RDebug::Print(_L("[%x] CFAOEventGenerator::DoCancel()\n"), this);
#endif
    }

// End of file

