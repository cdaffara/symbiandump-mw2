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
// MsgTestUtilsServer.cpp
// MessagingTestUtilityServer implementation
// 
//

#include <e32std.h>
#include "MessagingTestUtilityServer.h"
#include "messagingtestutility.h"

//DoDeleteMessageStore headers:
#include "TestUtilityServerObserver.h"

//DoCopyFile headers:
#include <bautils.h>


inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}
inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}
inline void CShutdown::Start()
	{After(KMyShutdownDelay);}

inline CMessagingTestUtilityServer::CMessagingTestUtilityServer()
	:CServer2(0,ESharableSessions)
	{}

inline CMessagingTestUtilitySession::CMessagingTestUtilitySession()
	{}
inline CMessagingTestUtilityServer& CMessagingTestUtilitySession::Server()
	{return *static_cast<CMessagingTestUtilityServer*>(const_cast<CServer2*>(CSession2::Server()));}


void PanicClient(const RMessage2& aMessage,TTestPanic aPanic)
//
// RMessage::Panic() also completes the message. This is:
// (a) important for efficient cleanup within the kernel
// (b) a problem if the message is completed a second time
//
	{
	_LIT(KPanic,"MessagingTestUtilityServer");
 	aMessage.Panic(KPanic,aPanic);
	}

void CMessagingTestUtilitySession::CreateL()
//
// 2nd phase construct for sessions - called by the CServer framework
//
	{
	Server().AddSession();
	
	User::LeaveIfError(iFs.Connect());
	}

CMessagingTestUtilitySession::~CMessagingTestUtilitySession()
	{
	Server().DropSession();
	
	iFs.Close();
	delete iDir;
	}


void CMessagingTestUtilitySession::ServiceL(const RMessage2& aMessage)
//
// Entry point for when a new message is received
//
	{
	TInt result = KErrNone;

	switch (aMessage.Function())
		{
	case EDeleteMessageStore:
		result = DoDeleteMessageStoreL(aMessage);
		break;
	case EKillProcess:
		result = DoKillProcessL(aMessage);
		break;
	case ECopyFile:
		result = DoCopyFileL(aMessage);
		break;
	case EDeleteFile:
		result = DoDeleteFileL(aMessage);
		break;
	case EMkDir:
		result = DoMkDirL(aMessage);
		break;
	case ERmDir:
		result = DoRmDirL(aMessage);
		break;
	case EGetDir:
		result = DoGetDirL(aMessage);
		break;
	case EGetDirCount:
		result = DoGetDirCountL(aMessage);
		break;
	case EGetDirEntry:
		result = DoGetDirEntryL(aMessage);
		break;
	case EFileExists:
		result = DoFileExistsL(aMessage);
		break;
	default:
		PanicClient(aMessage,ETestPanicIllegalFunction);
		break;
		}
	aMessage.Complete(result);
	}

void CMessagingTestUtilitySession::ServiceError(const RMessage2& aMessage,TInt aError)
//
// Handle an error from CMySession::ServiceL()
// A bad descriptor error implies a badly programmed client, so panic it;
// otherwise use the default handling (report the error to the client)
//
	{
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,ETestPanicBadDescriptor);
	CSession2::ServiceError(aMessage,aError);
	}

void CShutdown::RunL()
//
// Initiate server exit when the timer expires
//
	{
	CActiveScheduler::Stop();
	}

