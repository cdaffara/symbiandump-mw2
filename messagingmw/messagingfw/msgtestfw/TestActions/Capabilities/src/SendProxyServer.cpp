// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// server.cpp
// Transient server example - server implementation
// 
//

#include "SendProxyserver.h"
#include "MSERVER.H"



inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}
	
	
inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}
	
	
inline void CShutdown::Start()
	{After(KSendProxyShutdownDelay);}


inline CSendProxyServer::CSendProxyServer()
	:CServer2(0,ESharableSessions)
	{}



EXPORT_C TInt RMessage2Derv::GetiArgs(TInt aPos) const
{
	return iArgs[aPos];
}




inline CSendProxyServerSession::CSendProxyServerSession() : iSessionCount(0), iCommand( ENoCommandPending )
	{}
		
	
inline CSendProxyServer& CSendProxyServerSession::Server()
	{return *static_cast<CSendProxyServer*>(const_cast<CServer2*>(CSession2::Server()));}
	
	
inline TBool CSendProxyServerSession::ReceivePending() const
	{return !iReceiveMsg.IsNull();}



TInt RMsvServerSessionTest::SendReceive( TInt aFunction,const TIpcArgs& aArgs ) const
	{ 
	return RMsvServerSession::SendReceive( aFunction,aArgs ); 
	}
	
TInt RMsvServerSessionTest::SendReceive( TInt aFunction ) const
	{ 
	return RMsvServerSession::SendReceive( aFunction ); 
	}
	// All we want is to access. Since RSessionBase::SendReceive is protected so is RMsvServerSession::SendReceive
	// So as long as we derive from RSessionBase base we should be get access to SendReceive.


void RMsvServerSessionTest::SendReceive( TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aRequestStatus ) const
	{ 
	RMsvServerSession::SendReceive( aFunction,aArgs,aRequestStatus ); 		
	}
	
void RMsvServerSessionTest::SendReceive( TInt aFunction,TRequestStatus& aRequestStatus) const
	{ 
	return RMsvServerSession::SendReceive( aFunction,aRequestStatus ); 
	}


void RMsvServerSessionTest::SendReceiveAsync( TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aRequestStatus ) const
	{ 
	RMsvServerSession::SendReceive( aFunction,aArgs,aRequestStatus ); 		
	User::WaitForRequest(aRequestStatus);		
		// Lets pretend sync for simplicity sake.
	}
	
void RMsvServerSessionTest::SendReceiveAsync( TInt aFunction,TRequestStatus& aRequestStatus) const
	{ 
	RMsvServerSession::SendReceive( aFunction,aRequestStatus ); 
	User::WaitForRequest(aRequestStatus);
		// Lets pretend sync for simplicity sake.
	}


TInt RMsvServerSessionTest::CreateSession(const TDesC& aServer,const TVersion& aVersion,TInt aAsyncMessageSlots)
	{
	return RMsvServerSession::CreateSession(aServer, aVersion, aAsyncMessageSlots);
	}



//


TInt StartMsvServer()
{

	TInt err =  KErrNone ;
	// EKA2 is simple No path required 
	TBuf<32> serverFile;
	
	serverFile.Copy(_L("!") );
	serverFile.Copy( KMsvServerNameExe );
	
	_LIT(KExe,".exe");
	serverFile.Append(KExe);
	RProcess server;
	err = server.Create(serverFile,_L(""));
	if(err != KErrNone)
		return err;
		
	// Synchronise with the server
	TRequestStatus reqStatus;
	server.Rendezvous(reqStatus);
	server.Resume();
	 //Server will call the reciprocal static synchronise call
	User::WaitForRequest(reqStatus);
	//server.Close();
	if(reqStatus.Int() != KErrNone)
	{
		return reqStatus.Int();
	}
	
	return err;
}	


void CSendProxyServerSession::CreateL()
//
// 2nd phase construct for sessions - called by the CServer framework
//
	{	
	
	// Start up the messaging server that all messages will be forwarded to. 
	TInt error = iMsvServerSession.CreateSession( KMsvServerName, iMsvServerSession.Version(), 2);
	if (error==KErrNone)
		{ // Session has been started. Server was already running.
		}
	else
		{
		error=StartMsvServer();
		
		if (error!=KErrNone)
			{ // Leave
			User::LeaveIfError(error);
			}
		else
			{
			
			TInt error = iMsvServerSession.CreateSession( KMsvServerName, iMsvServerSession.Version(), 2);
			if (error!=KErrNone)
				{ // Leave					
				User::LeaveIfError(error);
				}
			}
		}		

	
	Server().AddSession();
	
	}


