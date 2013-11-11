/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Send Ss call waiting observer
*
*/


#include    "csatsendsscallwaitingnouiobs.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::CSatSendSsCallWaitingNoUiObs
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSatSendSsCallWaitingNoUiObs::CSatSendSsCallWaitingNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::CSatSendSsCallWaitingNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::~CSatSendSsCallWaitingNoUiObs
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSendSsCallWaitingNoUiObs::~CSatSendSsCallWaitingNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::~CSatSendSsCallWaitingNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::HandleCallWaitingGetStatusL
// Notification of call waiting mode change
// -----------------------------------------------------------------------------
//
void CSatSendSsCallWaitingNoUiObs::HandleCallWaitingGetStatusL(
    MPsetCallWaiting::TGetCallWaitingStatus aStatus,
    TUint8 aBsc[KPSetNumberOfBsc] )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCallWaitingGetStatusL \
        calling" )
    LOG2( DETAILED, "  call waiting status: %i", aStatus )

    // Avoid warnigs about unused parameters
    aStatus = aStatus;
    aBsc = aBsc;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCallWaitingGetStatusL \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::HandleCallWaitingChangedL
// Notification of call waiting change
// -----------------------------------------------------------------------------
//
void CSatSendSsCallWaitingNoUiObs::HandleCallWaitingChangedL(
    MPsetCallWaiting::TSetCallWaiting aSetting,
    TInt aResult )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCallWaitingChangedL \
        calling" )

    LOG2( DETAILED, "  call waiting status: %i", aSetting )
    LOG2( DETAILED, "  result:              %i", aResult )

    // Avoid warnigs about unused parameters
    aSetting = aSetting;
    aResult = aResult;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCallWaitingChangedL \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::HandleCWErrorL
// Notification of errors in call waitings
// -----------------------------------------------------------------------------
//
void CSatSendSsCallWaitingNoUiObs::HandleCWErrorL( TInt aReason )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCWErrorL calling" )
    LOG2( DETAILED, "  CallWaiting error: %i", aReason )

    // Avoid warnigs about unused parameters
    aReason = aReason;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCWErrorL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::HandleCWRequestingL
// Notification of requesting
// -----------------------------------------------------------------------------
//
void CSatSendSsCallWaitingNoUiObs::HandleCWRequestingL(
    TBool aOngoing, TBool aInterrupted )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCWRequestingL calling" )
    LOG2( DETAILED, "  ongoing: %i", aOngoing )
    LOG2( DETAILED, "  interrupted: %i", aInterrupted )

    // Avoid warnigs about unused parameters
    aOngoing = aOngoing;
    aOngoing = aInterrupted;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::HandleCWRequestingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCallWaitingNoUiObs::SetEngineContact
//
// -----------------------------------------------------------------------------
//
void CSatSendSsCallWaitingNoUiObs::SetEngineContact(
    MPsetCallWaiting& /*aEngineContact*/ )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCallWaitingNoUiObs::SetEngineContact \
        calling-exiting" )
    }

//  End of File