CServer2* CMessagingTestUtilityServer::NewLC()
	{
	CMessagingTestUtilityServer* self=new(ELeave) CMessagingTestUtilityServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMessagingTestUtilityServer::ConstructL()
//
// 2nd phase construction - ensure the timer and server objects are running
//
	{
	StartL(KTestServerName);
	iShutdown.ConstructL();
	// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();
	}


CSession2* CMessagingTestUtilityServer::NewSessionL(const TVersion&,const RMessage2&) const
//
// Create a new client session. This should really check the version number.
//
	{
	return new(ELeave) CMessagingTestUtilitySession();
	}

void CMessagingTestUtilityServer::AddSession()
//
// A new session is being created
// Cancel the shutdown timer if it was running
//
	{
	++iSessionCount;
	iShutdown.Cancel();
	}

void CMessagingTestUtilityServer::DropSession()
//
// A session is being destroyed
// Start the shutdown timer if it is the last session.
//
	{
	if (--iSessionCount==0)
		iShutdown.Start();
	}
	
//
//Functions to service calls to the Messaging Test Utility Server:

TInt CMessagingTestUtilitySession::DoDeleteMessageStoreL(const RMessage2& aMessage)
	{
	TPckgBuf<TMsvId> msvIdBuf;
	aMessage.ReadL(0,msvIdBuf);
	TMsvId msvId;	
	msvId = msvIdBuf();
	
	CTestUtilityServerObserver* ob = CTestUtilityServerObserver::NewLC();
	
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, msvId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TInt ret = KErrNone;
/*
	if (cEntry->HasStoreL())
		{
		CMsvStore* store = cEntry->EditStoreL();
		store->DeleteL();
//		test(cEntry->HasStoreL()==EFalse);
		}
	else
		{
		ret = KErrInUse;
		}
*/		
	CMsvStore* store = cEntry->EditStoreL();
	store->DeleteL();
	
	delete store;

	CleanupStack::PopAndDestroy(3); //cEntry, session, ob.
	
	return ret;
	}
	
TInt CMessagingTestUtilitySession::DoKillProcessL(const RMessage2& aMessage)
	{
	TPckgBuf<TProcessId> procIdBuf;
	aMessage.ReadL(0,procIdBuf);
	TProcessId procId;	
	procId = procIdBuf();
	
	RProcess process;
	
	TInt ret;
	
	ret = process.Open(procId);
	
	if (ret != KErrNone)
		return ret;
	
	if (User::ProcessCritical() != User::ENotCritical)
		ret = KErrPermissionDenied;
	else
		process.Kill(KErrNone);
	
	process.Close();
	
	return ret;
	}
	
TInt CMessagingTestUtilitySession::DoCopyFileL(const RMessage2& aMessage)
	{
	TBuf<255> srcPath, destPath;
	aMessage.ReadL(0, srcPath);
	aMessage.ReadL(1, destPath);
	
	TInt ret = BaflUtils::CopyFile(iFs, srcPath, destPath);
	
	return ret;
	}
	
TInt CMessagingTestUtilitySession::DoDeleteFileL(const RMessage2& aMessage)
	{
	TBuf<255> filePath;
	aMessage.ReadL(0, filePath);
		
	TInt ret = iFs.Delete(filePath);
		
	return ret;
	}
	
TInt CMessagingTestUtilitySession::DoMkDirL(const RMessage2& aMessage)
	{
	TBuf<255> dirPath;
	aMessage.ReadL(0, dirPath);
		
	TInt ret = iFs.MkDirAll(dirPath);
		
	return ret;
	}

TInt CMessagingTestUtilitySession::DoRmDirL(const RMessage2& aMessage)
	{
	TBuf<255> dirPath;
	aMessage.ReadL(0, dirPath);
		
	TInt ret = iFs.RmDir(dirPath);
		
	return ret;
	}

TInt CMessagingTestUtilitySession::DoGetDirL(const RMessage2& aMessage)
	{
	TBuf<255> dirPath;
	aMessage.ReadL(0, dirPath);
	
	TPckgBuf<TUint> entryAttMaskBuf;
	aMessage.ReadL(1,entryAttMaskBuf);
	TUint entryAttMask = entryAttMaskBuf();

	TPckgBuf<TUint> entrySortKeyBuf;
	aMessage.ReadL(2,entrySortKeyBuf);
	TUint entrySortKey = entrySortKeyBuf();
	
	delete iDir; //clean up first.
	
	// Hold the CDir* in the Session object.
	// Used by calls to DoGetDirCount() and DoGetDirEntry().	
	TInt ret = iFs.GetDir(dirPath, entryAttMask, entrySortKey, iDir);
	
	return ret;
	}

TInt CMessagingTestUtilitySession::DoGetDirCountL(const RMessage2& aMessage)
	{
	TUint count =0;
	
	if(iDir != 0)
	{
		count = iDir->Count();
		
		TPckgBuf<TUint> countBuf(count);
		aMessage.WriteL(0, countBuf);
		return KErrNone;
	}
	
	return KErrNotFound;
	}

TInt CMessagingTestUtilitySession::DoGetDirEntryL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> indexBuf;
	aMessage.ReadL(0,indexBuf);
	TInt index = indexBuf();
	
	TEntry entry;
	
	if(iDir != 0)
	{
		entry = iDir->operator[](index);

		TPckgBuf<TEntry> entryBuf(entry);		
		aMessage.WriteL(1, entryBuf);
		return KErrNone;
	}
	
	return KErrNotFound;	
	}
	
TBool CMessagingTestUtilitySession::DoFileExistsL(const RMessage2& aMessage)
	{
	TBuf<255> filePath;
	aMessage.ReadL(0, filePath);
	
	TBool ret = BaflUtils::FileExists(iFs, filePath);
		
	return ret;
	}
	
//EOF 
