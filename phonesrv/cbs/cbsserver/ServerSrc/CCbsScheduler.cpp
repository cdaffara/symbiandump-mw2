/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsScheduler class 
*                member functions.   
*
*/


//  INCLUDES
#include "CbsServerPanic.h"
#include "CCbsScheduler.h"
#include "CCbsServer.h"
 
// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsScheduler::CCbsScheduler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsScheduler::CCbsScheduler()
    {
    }

// -----------------------------------------------------------------------------
// CCbsScheduler::SetServer
// Gives a pointer of the current CCbsServer instance to the scheduler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsScheduler::SetServer( 
    CCbsServer* aServer )
    {
    iServer = aServer;
    }

// -----------------------------------------------------------------------------
// CCbsScheduler::Error
// Overridden to handle leaves in request functions and unexpected situations.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsScheduler::Error( 
    TInt aError ) const
    {
    // Server caused the error
    if ( iServer->IsActive() )
        {
        __DEBUGGER();
        }
    else
        {
        // Panic the client.
        if ( aError == KErrBadDescriptor )
            {
            iServer->PanicClient( KErrBadDescriptor );
            }
        // Otherwise relay the error code to the client and restart the server
        iServer->ServerMessage().Complete( aError );
        iServer->ReStart();
        }    
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  
