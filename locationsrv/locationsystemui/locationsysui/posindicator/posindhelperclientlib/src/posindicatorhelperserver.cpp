/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Positioning Indicator Helper Client-Server communication
*
*/

#include "posindicatorhelperserver.h"
#include "posindicatorhelpererrors.h"
#include "posindicatorserverconst.h"

#include <s32mem.h>
#include <e32std.h>

const TInt KDefaultMessageSlots = 255; //max

// ---------------------------------------------------------------------------
// RPosIndicatorHelperServer::RPosIndicatorHelperServer()
// Default constructor
// ---------------------------------------------------------------------------
// 
 RPosIndicatorHelperServer :: RPosIndicatorHelperServer():RSessionBase(),iReserved(NULL)
    {
     FUNC("+ RPosIndicatorHelperServer::RPosIndicatorHelperServer");
     FUNC("- RPosIndicatorHelperServer::RPosIndicatorHelperServer");
    }

// ---------------------------------------------------------
// RPosIndicatorHelperServer::Connect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 TInt RPosIndicatorHelperServer::Connect()
	{
     FUNC("+ RPosIndicatorHelperServer::Connect");
 	__ASSERT_ALWAYS(Handle() == 0, User::Panic(KPosIndHelperClientPanicCategory, EPosIndicatorHelperServerHandleNotClosed));

	TInt ret = CreateSession(KPosIndHelperSrvName, Version(), KDefaultMessageSlots);
	
	if( ret == KErrNotFound )
	    {
	    ret = StartServer();
	    if( !( ret == KErrNone || ret == KErrAlreadyExists ) )
	        {
	        return ret;
	        }
	    ret = CreateSession(KPosIndHelperSrvName, Version(), KDefaultMessageSlots);		    
	    }
   
	return ret;
   }
	
// ---------------------------------------------------------
// RPosIndicatorHelperServer::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 void RPosIndicatorHelperServer::Close()
	{
    FUNC("+ RPosIndicatorHelperServer::Close");
	RSessionBase::Close();
    FUNC("- RPosIndicatorHelperServer::Close");
	}
	
// ---------------------------------------------------------
// RPosIndicatorHelperServer::Version
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
 TVersion RPosIndicatorHelperServer::Version() const
	{	
    FUNC("+ RPosIndicatorHelperServer::Version");
	return TVersion(KMajorVersionNumber,
					KMinorVersionNumber,
					KBuildVersionNumber);
	}

// ---------------------------------------------------------
// RPosIndicatorHelperServer::StartServer
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosIndicatorHelperServer::StartServer()
    {
    FUNC("+ RPosIndicatorHelperServer::StartServer");

#if defined(__WINSCW__)
    StartLbsRootProcess();
#endif

    RProcess posIndHelperServerProcess;
    TInt ret = posIndHelperServerProcess.Create(KPosIndHelperSrvName, KNullDesC);
    
    if( ret != KErrNone )
        {
        posIndHelperServerProcess.Close();
        return ret;
        }
    
    TRequestStatus status;
    posIndHelperServerProcess.Rendezvous(status);
    
    if( status != KRequestPending )
        {
        User::WaitForRequest(status);
        posIndHelperServerProcess.Kill(KErrNone);
        posIndHelperServerProcess.Close();
        return status.Int();
        }
    else
        {
        posIndHelperServerProcess.Resume();             
        }

    User::WaitForRequest(status);
    posIndHelperServerProcess.Close();
   
    FUNC("- RPosIndicatorHelperServer::StartServer");
    return status.Int();
    }
 
#if defined(__WINSCW__)
// ---------------------------------------------------------
// RPosIndicatorHelperServer::Version
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosIndicatorHelperServer::StartLbsRootProcess()
    {
    FUNC("+ RPosIndicatorHelperServer::StartLbsRootProcess");

    RProcess lbsrootProcess;
    TInt ret = lbsrootProcess.Create(_L("lbsroot"), KNullDesC);
    
    if( ret != KErrNone )
        {
        lbsrootProcess.Close();
        return KErrNotFound;
        }
    
    TRequestStatus status;
    lbsrootProcess.Rendezvous(status);
    
    if( status != KRequestPending )
        {
        User::WaitForRequest(status);
        lbsrootProcess.Kill(KErrNone);
        lbsrootProcess.Close();
        return KErrNotFound;
        }
    else
        {
        lbsrootProcess.Resume();             
        }

    User::WaitForRequest(status);
    lbsrootProcess.Close();

    if( status != KErrNone )
        {
        return (status.Int());
        }
    return ret;
    }
#endif
