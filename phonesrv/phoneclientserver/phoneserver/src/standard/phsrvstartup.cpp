/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Start Up functionality of Phone Server.
*
*/



// INCLUDE FILES
#include "phsrvstartup.h" 
#include "cphsrvscheduler.h" 
#include "cphsrvserver.h" 
#include "phsrvutils.h" 


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// PhoneServerStartUpUtils::CreateAndRunServerL
// 
// Creates a server instance, active scheduler, etc, and
// starts the server going by starting the active scheduler.
// -----------------------------------------------------------------------------
//
void PhoneServerStartUpUtils::CreateAndRunServerL()
    {
    CPhSrvScheduler* scheduler = new( ELeave ) CPhSrvScheduler();
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );

    // Create the server
    CPhSrvServer* server = CPhSrvServer::NewL();
    CleanupStack::PushL( server );

    // Attempt to rename the thread - it isn't critial if this fails
#ifdef _DEBUG
    TInt err = 
#endif // _DEBUG

    User::RenameThread( KPhServerThreadName );

    __ASSERT_DEBUG( err == KErrNone, 
        PhoneServerUtils::Panic(EPhoneServerPanicCouldNotRenameServerThread ) );

    // Now that the server has been started okay, we can signal the
    // client so that they can attempt to connect.

    RProcess::Rendezvous( KErrNone );

    // The final step in the chain is to start the active scheduler
    // which will allow the server to be fully operational and enable
    // request processing.
    CActiveScheduler::Start();

    // Cleanup the server and scheduler
    // Check also that the last poped item was scheduler.
    CleanupStack::PopAndDestroy( 2, scheduler );
    }


// -----------------------------------------------------------------------------
// PhoneServerStartUpUtils::PrepareServerEnvironment
// 
// Creates a cleanup stack trap harness and attemps to run
// the server.
// -----------------------------------------------------------------------------
//
TInt PhoneServerStartUpUtils::PrepareServerEnvironment()
    {
    TInt err = KErrNoMemory;

    CTrapCleanup* cleanup = CTrapCleanup::New();
    if ( cleanup )
        {
        TRAP( err, CreateAndRunServerL() );
        }
    delete cleanup;

    return err;
    }



// -----------------------------------------------------------------------------
// E32Main (MARM ONLY - RUNNING SERVER IN THREAD INSIDE PROCESS)
// 
// Satisfies EPOCEXE build criteria. Called by Kernel when
// a new process is created, and is used to start the server going.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    TInt err;

    err = PhoneServerStartUpUtils::PrepareServerEnvironment();

    return err;
    }



// End of File
