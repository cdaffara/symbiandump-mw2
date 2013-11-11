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
* Description:   Client-Server communication
*
*/



#include <s32mem.h>
#include <e32std.h>

#include "epos_suplnetworkserverstart.h"
#include "epos_suplnetwork.h"
#include "epos_suplnetworkipc.h"
#include "epos_suplnetworkerrors.h"

//-----------------------------------------------------------------------------
// RSuplNetworkServer
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RSuplNetworkServer::RSuplNetworkServer()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSuplNetworkServer::RSuplNetworkServer()
  : RSessionBase(), 
    iReserved(NULL)
	{
	}

// -----------------------------------------------------------------------------
// RSuplNetworkServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void RSuplNetworkServer::ConstructL()
	{
            
	}

// ---------------------------------------------------------
// RSuplNetworkServer::Destruct
//
// Destruction method for RSuplNetworkServer. 
// ---------------------------------------------------------
//
void RSuplNetworkServer::Destruct()
	{
	}

// ---------------------------------------------------------
// RSuplNetworkServer::Connect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplNetworkServer::Connect()
	{
    __ASSERT_ALWAYS(Handle() == 0, User::Panic(KSuplNWClientFault, ESuplNWServerHandleNotClosed));
	TRAPD(ret, ConstructL());

	if (ret == KErrNone)
		{
		TInt retry = 200;
		for (;;) // FOREVER
			{
			ret = CreateSession(KSuplServerName, Version(), KDefaultMessageSlots);

			if (ret != KErrNotFound && ret != KErrServerTerminated)
				{
				break;
				}

			if (--retry == 0)
				{
				break;
				}

			ret = StartServer();

			if (ret != KErrAlreadyExists && ret != KErrNone)
				{
				break;
				}
			}
		}

	if (ret != KErrNone)
		{
		Destruct();
		}

	return ret;
	}

// ---------------------------------------------------------
// RSuplNetworkIPCSubSession::ForwardMessage
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplNetworkServer::ForwardMessage(const TDesC8& aSuplMsg)
	{
    __ASSERT_ALWAYS(Handle(), User::Panic(KSuplNWClientFault, ESuplServerNWBadHandle));            

	if(aSuplMsg.Length()==0)
	{
		return KErrArgument;
	}	
	TIpcArgs ipcArgs(&aSuplMsg);
	return SendReceive(ESuplNetworkForwardSuplMsg, ipcArgs);
	}


// ---------------------------------------------------------
// RSuplNetworkServer::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplNetworkServer::Close()
	{
    RSessionBase::Close();
	Destruct();
	}

// ---------------------------------------------------------
// RSuplNetworkServer::Version
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TVersion RSuplNetworkServer::Version() const
	{
	return TVersion(
		KSuplMajorVersionNumber,
		KSuplMinorVersionNumber,
		KSuplBuildVersionNumber);
	}