CSendProxyServerSession::~CSendProxyServerSession()
	{	
	iMsvServerSession.Close();
	Server().DropSession();		
	}



void CSendProxyServerSession::ServiceL(const RMessage2& aMessage)
//
// Handle a client request.
// Leaving is handled by CSendProxyServer::ServiceError() which reports
// the error code to the client
//
	{
	TInt messageFunction = aMessage.Function();
	
	if (messageFunction==ESendData)
		{	// We are setting up buffers as next time we actually execute the command.
			// _ASSERT( iCommand == ENoCommandPending );
			// Make sure we do not have another command pending.
		iCommand = (TSendProxyMessages) aMessage.Int0();  //???? fix casts  
		iFlags = aMessage.Int1();
		aMessage.Complete( KErrNone );		
		}
	else if (messageFunction==EGetServerSecureId)
		{ // This must done for the proxy server. 
		  // We are not passing the data on to the messaging server

		TSecureId cServerSecureId;
		TPckg<TSecureId> buf(cServerSecureId);
		
		aMessage.ReadL(0,buf);
			
		cServerSecureId = RProcess().SecureId();
		aMessage.WriteL(0,buf); 
								
		aMessage.Complete( KErrNone );
		}
	else if (messageFunction==EGetServerCapabilities)
		{ // return the capabilities for the proxy server. 
		  // We are not passing the data on to the messaging server

		TInt32 cServerCaps;
		TPckg<TInt32> buf(cServerCaps);
		
		aMessage.ReadL(0,buf);
		
		ReadCapabilities(cServerCaps);

		aMessage.WriteL(0,buf); 
								
		aMessage.Complete( KErrNone );
		}
	else if ( messageFunction==EReceive )
		{ 
		if (ReceivePending())
			PanicClient(aMessage,EPanicAlreadyReceiving);
		else
			{
			iReceiveMsg=aMessage;
			iReceiveLen=aMessage.Int1();
			}
		}
	else if ( messageFunction==ECancelReceive )
		{				
		if (ReceivePending())
			iReceiveMsg.Complete(KErrCancel);
		aMessage.Complete( KErrNone );
		}
	else
		{  // We are expected to execute a command.
		TInt complete = DoCommandL( (const RMessage2Derv&) aMessage );		

		// Indicate that we succesfully executed a full command and can take another.			
		iCommand = ENoCommandPending;
		iFlags = 0;
		aMessage.Complete( complete );
		}	
		
	}


void CSendProxyServerSession::ReadCapabilities(TInt32& aServerCaps)
	{
	aServerCaps=0;

	if (RProcess().HasCapability(ECapabilityTCB))
		aServerCaps |= (1<<ECapabilityTCB);
	
	if (RProcess().HasCapability(ECapabilityCommDD))
		aServerCaps |= (1<<ECapabilityCommDD);
	
	if (RProcess().HasCapability(ECapabilityPowerMgmt))
		aServerCaps |= (1<<ECapabilityPowerMgmt);
	
	if (RProcess().HasCapability(ECapabilityMultimediaDD))
		aServerCaps |= (1<<ECapabilityMultimediaDD);

	if (RProcess().HasCapability(ECapabilityReadDeviceData))
		aServerCaps |= (1<<ECapabilityReadDeviceData);
	
	if (RProcess().HasCapability(ECapabilityWriteDeviceData))
		aServerCaps |= (1<<ECapabilityWriteDeviceData);

	if (RProcess().HasCapability(ECapabilityDRM))
		aServerCaps |= (1<<ECapabilityDRM);
	
	if (RProcess().HasCapability(ECapabilityTrustedUI))
		aServerCaps |= (1<<ECapabilityTrustedUI);

	if (RProcess().HasCapability(ECapabilityProtServ))
		aServerCaps |= (1<<ECapabilityProtServ);

	if (RProcess().HasCapability(ECapabilityDiskAdmin))
		aServerCaps |= (1<<ECapabilityDiskAdmin);

	if (RProcess().HasCapability(ECapabilityNetworkControl))
		aServerCaps |= (1<<ECapabilityNetworkControl);

	if (RProcess().HasCapability(ECapabilityAllFiles))
		aServerCaps |= (1<<ECapabilityAllFiles);

	if (RProcess().HasCapability(ECapabilitySwEvent))
		aServerCaps |= (1<<ECapabilitySwEvent);

	if (RProcess().HasCapability(ECapabilityNetworkServices))
		aServerCaps |= (1<<ECapabilityNetworkServices);

	if (RProcess().HasCapability(ECapabilityLocalServices))
		aServerCaps |= (1<<ECapabilityLocalServices);

	if (RProcess().HasCapability(ECapabilityReadUserData))
		aServerCaps |= (1<<ECapabilityReadUserData);

	if (RProcess().HasCapability(ECapabilityWriteUserData))
		aServerCaps |= (1<<ECapabilityWriteUserData);

	if (RProcess().HasCapability(ECapabilityLocation))
		aServerCaps |= (1<<ECapabilityLocation);

	if (RProcess().HasCapability(ECapabilitySurroundingsDD))
		aServerCaps |= (1<<ECapabilitySurroundingsDD);

	if (RProcess().HasCapability(ECapabilityUserEnvironment))
		aServerCaps |= (1<<ECapabilityUserEnvironment);

		
	}


