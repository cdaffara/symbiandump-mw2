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
* Description:  Send Ss diverts observer
*
*/


#include    "csatsendssdivertnouiobs.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::CSatSendSsDivertNoUiObs
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSatSendSsDivertNoUiObs::CSatSendSsDivertNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::CSatSendSsDivertNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::~CSatSendSsDivertNoUiObs
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSendSsDivertNoUiObs::~CSatSendSsDivertNoUiObs()
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::~CSatSendSsDivertNoUiObs \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::HandleDivertingChangedL
// Notification of diverting change
// -----------------------------------------------------------------------------
//
void CSatSendSsDivertNoUiObs::HandleDivertingChangedL(
    const TCallDivertSetting& aSetting,
    TBool aPlural )
    {
    LOG( SIMPLE, "SENDSS: CSatSendSsDivertNoUiObs::HandleDivertingChangedL \
    calling" )

    LOG2( DETAILED, "  CallDivert setting Condition: %i",
        aSetting.iCondition )
    LOG2( DETAILED, "  CallDivert setting Setting: %i",
        aSetting.iSetting )
    LOG2( DETAILED, "  CallDivert setting Status: %i",
        aSetting.iStatus )
    LOG2( DETAILED, "  CallDivert setting Service group: %i",
        aSetting.iServiceGroup )
    LOG2( DETAILED, "  CallDivert setting Tel Number: %S",
         &aSetting.iNumber )
    LOG2( DETAILED, "  Plural:             %i",
        aPlural )

    // Avoid warnigs about unused parameters.
    aPlural = aPlural;
    aPlural = aSetting.iCondition;

    LOG( SIMPLE, "SENDSS: CSatSendSsDivertNoUiObs::HandleDivertingChangedL \
    exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::HandleDivertingStatusL
// Notification of diverting status change
// -----------------------------------------------------------------------------
//
void CSatSendSsDivertNoUiObs::HandleDivertingStatusL(
    CMobilePhoneCFList&,
    TBool )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::HandleDivertingStatusL \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::HandleDivertingErrorL
// Notification of errors in diverts
// -----------------------------------------------------------------------------
//
void CSatSendSsDivertNoUiObs::HandleDivertingErrorL( TInt aReason )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::HandleDivertingErrorL calling" )

    LOG2( DETAILED, "  Diverting error: %i", aReason )

    // Avoid warnigs about unused parameters.
    aReason = aReason;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::HandleDivertingErrorL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::HandleCFRequestingL
// Notification of requesting
// -----------------------------------------------------------------------------
//
void CSatSendSsDivertNoUiObs::HandleCFRequestingL(
    TBool aOngoing,
    TBool aInterrupted )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::HandleCFRequestingL calling" )

    LOG2( DETAILED, "  ongoing: %i", aOngoing )
    LOG2( DETAILED, "  interrupted: %i", aInterrupted )

    // Avoid warnigs about unused parameters.
    aOngoing = aOngoing;
    aInterrupted = aInterrupted;

    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::HandleCFRequestingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsDivertNoUiObs::SetEngineContact
//
// -----------------------------------------------------------------------------
//
void CSatSendSsDivertNoUiObs::SetEngineContact(
    MPsetCallDiverting* /*aDivertEngine*/ )
    {
    LOG( SIMPLE,
        "SENDSS: CSatSendSsDivertNoUiObs::SetEngineContact calling-exiting" )
    }

//  End of File
