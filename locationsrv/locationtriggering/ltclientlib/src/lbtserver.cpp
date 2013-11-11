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
* Description:  Client-Server communication
*
*/


#include <s32mem.h>
#include <e32std.h>
#include "lbtserver.h"
#include "lbterrors.h"
#include "lbtserverconsts.h"


const TInt KDefaultMessageSlots = 16;
_LIT(KServerCoreProcessName, "lbtserver.exe");

// ---------------------------------------------------------------------------
// RLbtServer::RLbtServer()
// Default constructor
// ---------------------------------------------------------------------------
// 
EXPORT_C RLbtServer :: RLbtServer():RSessionBase(),iReserved(NULL)
    {

    }

// -----------------------------------------------------------------------------
// RLbtServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void RLbtServer::ConstructL()
	{
  
	}
	
// ---------------------------------------------------------
// RLbtServer::Connect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RLbtServer::Connect()
	{
 	__ASSERT_ALWAYS(Handle() == 0, User::Panic(KLbtClientPanicCategory, ELbtServerHandleNotClosed));
	
	TRAPD(ret, ConstructL());

	if (ret == KErrNone)
		{
		ret = CreateSession(KLbtServerCoreName, Version(), KDefaultMessageSlots);
		
		if( ret == KErrNotFound )
		    {
		    RProcess lbtServer;
		    ret = lbtServer.Create(KServerCoreProcessName, KNullDesC);
		    
		    if( ret != KErrNone )
		        {
		        lbtServer.Close();
		        return ret;
		        }
		    
		    TRequestStatus status;
		    lbtServer.Rendezvous(status);
		    
		    if( status != KRequestPending )
		        {
		        User::WaitForRequest(status);
		        lbtServer.Kill(KErrNone);
		        lbtServer.Close();
		        return KErrNotFound;
		        }
		    else
		        {
		        lbtServer.Resume();		        
		        }

		    User::WaitForRequest(status);
		    lbtServer.Close();
		    ret = status.Int();

		    if( !( ret == KErrNone || ret == KErrAlreadyExists ) )
		        {
		        return ret;
		        }

		    ret = CreateSession(KLbtServerCoreName, Version(), KDefaultMessageSlots);		    
		    }
        }
	return ret;
    }
	
// ---------------------------------------------------------
// RLbtServer::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbtServer::Close()
	{
	RSessionBase::Close();
	}
	
// ---------------------------------------------------------
// RLbtServer::Version
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TVersion RLbtServer::Version() const
	{	
	return TVersion(KLbtMajorVersionNumber,
					KLbtMinorVersionNumber,
					KLbtBuildVersionNumber);
	}