//*************


void CSendProxyServerSession::SetupTIpcArgL(const RMessage2Derv& aMessage, TInt aPos, TIpcArgs& atipcArgs )
	{		
	const TInt mask = ( 1<<TIpcArgs::KBitsPerType )-1;
	
	const TInt bits_shift  = (aPos * TIpcArgs::KBitsPerType );
	const TInt type_flags = ( iFlags >> bits_shift ) & mask ;
	
		
	if ( type_flags == TIpcArgs::EUnspecified )
		{ // Unspecified so must be TInt.		
		TInt tmp = aMessage.GetiArgs(aPos);
		atipcArgs.Set( aPos, (TAny*) tmp);
		}
	else if ( type_flags  == TIpcArgs::EDesC8 )
		{ // We have a read only descriptor can not use GetDesMaxLength on it. 
		
		TInt maxLength1 = aMessage.GetDesLength(aPos);
		HBufC8* buffer1 = HBufC8::NewL(maxLength1);
		iBuffer8[aPos].Assign(buffer1);
			// Owns the buffer.

		TPtr8 ptrBuffer1= (buffer1->Des());						
		
		aMessage.ReadL( aPos, ptrBuffer1 );		
		atipcArgs.Set( aPos, buffer1);
			// buffer is considered constant so internal flags in atipcArgs will be okay.
		}
	else if ( type_flags == TIpcArgs::EDes8 )
		{ // Just generate a descriptor and copy the contents into it.
		  // We need GetDesMaxLength because it is read/write and somebody will right more than the current length.

		TInt maxLength2 = aMessage.GetDesMaxLength(aPos);		
		 
		HBufC8* buffer2 = HBufC8::NewL(maxLength2);
		iBuffer8[aPos].Assign(buffer2);
			// we want the buffer to read/write which is why we use RBuf8.
			// The RBuf8 takes ownership of the HBuf.
		
		iTPtr8[aPos] = new (ELeave) TPtr8(buffer2->Des());
								
		aMessage.ReadL( aPos, *(iTPtr8[aPos]) );			
		atipcArgs.Set( aPos, iTPtr8[aPos] );		
			
		}
	else if (  type_flags == TIpcArgs::EDesC16 )
		{ // We have a read only descriptor can not use GetDesMaxLength on it. 
		
		TInt maxLength3 = aMessage.GetDesLength(aPos);
		HBufC16* buffer3 = HBufC16::NewL(maxLength3);
		iBuffer16[aPos].Assign(buffer3);
			// Owns the buffer.

		TPtr16 ptrBuffer3= (buffer3->Des());						
		
		aMessage.ReadL( aPos, ptrBuffer3 );		
		atipcArgs.Set( aPos, buffer3);
			// buffer is considered constant so internal flags in atipcArgs will be okay.
		}
	else if ( type_flags == TIpcArgs::EDes16 )
		{ // Just generate a descriptor and copy the contents into it.
		  // We need GetDesMaxLength because it is read/write and somebody will right more than the current length.

		TInt maxLength4 = aMessage.GetDesMaxLength(aPos);		
		 
		HBufC16* buffer4 = HBufC16::NewL(maxLength4);
		iBuffer16[aPos].Assign(buffer4);
			// we want the buffer to read/write which is why we use RBuf16.
			// The RBuf16 takes ownership of the HBuf.
		
		iTPtr16[aPos] = new (ELeave) TPtr16(buffer4->Des());
								
		aMessage.ReadL( aPos, *(iTPtr16[aPos]) );			
		atipcArgs.Set( aPos, iTPtr16[aPos] );		
			
		}
	else if ( type_flags == TIpcArgs::EHandle )
		{		
		TInt tmp = aMessage.GetiArgs(aPos);
		atipcArgs.Set( aPos, tmp);
		}		
	else
		{ // Unknown error.
		User::LeaveIfError(KErrGeneral);
		}
	
	}

