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
* Description:  Send Ss Cli observer
*
*/


#include    "csatsendssclinouiobs.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::CSatSendSsCliNoUiObs
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSatSendSsCliNoUiObs::CSatSendSsCliNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCliNoUiObs::CSatSendSsCliNoUiObs calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::~CSatSendSsCliNoUiObs
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSendSsCliNoUiObs::~CSatSendSsCliNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCliNoUiObs::~CSatSendSsCliNoUiObs calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::CliInformationL
// Notification of Cli information
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::CliInformationL( TPsuiCli aType )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::CliInformationL calling" )

    LOG2( DETAILED, "  Type: %i", aType )

    // Avoid warnigs about unused parameters
    aType =  aType;

    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::CliInformationL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::HandleCliRequestingL
// Notification of requesting
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::HandleCliRequestingL(
    TBool aOngoing,
    TBool aInterrupted )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliRequestingL calling" )

    LOG2( DETAILED, "  ongoing: %i", aOngoing )
    LOG2( DETAILED, "  interrupted: %i", aInterrupted )

    // Avoid warnigs about unused parameters
    aOngoing = aOngoing;
    aInterrupted = aInterrupted;

    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliRequestingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::SetEngineContact
//
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::SetEngineContact( MPsetCli* /*aEngine*/ )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsCliNoUiObs::SetEngineContact calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::HandleCliStatusL
// Notification of Cli status
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::HandleCliStatusL(
    TUint8 aBsc[KPSetNumberOfBsc],
    TPsuiCli aMode )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliStatusL calling" )

    LOG2( DETAILED, "  Type: %i", aMode )

    // Avoid warnigs about unused parameters
    aBsc = aBsc;
    aMode = aMode;

    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::HandleCnapStatusL
// Notification of the status of Cli if requested
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::HandleCnapStatusL( TInt aStatus )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCnapStatusL calling" )

    LOG2( DETAILED, "  status: %i", aStatus )

    // Avoid warnigs about unused parameters
    aStatus = aStatus;

    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCnapStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsCliNoUiObs::HandleCliErrorL
// Notification of error in Cli handling
// -----------------------------------------------------------------------------
//
void CSatSendSsCliNoUiObs::HandleCliErrorL( TInt aError )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliErrorL calling" )

    LOG2( DETAILED, "  Cli error: %i", aError )

    // Avoid warnigs about unused parameters
    aError = aError;

    LOG( SIMPLE, "SENDSS: CSatSendSsCliNoUiObs::HandleCliErrorL exiting" )
    }

//  End of File
