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
* Description:  Send Ss barrings observer
*
*/


#include    "csatsendssbarringnouiobs.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::CSatSendSsBarringNoUiObs
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSatSendSsBarringNoUiObs::CSatSendSsBarringNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::CSatSendSsBarringNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::~CSatSendSsBarringNoUiObs
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSendSsBarringNoUiObs::~CSatSendSsBarringNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::~CSatSendSsBarringNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::HandleBarringModeChangedL
// Notification of barring mode change
// -----------------------------------------------------------------------------
//
void CSatSendSsBarringNoUiObs::HandleBarringModeChangedL(
    TBarringProgram aType,
    TBarringStatus aStatus,
    TBool aPlural )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringModeChangedL calling" )

    LOG2( DETAILED, "  Barring program: %i", aType )
    LOG2( DETAILED, "  Barring status:  %i", aStatus )
    LOG2( DETAILED, "  Plural:          %i", aPlural)

    // Avoid warnigs about unused parameters.
    aType = aType;
    aStatus = aStatus;
    aPlural = aPlural;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringModeChangedL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::HandleBarringModeStatusL
// Notification of barring mode status
// -----------------------------------------------------------------------------
//
void CSatSendSsBarringNoUiObs::HandleBarringModeStatusL(
    TUint8 aBsc[KPSetNumberOfBsc],
    TBarringStatus aStatus )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringModeStatusL calling" )

    LOG2( DETAILED, "  Barring status:  %i", aStatus )

    // To avoid warnigs about unused parameters
    aBsc = aBsc;
    aStatus = aStatus;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringModeStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::HandleBarringErrorL
// Notification of errors in call barring requests
// -----------------------------------------------------------------------------
//
void CSatSendSsBarringNoUiObs::HandleBarringErrorL(
    TInt aReason )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringErrorL calling" )

    LOG2( DETAILED, "  Barring error: %i", aReason )

    // To avoid warnigs about unused parameters
    aReason = aReason;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleBarringErrorL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::HandleCBRequestingL
// Notification of requesting
// -----------------------------------------------------------------------------
//
void CSatSendSsBarringNoUiObs::HandleCBRequestingL(
    TBool aOngoing, TBool aInterrupted )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleCBRequestingL calling" )

    LOG2( DETAILED, "  ongoing: %i", aOngoing )
    LOG2( DETAILED, "  interrupted: %i", aInterrupted )

    // To avoid warnigs about unused parameters
    aOngoing = aOngoing;
    aInterrupted = aInterrupted;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::HandleCBRequestingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::CbPasswordChangedL
// Notification relating to cb password change requests.
// -----------------------------------------------------------------------------
//
void CSatSendSsBarringNoUiObs::CbPasswordChangedL(
    TBool aSuccess )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::CbPasswordChangedL calling" )

    LOG2( DETAILED, "  success: %i", aSuccess )

    // To avoid warnigs about unused parameters
    aSuccess = aSuccess;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::CbPasswordChangedL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsBarringNoUiObs::SetEngineContact
// Notification relating to cb password change requests.
// -----------------------------------------------------------------------------
//
 void CSatSendSsBarringNoUiObs::SetEngineContact(
    MPsetCallBarring* /*aBarringEngine*/ )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsBarringNoUiObs::SetEngineContact calling-exiting" )
    }

//  End of File