void CSendProxyServerSession::SetupTIpcArgsL(const RMessage2Derv& aMessage, TIpcArgs& atipcArgs)
	{	
	SetupTIpcArgL( aMessage, 0, atipcArgs );
	SetupTIpcArgL( aMessage, 1, atipcArgs );		
	SetupTIpcArgL( aMessage, 2, atipcArgs );		
	SetupTIpcArgL( aMessage, 3, atipcArgs );			
	}


//*************

void CSendProxyServerSession::SetupReturnTIpcArgL(const RMessage2Derv& aMessage, TInt aPos )
	{		
	const TInt mask = ( 1<<TIpcArgs::KBitsPerType )-1;
	
	const TInt bits_shift  = (aPos * TIpcArgs::KBitsPerType );
	const TInt type_flags = ( iFlags >> bits_shift ) & mask ;
		
	if (type_flags == TIpcArgs::EUnspecified)
		{ // Do nothing can not write back to the args of a message.
		}
	else if ( type_flags == TIpcArgs::EDesC8 )
		{ // We have constant descriptor no action required as we can not change it.
		}
	else if ( type_flags == TIpcArgs::EDes8 )
		{ // We have a read/write descriptor we need to copy back any changes.
		aMessage.WriteL( aPos, *(iTPtr8[aPos]) );		
		}
	else if ( type_flags == TIpcArgs::EDesC16 )
		{ // We have constant descriptor no action required as we can not change it.
		}
	else if ( type_flags  == TIpcArgs::EDes16 )
		{ // We have a read/write descriptor we need to copy back any changes.
		aMessage.WriteL( aPos, *(iTPtr16[aPos]) );		
		}
	else if ( type_flags == TIpcArgs::EHandle )
		{ //Do nothing as handles should not change.
		}
	else
		{ // may need to implement 16 bit descriptors etc.
		User::LeaveIfError(KErrGeneral);
		}
	
	}


void CSendProxyServerSession::SetupReturnTIpcArgsL(const RMessage2Derv& aMessage)
	{	
	SetupReturnTIpcArgL( aMessage, 0 );
	SetupReturnTIpcArgL( aMessage, 1 );		
	SetupReturnTIpcArgL( aMessage, 2 );		
	SetupReturnTIpcArgL( aMessage, 3 );			
	}


void CSendProxyServerSession::ClearBuffer(TInt aPos)
	{
	
	iBuffer8[aPos].Close();
	delete iTPtr8[aPos];
	iTPtr8[aPos]=0;

	iBuffer16[aPos].Close();
	delete iTPtr16[aPos];
	iTPtr16[aPos]=0;
		
	}



void CSendProxyServerSession::ClearBuffers()
	{	
	
	ClearBuffer( 0 );
	ClearBuffer( 1 );		
	ClearBuffer( 2 );		
	ClearBuffer( 3 );		
	
	}


TInt CSendProxyServerSession::DoCommandL(const RMessage2Derv& aMessage ) 
	{
	TInt complete=KErrNone;	
			
	switch (iCommand)
		{
		
		case ESend:
			{		
			
			TIpcArgs tipcArgs;
			SetupTIpcArgsL( ( (const RMessage2Derv&) aMessage ), tipcArgs);			
			complete = iMsvServerSession.SendReceive( aMessage.Function(), tipcArgs );			
			SetupReturnTIpcArgsL( (const RMessage2Derv&) aMessage);
			ClearBuffers();
			break;
			}
						
		case ESendAsyncWait:
			{				

			TIpcArgs tipcArgs1;
			SetupTIpcArgsL( ( (const RMessage2Derv&) aMessage ), tipcArgs1);			
			
			TRequestStatus requestStatus=KRequestPending;
			iMsvServerSession.SendReceive( aMessage.Function(), tipcArgs1 , requestStatus );
			User::WaitForRequest(requestStatus);			
			SetupReturnTIpcArgsL( (const RMessage2Derv&) aMessage);
			ClearBuffers();
			complete = requestStatus.Int();

			break;
			}
		default:
			{
			complete=KErrGeneral;
			PanicClient(aMessage,EPanicIllegalFunction);			
			break;
			}
		}
				
	return complete;			
	}

