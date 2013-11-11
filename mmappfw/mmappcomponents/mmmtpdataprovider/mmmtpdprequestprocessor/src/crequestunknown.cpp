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
* Description: 
*
*/


#include <mtp/tmtptyperequest.h>
#include "crequestunknown.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CRequestUnknown::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CRequestUnknown::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection )
    {
    CRequestUnknown* self = new (ELeave) CRequestUnknown( aFramework, aConnection );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CRequestUnknown::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CRequestUnknown::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestUnknown::~CRequestUnknown
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRequestUnknown::~CRequestUnknown()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestUnknown::CRequestUnknown
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CRequestUnknown::CRequestUnknown( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL )
    {
    PRINT( _L( "Operation: Unknown" ) );
    }

// -----------------------------------------------------------------------------
// CRequestUnknown::ServiceL
// penSession request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestUnknown::ServiceL()
    {
    SendResponseL( EMTPRespCodeOperationNotSupported );
    }

// -----------------------------------------------------------------------------
// CRequestUnknown::Match
// Override to match both the unknown requests
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestUnknown::Match( const TMTPTypeRequest& /*aRequest*/,
    MMTPConnection& /*aConnection*/ ) const
    {
    return ETrue;
    }

// end of file
