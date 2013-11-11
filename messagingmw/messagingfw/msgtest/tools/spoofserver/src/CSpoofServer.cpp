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
 
#include "cspoofserver.h"

const TInt KListeningSocketQueueSize = 1;

CSpoofServer::CSpoofServer(MImapTestEventHandler& aObserver) 
: CActive(EPriorityStandard), iObserver(aObserver)
	{
	iServerStatus=EIdle;
	CActiveScheduler::Add(this); 
	}

EXPORT_C CSpoofServer* CSpoofServer::NewL(MImapTestEventHandler& aObserver,const TDesC& aScriptFile)
	{
	CSpoofServer* self = NewLC(aObserver,aScriptFile);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CSpoofServer* CSpoofServer::NewLC(MImapTestEventHandler& aObserver,const TDesC& aScriptFile)
	{
	CSpoofServer* self = new(ELeave) CSpoofServer(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aScriptFile);
	return self;
	}
	
CSpoofServer::~CSpoofServer()
	{
	Cancel();	
	iListeningSocket.Close();
	iServiceSocket.Close();
	iSocketServer.Close();
	delete iScriptFileProcessor;
	}

EXPORT_C TInt CSpoofServer::RunError(TInt aError)
	{
	iObserver.TestComplete(aError);
	return KErrNone;
	}

void CSpoofServer::ConstructL(const TDesC& aScriptFile)
	{
	//connect to the socket server
	User::LeaveIfError(iSocketServer.Connect());
	//create the script file processor
	iScriptFileProcessor = CScriptFileProcessor::NewL(aScriptFile);	
	}

void CSpoofServer::DoCancel()
	{
	iListeningSocket.CancelAll();
	}
	
EXPORT_C void CSpoofServer::StartL(const TInt aPortNumber)
	{
	iServerStatus = EWriting;
	//set up the listening socket
	User::LeaveIfError(iListeningSocket.Open(iSocketServer, KAfInet, KSockStream, KProtocolInetTcp));
	//bind the listener
	TSockAddr address;
	address.SetPort(aPortNumber);
	iListeningSocket.Bind(address);
	//listen for incomming connections
	iListeningSocket.Listen(KListeningSocketQueueSize);
	//accept incomming connections and use iServiceSocket to communicate with it.
	User::LeaveIfError(iServiceSocket.Open(iSocketServer));
	iListeningSocket.Accept(iServiceSocket,iStatus);
	SetActive();	
	}
	
void CSpoofServer::IssueWrite()
	{			
    iBuffer.Zero();
  	iBuffer.Append(iNextLine);
  	_LIT(KEndOfLine,"\r\n");
  	iBuffer.Append(KEndOfLine);
	iServiceSocket.Write(iBuffer, iStatus);
	SetActive();
	}

void CSpoofServer::IssueRead()
	{
    iBuffer.Zero();
  	iServiceSocket.RecvOneOrMore(iBuffer, 0, iStatus,iReadLength);
  	SetActive();
	}
	
void CSpoofServer::RunL()
	{
	User::LeaveIfError(iStatus.Int());	
	
	CScriptFileProcessor::TDataDirection direction;
		
	iBuffer.TrimAll();
	if(iServerStatus==EReading)
		{
		if(iNextLine!=iBuffer)
			{
			User::Leave(KErrCorrupt);		
			}		
		}
	//read next line of script file and get its direction
	User::LeaveIfError(iScriptFileProcessor->ReadLine(iNextLine));
	direction=iScriptFileProcessor->DataDirection();
		
    //if we are sending then write the line, else read 
    if(direction==CScriptFileProcessor::EFromServer)
    	{
    	iServerStatus=EWriting;	
    	IssueWrite();
    	}
    else if(direction==CScriptFileProcessor::EFromClient)
    	{
    	iServerStatus=EReading;
    	IssueRead();
    	}    	
	}	

	


