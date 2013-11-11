/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements helper class to handle location server tiimer related functionality
*
*/

#include "clocationservertimerhandler.h"
#include "locationmanagerdebug.h"



// --------------------------------------------------------------------------
// CLocationServerTimerHandler::CLocationServerTimerHandler
// --------------------------------------------------------------------------
//
CLocationServerTimerHandler::CLocationServerTimerHandler
            (MLocationServerTimerObserver& aLocationServerTimerObserver):
        CTimer( EPriorityStandard ),
        iLocationServerTimerObserver(aLocationServerTimerObserver),
        iLocationServerTimerType(MLocationServerTimerObserver::ELocGeneralPurpose)
    {
    }

// --------------------------------------------------------------------------
// CLocationServerTimerHandler::~CLocationServerTimerHandler
// --------------------------------------------------------------------------
//
CLocationServerTimerHandler::~CLocationServerTimerHandler()
    {
    LOG ("CLocationServerTimerHandler::~CLocationServerTimerHandler(), begin");
    Cancel();
    LOG ("CLocationServerTimerHandler::~CLocationServerTimerHandler(), end");
    }

// --------------------------------------------------------------------------
// CLocationServerTimerHandler::NewL
// --------------------------------------------------------------------------
//
CLocationServerTimerHandler* CLocationServerTimerHandler::NewL(MLocationServerTimerObserver& aLocationServerTimerObserver)
    {
    CLocationServerTimerHandler* self = 
        new( ELeave ) CLocationServerTimerHandler(aLocationServerTimerObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }


// --------------------------------------------------------------------------
// CLocationServerTimerHandler::ConstructL
// --------------------------------------------------------------------------
//
void CLocationServerTimerHandler::ConstructL()
    {
    LOG ("CLocationServerTimerHandler::ConstructL(), begin");
	CActiveScheduler::Add(this);
    CTimer::ConstructL();
    LOG ("CLocationServerTimerHandler::ConstructL(), end");
    }

// --------------------------------------------------------------------------
// CLocationServerTimerHandler::StartTimer
// --------------------------------------------------------------------------
//
void CLocationServerTimerHandler::StartTimer(const TTimeIntervalMicroSeconds32 anInterval,
   const MLocationServerTimerObserver::TLocationServerTimerType aLocationServerTimerType)
    {
    LOG ("CLocationServerTimerHandler::StartTimer(), begin");
    if(!IsActive())
        {
        LOG1("Starting timer of type - %d", aLocationServerTimerType);
        // not active.
        iLocationServerTimerType = aLocationServerTimerType;
        After( anInterval );
        }
    LOG ("CLocationServerTimerHandler::StartTimer(), end");
    }


// --------------------------------------------------------------------------
// CLocationServerTimerHandler::RunL
// --------------------------------------------------------------------------
//
void CLocationServerTimerHandler::RunL( )
    {
    LOG ("CLocationServerTimerHandler::RunL(), begin");
    TInt status = iStatus.Int();
    LOG1("Error code - %d", status);
    iLocationServerTimerObserver.LocationServerTimerCallBackL
            (iLocationServerTimerType,status);
    LOG ("CLocationServerTimerHandler::RunL(), end");
    }
    

// End of file

