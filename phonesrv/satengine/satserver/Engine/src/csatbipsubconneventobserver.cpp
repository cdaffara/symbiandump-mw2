/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer of Subconnection parameters granted event
*
*/


#include <cs_subconevents.h>
#include "csatbipsubconneventobserver.h"
#include "CSatBIPGPRSDataChannel.h"
#include "SatLog.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSatBIPSubConnEventObserver::CSatBIPSubConnEventObserver(
    CSatBIPGPRSDataChannel& aDataChannel ) :
    CActive( EPriorityNormal ),
    iDataChannel( aDataChannel )
    {
    LOG( NORMAL, "SATENGINE: CSatBIPSubConnEventObserver::\
        CSatBIPSubConnEventObserver calling" )
    CActiveScheduler::Add( this );
    
    iEventFilter.iEventGroupUid = KSubConnGenericEventsImplUid;
    iEventFilter.iEventMask =
        KSubConGenericEventParamsRejected | KSubConGenericEventParamsGranted;
        
    LOG( NORMAL, "SATENGINE: CSatBIPSubConnObserver::\
        CSatBIPSubConnEventObserver exiting" )
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatBIPSubConnEventObserver::~CSatBIPSubConnEventObserver()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPSubConnEventObserver::~CSatBIPSubConnEventObserver \
        calling" )
    Cancel();
    LOG( SIMPLE,
        "SATENGINE: CSatBIPSubConnEventObserver::~CSatBIPSubConnEventObserver \
        exiting" )
    }

// ---------------------------------------------------------------------------
// Starts to observe connection status
// ---------------------------------------------------------------------------
//
void CSatBIPSubConnEventObserver::StartObservSubConnEvent()
    {
    LOG( NORMAL, 
        "SATENGINE: CSatBIPSubConnEventObserver::StartObservSubConnEvent\
         calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "  Activating observer" )
        iDataChannel.SubConnection().EventNotification(
            iEventBuffer, &iEventFilter, 1, iStatus );
        SetActive();
        }

    LOG( NORMAL, 
        "SATENGINE: CSatBIPSubConnEventObserver::StartObservSubConnEvent\
        exiting" )
    }

// ---------------------------------------------------------------------------
// From base class
// RunL
// ---------------------------------------------------------------------------
//
void CSatBIPSubConnEventObserver::RunL()
    {
    LOG( NORMAL, "SATENGINE: CSatBIPSubConnEventObserver::RunL calling" )

    const TInt errStatus( iStatus.Int() );
    LOG2( SIMPLE, " RunL (%i)", errStatus )
    if ( KErrNone == errStatus )
        {
        iDataChannel.GrantedEventReceivedL( iEventBuffer );
        }
    else
        {
        LOG( NORMAL, 
            "SATENGINE: CSatBIPSubConnEventObserver::RunL Leave with err" )
        User::LeaveIfError( errStatus );
        }

    LOG( NORMAL, "SATENGINE: CSatBIPSubConnEventObserver::RunL exiting" )
    }

// ---------------------------------------------------------------------------
// From base class
// DoCancel
// ---------------------------------------------------------------------------
//
void CSatBIPSubConnEventObserver::DoCancel()
    {
    LOG( NORMAL, "SATENGINE: CSatBIPSubConnEventObserver::DoCancel calling" )

    iDataChannel.SubConnection().CancelEventNotification();

    LOG( NORMAL, "SATENGINE: CSatBIPSubConnEventObserver::DoCancel exiting" )
    }

// End of File
