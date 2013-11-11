/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWGsmSessionImplementation
*                class member functions.
*
*/



// INCLUDE FILES
#include "CNWGsmSessionImplementation.h"
#include "CNWGsmMessageHandler.h"
#include <nwhandlingengine.h>
#include "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWGsmSessionImplementation::CNWGsmSessionImplementation
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWGsmSessionImplementation::CNWGsmSessionImplementation(
    MNWMessageObserver& aMessageObserver,
    TNWInfo& aNWInfo )
    : CNWSession( ),
      iMessageObserver( aMessageObserver ),
      iNWInfo( aNWInfo )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::\
        CNWGsmSessionImplementation() called " );
    }

// ----------------------------------------------------------------------------
// CNWGsmSessionImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWGsmSessionImplementation::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::ConstructL() Begin " );
    
    iGsmMessageHandler = CNWGsmMessageHandler::NewL( 
            *this,
            iMessageObserver,
            iNWInfo );
    
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::ConstructL() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmSessionImplementation::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CNWGsmSessionImplementation* CNWGsmSessionImplementation::NewL(
        MNWMessageObserver& aMessageObserver,
        TNWInfo& aNWInfo )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::NewL() Begin " );
    
    CNWGsmSessionImplementation* self = 
            new ( ELeave ) CNWGsmSessionImplementation( aMessageObserver,
                                                        aNWInfo );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::ConstructL() End " );
    return self;
    }

// ----------------------------------------------------------------------------
// CNWGsmSessionImplementation::~CNWGsmSessionImplementation()
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CNWGsmSessionImplementation::~CNWGsmSessionImplementation( )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::\
        ~CNWGsmSessionImplementation() Begin " );
    
    delete iGsmMessageHandler;
    
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmSessionImplementation::\
        ~CNWGsmSessionImplementation() End " );
    }


//  End of File
