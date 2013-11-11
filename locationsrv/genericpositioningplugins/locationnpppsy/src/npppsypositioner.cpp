/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is NPP PSY Positioning Plug-in ( PSY ).
*
*/



// INCLUDE FILES
#include <e32std.h>            
#include <implementationproxy.h>  // TImplementationProxy
#include <lbspositioninfo.h>      // TPositionInfoBase

#include "npppsypositioner.h"
#include "npppsyrequester.h"
#include "npppsylogging.h"
#include "npppsypanic.h"
#include "npppsy.hrh"


// LOCAL CONSTANTS AND MACROS
const TImplementationProxy KFactoryPtr =
    IMPLEMENTATION_PROXY_ENTRY( KNppPsyImplUid, CNppPsyPositioner::NewL );

// ========================= ==  == LOCAL FUNCTIONS ===============================

// Following function is required by Positioning Plug-in API at ordinal 1.

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    aTableCount = 1;
    return &KFactoryPtr;
    }

// ========================== == MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNppPsyPositioner::CNppPsyPositioner
// C++default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CNppPsyPositioner::CNppPsyPositioner()
    {
    // Nothing to do here
    }


// -----------------------------------------------------------------------------
// CNppPsyPositioner::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNppPsyPositioner::ConstructL( TAny* aConstructionParameters )
    {
    TRACESTRING( "CNppPsyPositioner::ConstructL start" )

    // Calling BaseConstructL is required by Positioning Plug-in API.
    BaseConstructL( aConstructionParameters );
    
    // Construct requestor
    iRequester = CNppPsyRequester::NewL( *this );

    TRACESTRING( "CNppPsyPositioner::ConstructL end" )
    }


// -----------------------------------------------------------------------------
// CNppPsyPositioner::NewL
// Two - phased constructor.
// -----------------------------------------------------------------------------
//
CNppPsyPositioner* CNppPsyPositioner::NewL( TAny* aConstructionParameters )
    {
    CNppPsyPositioner* self = new( ELeave ) CNppPsyPositioner;
    
    CleanupStack::PushL( self );
    self->ConstructL( aConstructionParameters );
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CNppPsyPositioner::~CNppPsyPositioner
// 
// -----------------------------------------------------------------------------
//
CNppPsyPositioner::~CNppPsyPositioner()
    {
    if ( iRequester )
        {
        iRequester->Cancel();
        delete iRequester;
        }

    TRACESTRING( "CNppPsyPositioner:: destructed" )
    }

// -----------------------------------------------------------------------------
// CNppPsyPositioner::StatusReportInterface
// 
// -----------------------------------------------------------------------------
//
MPositionerStatus& CNppPsyPositioner::StatusReportInterface()
    {
    return *PositionerStatus();
    }


// -----------------------------------------------------------------------------
// CNppPsyPositioner::NotifyPositionUpdate
// 
// -----------------------------------------------------------------------------
//
void CNppPsyPositioner::NotifyPositionUpdate ( 
    TPositionInfoBase& aPosInfo,  
    TRequestStatus&    aStatus )
    {
    TRACESTRING( "CNppPsyPositioner::NotifyPositionUpdate start" )

    iRequester->NotifyPositionUpdate( aPosInfo, aStatus );
        
    TRACESTRING( "CNppPsyPositioner::NotifyPositionUpdate end" )
    }


// -----------------------------------------------------------------------------
// CNppPsyPositioner::CancelNotifyPositionUpdate
// 
// -----------------------------------------------------------------------------
//
void CNppPsyPositioner::CancelNotifyPositionUpdate ()
    {
    TRACESTRING( "CNppPsyPositioner::CancelNotifyPositionUpdate start" )

    if ( iRequester )
        {
        iRequester->Cancel();
        }
        
    TRACESTRING( "CNppPsyPositioner::CancelNotifyPositionUpdate end" )
    }


//  End of File  
