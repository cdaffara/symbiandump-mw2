/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the active scheduler of the SAT Server.
*
*/



//  INCLUDES
#include    <e32svr.h>             // RDebug
#include    "CSatSScheduler.h"
#include    "CSatSServer.h"
#include    "SatLog.h"

// ================= MEMBER FUNCTIONS ==========================================

// The class constructor.
EXPORT_C CSatSScheduler::CSatSScheduler()
    {
    LOG( SIMPLE, "SATENGINE: CSatSScheduler::SetServer calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSScheduler::SetServer
// Gives a pointer of the current CSatSServer instance to the scheduler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatSScheduler::SetServer( CSatSServer* aServer )
    {
    LOG( SIMPLE, "SATENGINE: CSatSScheduler::SetServer calling" )
    iServer = aServer;
    LOG( SIMPLE, "SATENGINE: CSatSScheduler::SetServer exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSScheduler::Error
// Overridden to handle leaves in request functions and unexpected situations.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSScheduler::Error( TInt aError ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSScheduler::Error calling" )
    LOG2( SIMPLE, "SATENGINE: CSatSScheduler::Error aError=%d", aError )

    if ( iServer )
        {
        if ( iServer->IsActive() )
            {
            LOG( SIMPLE, "SATENGINE: CSatSScheduler::Error iServer active" )
            __DEBUGGER()
            }
        else
            {
            // Relay the error code to the client and restart the server.
            LOG( SIMPLE, "SATENGINE: CSatSScheduler::Error iServer not active" )
            iServer->CompleteMessage( aError );
            iServer->ReStart();
            }        
        }
   
    LOG( SIMPLE, "SATENGINE: CSatSScheduler::Error exiting" )
    }

//  End of File
