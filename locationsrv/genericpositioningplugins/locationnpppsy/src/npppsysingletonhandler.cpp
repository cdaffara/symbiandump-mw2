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
* Description:  Handler for singleton objects
*
*/



// INCLUDE FILES
#include <e32std.h>

#include "npppsysingletonhandler.h"
#include "npppsystatushandler.h"
#include "npppsysettinghandler.h"
#include "npppsylogging.h"
#include "npppsypanic.h"


// ======================== ==  == MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cnpppsysingletonhandler::GetInstanceL
// -----------------------------------------------------------------------------
//
CNppPsySingletonHandler* CNppPsySingletonHandler::GetInstanceL( 
    MPositionerStatus& aStatusInterface )
    {
    TRACESTRING( "CNppPsySingletonHandler:: GetInstanceL" )
    CNppPsySingletonHandler* self = 
        reinterpret_cast < CNppPsySingletonHandler*>( Dll::Tls() );

    if ( !self )
        {
        self = new( ELeave ) CNppPsySingletonHandler;
        CleanupStack::PushL( self );
        self->ConstructL( aStatusInterface );
        CleanupStack::Pop( self );
        Dll::SetTls( self );
        }

    self->iRefCount++;
    return self;
    }


// -----------------------------------------------------------------------------
// CNppPsySingletonHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CNppPsySingletonHandler::ConstructL( MPositionerStatus& aStatusInterface )
    {
    //Construct status handler
    iStatusHandler = CNppPsyStatusHandler::NewL( aStatusInterface );

    //Construct setting handler
    iSettingHandler = CNppPsySettingHandler::NewL( *iStatusHandler );
    }

// -----------------------------------------------------------------------------
// CNppPsySingletonHandler::~CNppPsySingletonHandler
// -----------------------------------------------------------------------------
//
CNppPsySingletonHandler::~CNppPsySingletonHandler()
    {
    delete iSettingHandler;
    delete iStatusHandler;
    }


// -----------------------------------------------------------------------------
// CNppPsySingletonHandler::ReleaseInstance
// -----------------------------------------------------------------------------
//
void CNppPsySingletonHandler::ReleaseInstance()
    {
    iRefCount--;
    if ( iRefCount == 0 )
        {
        delete this;
        Dll::SetTls( NULL );
        }
    }

// -----------------------------------------------------------------------------
// CNppPsySingletonHandler::StatusHandler
// -----------------------------------------------------------------------------
//
CNppPsyStatusHandler& CNppPsySingletonHandler::StatusHandler()
    {
    return *iStatusHandler;
    }

// -----------------------------------------------------------------------------
// CNppPsySingletonHandler::SettingHandler
// -----------------------------------------------------------------------------
//
CNppPsySettingHandler& CNppPsySingletonHandler::SettingHandler()
    {
    return *iSettingHandler;
    }


//  End of File  

