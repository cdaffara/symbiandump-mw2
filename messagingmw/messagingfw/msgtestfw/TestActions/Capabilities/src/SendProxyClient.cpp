// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include "SendProxyClient.h"
#include <e32debug.h>


TInt StartSendProxyServer(void)
{
	const TUidType serverUid(KNullUid,KNullUid,KServerUid3);
#ifdef __SENDPROXYSERVER_NO_PROCESSES__
	//
	// In EKA1 WINS the server is a DLL, the exported entrypoint returns a TInt
	// which represents the real entry-point for the server thread
	//
	RLibrary lib;
	TInt r=lib.Load(KSendProxyServerImg,serverUid);
	if (r!=KErrNone)
		return r;
	TLibraryFunction ordinal1=lib.Lookup(1);
	TThreadFunction serverFunc=reinterpret_cast<TThreadFunction>(ordinal1());
	//
	// To deal with the unique thread (+semaphore!) naming in EPOC, and that we may
	// be trying to restart a server that has just exited we attempt to create a
	// unique thread name for the server.
	// This uses Math::Random() to generate a 32-bit random number for the name
	//
	TName name(KSendProxyServerName);
	name.AppendNum(Math::Random(),EHex);
	RThread server;
	r=server.Create(name,serverFunc,
					KSendProxyServerStackSize,
					NULL,&lib,NULL,
					KSendProxyServerInitHeapSize,KSendProxyServerMaxHeapSize,EOwnerProcess);
	lib.Close();	// if successful, server thread has handle to library now
#else
	//
	// EPOC and EKA2 is easy, we just create a new server process. Simultaneous
	// launching of two such processes should be detected when the second one
	// attempts to create the server object, failing with KErrAlreadyExists.
	//
	RProcess server;
	TInt r=server.Create(KSendProxyServerImg,KNullDesC,serverUid);
	
	
#endif
	if (r!=KErrNone)
	{
		return r;
	}

	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
		server.Kill(0);		// abort startup
	else
		server.Resume();	// logon OK - start the server
	User::WaitForRequest(stat);		// wait for start or death
	// we can't use the 'exit reason' if the server panicked as this
	// is the panic 'reason' and may be '0' which cannot be distinguished
	// from KErrNone

	r=(server.ExitType()==EExitPanic) ? KErrGeneral : stat.Int();
	server.Close();
	return r;
	
}


EXPORT_C TInt RProxyServerSession::Connect()
//
// Connect to the server, attempting to start it if necessary
//
	{
	TInt retry=2;
	for (;;)
		{
		TInt r=CreateSession(KSendProxyServerName,TVersion(0,0,0),1);
		if (r!=KErrNotFound && r!=KErrServerTerminated)
			return r;
		if (--retry==0)
			return r;
		r=StartSendProxyServer();
		if (r!=KErrNone && r!=KErrAlreadyExists)
			return r;
		}
	}



EXPORT_C TInt RProxyServerSession::SendReceive(TInt aCommand, const TIpcArgs& aTIpcArgs)
	{    
	
	            
	CPackServerData packServerData(aCommand, &aTIpcArgs);
	TPckg<CPackServerData> pack(packServerData);
	TIpcArgs tipcArgs(ESend, aTIpcArgs.iFlags);
	
	TInt err = RSessionBase::SendReceive(ESendData ,  tipcArgs);
		// We need to indicate how the buffers will be created and these are indicated by the 
		// flags in aTIpcArgs. Should never fail to get this info across.
	if (err<0)
		{
		return err;
		}
	
	return RSessionBase::SendReceive(aCommand , aTIpcArgs );
		// Same as parent call, except that we have got the data to build the dummy args in the proxy.

	}


EXPORT_C TInt RProxyServerSession::SendReceiveProxyAsync(TInt aCommand, const TIpcArgs& aTIpcArgs )
	{    
		            
	CPackServerData packServerData(aCommand, &aTIpcArgs);
	TPckg<CPackServerData> pack(packServerData);
	TIpcArgs tipcArgs(ESendAsyncWait, aTIpcArgs.iFlags);
	
	TInt err = RSessionBase::SendReceive(ESendData ,  tipcArgs);
		// We need to indicate how the buffers will be created and these are indicated by the 
		// flags in aTIpcArgs. Should never fail to get this info across.
	if (err<0)
		{
		return err;
		}
	
	return RSessionBase::SendReceive(aCommand , aTIpcArgs );
		// Same as parent call, except that we have got the data to build the dummy args in the proxy.	            

	}

	

EXPORT_C TInt RProxyServerSession::SendReceive(TInt aCommand)
{
	
	TIpcArgs aTIpcArgs;
	
	CPackServerData packServerData(aCommand, &aTIpcArgs);
	TPckg<CPackServerData> pack(packServerData);
	TIpcArgs tipcArgs(ESend, aTIpcArgs.iFlags);
	
	TInt err = RSessionBase::SendReceive(ESendData ,  tipcArgs);
		// We need to indicate how the buffers will be created and these are indicated by the 
		// flags in aTIpcArgs. Should never fail to get this info across.
	if (err<0)
		{
		return err;
		}
	
	return RSessionBase::SendReceive(aCommand , aTIpcArgs );
		// Same as parent call, except that we have got the data to build the dummy args in the proxy.	
	
}



EXPORT_C TInt RProxyServerSession::SendReceiveProxyAsync(TInt aCommand )
	{    
	TIpcArgs aTIpcArgs;

	CPackServerData packServerData(aCommand, &aTIpcArgs);
	TPckg<CPackServerData> pack(packServerData);
	TIpcArgs tipcArgs(ESendAsyncWait, aTIpcArgs.iFlags);
	
	TInt err = RSessionBase::SendReceive(ESendData ,  tipcArgs);
		// We need to indicate how the buffers will be created and these are indicated by the 
		// flags in aTIpcArgs. Should never fail to get this info across.
	
	if (err<0)
		{
		return err;
		}
	
	return RSessionBase::SendReceive(aCommand , aTIpcArgs );
		// Same as parent call, except that we have got the data to build the dummy args in the proxy.	            

	
	}



EXPORT_C TInt RProxyServerSession::GetServerSecureId( TSecureId& p )
	{	
	TPckg<TSecureId> buf(p);	
	TIpcArgs send(&buf);
				
	TInt ret= RSessionBase::SendReceive(EGetServerSecureId, send); 
	
	return ret;	
	}



EXPORT_C TInt RProxyServerSession::GetServerCapabilities( TInt32& p )
	{	
	TPckg<TInt32> buf(p);	
	TIpcArgs send(&buf);
				
	TInt ret= RSessionBase::SendReceive(EGetServerCapabilities, send); 
	
	return ret;	
	}


