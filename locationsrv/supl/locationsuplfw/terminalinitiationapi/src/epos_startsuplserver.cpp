/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL Server startup code
*
*/



#include <e32base.h>
#include <e32math.h>

#include "epos_suplterminal.h"
#include "epos_suplterminalipc.h"
#include "epos_startsuplserver.h"

#ifdef _DEBUG
#include <e32debug.h>
#endif
_LIT(KSuplServerStartupSemaphoreName, "SuplServerStartupSem");

/**
 * Start the server process/thread which lives in an EPOCEXE object.
 *
 * \internal
 * @return KErrNone if successful, any other error code otherwise
 */
TInt RSuplTerminalServer::StartServer()
    {
    TRequestStatus started;
    TInt ret;

    RSemaphore startupSemaphore;
    ret = startupSemaphore.CreateGlobal( KSuplServerStartupSemaphoreName, 0 );

    if ( ret == KErrAlreadyExists )
        {
        // The server is starting up, but has not yet started 
        startupSemaphore.OpenGlobal( KSuplServerStartupSemaphoreName );
        // wait until the server has started up, Max 5 secs.
        TInt err = startupSemaphore.Wait(5000000); 
        startupSemaphore.Close();
        if (err != KErrNone)
        	return err;
        else
        	return ret;
        }

#ifdef _DEBUG
    RDebug::Print(_L("SuplApi: Starting SUPL Server..."));
#endif
    const TUidType serverUid(KNullUid, KNullUid, KSuplServerUid3);

    // Simultaneous launching of two such processes should be detected 
    // when the second one attempts to create the server object, 
    // failing with KErrAlreadyExists.
    //
    RProcess server;
    ret = server.Create(KSuplServerImg, KNullDesC, serverUid);

    if (ret != KErrNone)
        {
#ifdef _DEBUG
        RDebug::Print(_L("SuplTerminalApi: server start failed %d"), ret);
#endif
        startupSemaphore.Close();
        return ret;
        }

    TRequestStatus died;
    server.Rendezvous(died);

    if (died != KRequestPending)
        {
        // logon failed - server is not yet running, so cannot have terminated
        User::WaitForRequest(died); // eat signal
        server.Kill(0);                         // abort startup
        }
    else
        {
        server.Resume();
        }
    User::WaitForRequest(died);                // wait for start or death

    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' which cannot be distinguished
    // from KErrNone
    ret = (server.ExitType() == EExitPanic) ? KErrGeneral : died.Int();
    server.Close();
    startupSemaphore.Wait();
    startupSemaphore.Close();
    return ret;
    }

//
// End of file
