
// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include <d32locd.h>
#include "DummyServer.h"

// The dummy server..
CDummyServer* CDummyServer::NewLC()
	{
	CDummyServer* self = new (ELeave) CDummyServer();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	

//  Second-stage constructor.
void CDummyServer::ConstructL()
	{
	StartL(KDummyServerName);
	}

CDummyServer::CDummyServer()
: CServer2(CActive::EPriorityStandard)
	{
	}

CDummyServer::~CDummyServer()
	{

	}
	
void CDummyServer::StopServer() 
	{
	CActiveScheduler::Stop();			
	}

CSession2* CDummyServer::NewSessionL(const TVersion& aVersion,const RMessage2& /*aMessage*/) const
    {
    TVersion ver(KCountServMajorVersionNumber,KCountServMinorVersionNumber,KCountServBuildVersionNumber);
    
    if (!User::QueryVersionSupported(ver,aVersion))
    	{
     	User::Leave(KErrNotSupported);  
    	}
    return new (ELeave) CDummyServerSession;
    }

CDummyServerSession::CDummyServerSession()
    {
    }
CDummyServerSession::~CDummyServerSession()
	{
	const CDummyServer* observer = static_cast<const CDummyServer*>(Server());		
	//Lets take out constness of this pointer
	CDummyServer* observer1 = const_cast<CDummyServer*>(observer);
	//Call the non-const member function
	observer1->StopServer();
	}


void CDummyServerSession::ServiceL(const RMessage2& aMessage)
    {
    
    TRAPD(err,DispatchMessageL(aMessage));
    aMessage.Complete(err);
    }


void CDummyServerSession::DispatchMessageL(const RMessage2& aMessage)
    {
    if(aMessage.Function() == EServGetTMsvEntry)
        {
        GetTMsvEntryL(aMessage);
        return;
		}
	}


// Get the TMsvEntry of the message whose TMsvId is passed from the client..
void CDummyServerSession::GetTMsvEntryL(const RMessage2& aMessage)
	{
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	
	TPckgBuf<TMsvId> idBuf;
	idBuf = aMessage.Int0();
	
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	TMsvId entryId = idBuf();
    CMsvEntry* cEntry = session->GetEntryL(entryId);
	CleanupStack::PushL(cEntry); 
	
	TPckgBuf<TMsvEntry> pckg = cEntry->Entry();
	aMessage.WriteL(1,pckg);
	
	CleanupStack::PopAndDestroy(3,ob);//cEntry,session,ob
	}


void StartDummyServerL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	
	// create the server (leave it on the cleanup stack)
	CDummyServer::NewLC();
	
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameProcess(KDummyServerName));
	
	// initialisation complete, now signal the client.
	RProcess::Rendezvous(KErrNone);
	
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(2, scheduler);	//  server, scheduler
	}

TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt err = KErrNoMemory;
	if (cleanup)
		{
		TRAP(err, StartDummyServerL());
		delete cleanup;
		}	
		
	__UHEAP_MARKEND;
	return err;
	}
	
