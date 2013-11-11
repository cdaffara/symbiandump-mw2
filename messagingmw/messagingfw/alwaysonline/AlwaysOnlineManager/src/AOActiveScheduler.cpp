/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Always Online class
*
*/


// INCLUDES
#include "AOActiveScheduler.h"
#include "AlwaysOnlineManagerServer.h"
#include "AlwaysOnlineManagerLogging.h"

// CONSTANTS

#ifdef _DEBUG
_LIT(KAOActiveSchedulerPanic, "AOActiveScheduler");
const TInt KAOSchedulerPanicCode = -1;
#endif

//---------------------------------------------
// Constructor
//---------------------------------------------
CAOActiveScheduler::CAOActiveScheduler() 
{
}

//---------------------------------------------
// Destructor
//---------------------------------------------
CAOActiveScheduler::~CAOActiveScheduler()
{
    // this not owned, just set null
    iAoServer = NULL;
}

//---------------------------------------------
// Destructor
//---------------------------------------------
void CAOActiveScheduler::SetServerRef( CAlwaysOnlineManagerServer* aAoServer )
{
    iAoServer = aAoServer;    
}


//---------------------------------------------
// CAOActiveScheduler::Error()
//---------------------------------------------
void CAOActiveScheduler::Error( TInt anError ) const
{

    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOActiveScheduler::Error() ERROR IN AO ACTIVE SCHEDULER error code: ", anError );
    
    TPckgBuf<TInt> errorBuffer( anError );
    
    //check that error is short enough value for 
    //descriptor

    TInt error = KErrNone;
    
    // make sure thet AoServer pointer is set
    if ( iAoServer )
        {
        TRAP(error, iAoServer->HandleClientCommandL( 
            EServerAPIBaseCommandAoSchedulerError , errorBuffer ); );
        }
    else
        {
        error = KErrNotFound;
        }

    // panic in debug mode if there is error
    __ASSERT_DEBUG( error == KErrNone, 
        User::Panic( KAOActiveSchedulerPanic, KAOSchedulerPanicCode ) );
   
    KAOMANAGER_LOGGER_WRITE("CAOActiveScheduler::Error() ERROR is handled." );
}