void CSendProxyServerSession::ServiceError(const RMessage2& aMessage,TInt aError)
//
// Handle an error from CSendProxyServerSession::ServiceL()
// A bad descriptor error implies a badly programmed client, so panic it;
// otherwise use the default handling (report the error to the client)
//
	{
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,EPanicBadDescriptor);
	CSession2::ServiceError(aMessage,aError);
	}

void CShutdown::RunL()
//
// Initiate server exit when the timer expires
//
	{
	CActiveScheduler::Stop();
	}

CServer2* CSendProxyServer::NewLC()
	{
	CSendProxyServer* self=new(ELeave) CSendProxyServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


//**********************************

void CSendProxyServer::ConstructL()
//
// 2nd phase construction - ensure the timer and server objects are running
//
	{
	StartL(KSendProxyServerName);
			
	iShutdown.ConstructL();
		// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();	
	}


CSession2* CSendProxyServer::NewSessionL(const TVersion&,const RMessage2&) const
//
// Cretae a new client session. This should really check the version number.
//
	{
	return new(ELeave) CSendProxyServerSession();
	}

void CSendProxyServer::AddSession()
//
// A new session is being created
// Cancel the shutdown timer if it was running
//
	{
	++iSessionCount;
	iShutdown.Cancel();
	}

void CSendProxyServer::DropSession()
//
// A session is being destroyed
// Start the shutdown timer if it is the last session.
//
	{
	if (--iSessionCount==0)
		iShutdown.Start();
	}



void PanicClient(const RMessagePtr2& aMessage,TSendProxyServerPanic aPanic)
//
// RMessagePtr2::Panic() also completes the message. This is:
// (a) important for efficient cleanup within the kernel
// (b) a problem if the message is completed a second time
//
	{
	_LIT(KPanic,"SendProxyServer");
	aMessage.Panic(KPanic,aPanic);
	}


//******************************


static void RunServerL()
//
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//
	{
		
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameThread(KSendProxyServerName));
	//
	// create and install the active scheduler we need
	CActiveScheduler* s=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(s);
	CActiveScheduler::Install(s);
	//
	// create the server (leave it on the cleanup stack)
	CSendProxyServer::NewLC();
	//
	// Initialisation complete, now signal the client
#ifdef __SENDPROXYSERVER_NO_PROCESSES__
	RThread::Rendezvous(KErrNone);
#else
	RProcess::Rendezvous(KErrNone);
#endif
	//
	// Ready to run
	CActiveScheduler::Start();
	//
	// Cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2);

	}



//****************************************


TInt E32Main()
//
// Server process entry-point
//
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r,RunServerL());
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
	return r;
	}

#ifdef __SENDPROXYSERVER_NO_PROCESSES__

// The server binary is an "EPOCEXE" target type
// Thus the server parameter passing and startup code for WINS and EPOC are
// significantly different.
//
// In EKA1 WINS, the EPOCEXE target is a DLL with an entry point called WinsMain,
// taking no parameters and returning TInt. This is not really valid as a thread
// function which takes a TAny* parameter which we need.
//
// So the DLL entry-point WinsMain() is used to return a TInt representing the
// real thread function within the DLL. This is good as long as
// sizeof(TInt)>=sizeof(TThreadFunction).
//

static TInt ThreadFunction(TAny*)
//
// WINS thread entry-point function.
//
	{
	return E32Main();
	}

IMPORT_C TInt WinsMain();
EXPORT_C TInt WinsMain()
//
// WINS DLL entry-point. Just return the real thread function 
// cast to TInt
//
	{
	return reinterpret_cast<TInt>(&ThreadFunction);
	}

TInt E32Dll(TDllReason)
	{
	return KErrNone;
	}

#endif
