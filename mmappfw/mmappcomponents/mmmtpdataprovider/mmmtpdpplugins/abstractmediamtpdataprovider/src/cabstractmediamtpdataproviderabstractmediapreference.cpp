/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implement the operation: 0x9203
*
*/


#include "cabstractmediamtpdataproviderabstractmediapreference.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::NewLC
// Two-phase construction method
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderAbstractMediaPreference* CAbstractMediaMtpDataProviderAbstractMediaPreference::NewLC( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection )
    {
    CAbstractMediaMtpDataProviderAbstractMediaPreference* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderAbstractMediaPreference( aFramework, aConnection );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderAbstractMediaPreference::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& /*aDpConfig*/ )
    {
    CAbstractMediaMtpDataProviderAbstractMediaPreference* self =
        CAbstractMediaMtpDataProviderAbstractMediaPreference::NewLC( aFramework, aConnection );
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::CAbstractMediaMtpDataProviderAbstractMediaPreference
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderAbstractMediaPreference::CAbstractMediaMtpDataProviderAbstractMediaPreference( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection ) :
    CRequestProcessor( aFramework, aConnection, 0, NULL )
    {
    PRINT( _L( "Operation: WMPReportSyncOnlyAbstractMedias(0x9203)" ) );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::~CAbstractMediaMtpDataProviderAbstractMediaPreference
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderAbstractMediaPreference::~CAbstractMediaMtpDataProviderAbstractMediaPreference()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::ConstructL
// Two-phased constructor
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderAbstractMediaPreference::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderAbstractMediaPreference::ServiceL
//
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderAbstractMediaPreference::ServiceL()
    {
    TUint32 referenceType = EMTPAbstractMediaRefOrdinary;
    SendResponseL( EMTPRespCodeOK, 1, &referenceType );
    }

// end of file

