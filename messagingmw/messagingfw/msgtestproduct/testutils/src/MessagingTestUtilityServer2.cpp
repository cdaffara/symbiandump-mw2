// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MessagingTestUtilityServer2 implementation
// 
//

/**
 @file
*/

#include <e32std.h>
#include "MessagingTestUtilityServer2.h"
#include <messagingtestutility2.h>

//DoDeleteMessageStore headers:
#include "TestUtilityServerObserver.h"

//DoCopyFile headers:
#include <bautils.h>

#include "cmsgtestpropertywatcher.h"
#include "tmobilitytestpropertymapaccess.h"
#include "mobilitytestpropertyvalue.h"

inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}
inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}
inline void CShutdown::Start()
	{After(KMyShutdownDelay);}

inline CMessagingTestUtilityServer2::CMessagingTestUtilityServer2()
	:CServer2(0,ESharableSessions)
	{}

inline CMessagingTestUtilitySession2::CMessagingTestUtilitySession2()
: iEventList()
	{}

inline CMessagingTestUtilityServer2& CMessagingTestUtilitySession2::Server()
	{return *static_cast<CMessagingTestUtilityServer2*>(const_cast<CServer2*>(CSession2::Server()));}


/**
PanicClient()
RMessage::Panic() also completes the message. This is:
(a) important for efficient cleanup within the kernel
(b) a problem if the message is completed a second time

@param aMessage
@param aPanic
*/
void PanicClient(const RMessage2& aMessage,TTestPanic aPanic)
	{
	_LIT(KPanic,"MessagingTestUtilityServer2");
 	aMessage.Panic(KPanic,aPanic);
	}


/**
CreateL()
2nd phase construct for sessions - called by the CServer framework
*/
void CMessagingTestUtilitySession2::CreateL()
	{
	Server().AddSession();

	User::LeaveIfError(iFs.Connect());
	}


/**
~CMessagingTestUtilitySession2()
Destructor
*/
CMessagingTestUtilitySession2::~CMessagingTestUtilitySession2()
	{
	Server().DropSession();

	iFs.Close();
	delete iDir;

	delete iWatcherNetTestStub;

	iEventList.ResetAndDestroy();
	iEventList.Close();
	}


/**
ServiceL()
Entry point for when a new message is received

@param aMessage
*/
void CMessagingTestUtilitySession2::ServiceL(const RMessage2& aMessage)
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
	case EEventHandlerInit:
		result = DoEventHandlerInitL();
		break;
	case EEventHandlerAdd:
		result = DoEventHandlerAddL(aMessage);
		break;
	case EEventHandlerCheck:
		result = DoEventHandlerCheck();
		break;
	case EMount:
		{
		TInt drive = aMessage.Int0();
		TFileName fsName;
		result = iFs.FileSystemName(fsName, drive);
		result = iFs.MountFileSystem(fsName, drive);
		break;
		}
	case EUnMount:
		{
		TInt drive = aMessage.Int0();
		TFileName fsName;
		result = iFs.FileSystemName(fsName, drive);
		result = iFs.DismountFileSystem(fsName, drive);
		break;
		}
	default:
		PanicClient(aMessage,ETestPanicIllegalFunction);
		break;
		}
	aMessage.Complete(result);
	}


/**
ServiceError()
Handle an error from CMySession::ServiceL()
A bad descriptor error implies a badly programmed client, so panic it;
otherwise use the default handling (report the error to the client)
*/
void CMessagingTestUtilitySession2::ServiceError(const RMessage2& aMessage,TInt aError)
	{
	if (aError==KErrBadDescriptor)
		PanicClient(aMessage,ETestPanicBadDescriptor);
	CSession2::ServiceError(aMessage,aError);
	}


/**
RunL()
Initiate server exit when the timer expires
*/
void CShutdown::RunL()
	{
	CActiveScheduler::Stop();
	}


/**
NewLC()
static interface for symbian standard 2 phase constructor
*/
CServer2* CMessagingTestUtilityServer2::NewLC()
	{
	CMessagingTestUtilityServer2* self=new(ELeave) CMessagingTestUtilityServer2;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


/**
ConstructL()
2nd phase construction - ensure the timer and server objects are running
*/
void CMessagingTestUtilityServer2::ConstructL()
	{
	StartL(KTestServerName);
	iShutdown.ConstructL();
	// ensure that the server still exits even if the 1st client fails to connect
	iShutdown.Start();
	}


/**
NewSessionL()
Create a new client session. This should really check the version number.

@return
*/
CSession2* CMessagingTestUtilityServer2::NewSessionL(const TVersion&,const RMessage2&) const
	{
	return new(ELeave) CMessagingTestUtilitySession2();
	}


/**
AddSession()
A new session is being created
Cancel the shutdown timer if it was running
*/
void CMessagingTestUtilityServer2::AddSession()
	{
	++iSessionCount;
	iShutdown.Cancel();
	}


/**
DropSession()
A session is being destroyed
Start the shutdown timer if it is the last session.
*/
void CMessagingTestUtilityServer2::DropSession()
	{
	if (--iSessionCount==0)
		{
		iShutdown.Start();
		}
	}

/**
DoDeleteMessageStoreL()
Functions to service calls to the Messaging Test Utility Server:

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoDeleteMessageStoreL(const RMessage2& aMessage)
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
/*#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_TEST_USING_SQLDB)
	CMsvStore* store = cEntry->EditDBStoreL(ETrue);
#else
	CMsvStore* store = cEntry->EditStoreL();	
#endif*/

	CMsvStore* store = cEntry->EditStoreL();
	store->DeleteL();

	delete store;

	CleanupStack::PopAndDestroy(3); //cEntry, session, ob.

	return ret;
	}


/**
DoKillProcessL()
Kills a process

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoKillProcessL(const RMessage2& aMessage)
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


/**
DoCopyFileL()
Copies the file from source to the destination

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoCopyFileL(const RMessage2& aMessage)
	{
	TBuf<255> srcPath, destPath;
	aMessage.ReadL(0, srcPath);
	aMessage.ReadL(1, destPath);

	TInt ret = BaflUtils::CopyFile(iFs, srcPath, destPath);

	return ret;
	}


/**
DoDeleteFileL()
Delete the file

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoDeleteFileL(const RMessage2& aMessage)
	{
	TBuf<255> filePath;
	aMessage.ReadL(0, filePath);

	TInt ret = iFs.Delete(filePath);

	return ret;
	}


/**
DoMkDirL()
Creates a directory

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoMkDirL(const RMessage2& aMessage)
	{
	TBuf<255> dirPath;
	aMessage.ReadL(0, dirPath);

	TInt ret = iFs.MkDirAll(dirPath);

	return ret;
	}


/**
DoRmDirL()
Removes a directory

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoRmDirL(const RMessage2& aMessage)
	{
	TBuf<255> dirPath;
	aMessage.ReadL(0, dirPath);

	TInt ret = iFs.RmDir(dirPath);

	return ret;
	}


/**
DoGetDirL()
Gets a directory

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoGetDirL(const RMessage2& aMessage)
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


/**
DoGetDirCountL()
Gets a directory counts

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoGetDirCountL(const RMessage2& aMessage)
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


/**
DoGetDirEntryL()
Gets a directory entry

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoGetDirEntryL(const RMessage2& aMessage)
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


/**
DoFileExistsL()
checks if the file exist

@param aMessage
@return
*/
TBool CMessagingTestUtilitySession2::DoFileExistsL(const RMessage2& aMessage)
	{
	TBuf<255> filePath;
	aMessage.ReadL(0, filePath);

	TBool ret = BaflUtils::FileExists(iFs, filePath);

	return ret;
	}


//EventHandler

#ifdef __MOBILITY_TEST_FRAMEWORK
void CMessagingTestUtilitySession2::HandleEventL(RProperty& aProperty, TUint /*aKey*/)
#else //__MOBILITY_TEST_FRAMEWORK
void CMessagingTestUtilitySession2::HandleEventL(RProperty& /*aProperty*/, TUint /*aKey*/)
#endif //__MOBILITY_TEST_FRAMEWORK
	{
#ifdef __MOBILITY_TEST_FRAMEWORK

//Check the event is expected...
//	-take the first event from the list.
//	-if it matches (flag) then set the next event (call).
//	-then remove it from the list.

	TInt propertyVal = 0;
	User::LeaveIfError(aProperty.Get(propertyVal));

	const TMsgTestPropertyMapEntry* propertyMap = TMobilityTestPropertyMapAccess::Get(propertyVal);

	if(propertyMap == NULL)
		{
		User::Leave(KErrNotFound);
		}

	if(iEventList.Count() > 0)
		{
		CMsgTestPropertyMapPair* ptr = iEventList[0];

		if(propertyMap->value == ptr->flag->value)
			{
			aProperty.Set(KPropertyCatMsgTestUtilityServer2, EMyPropertyCounter, ptr->call->value);

			iEventList.Remove(0);
			delete ptr;
			}
		}

#endif //__MOBILITY_TEST_FRAMEWORK
	}

/**
DoEventHandlerInitL()
Initialise event handler.

@param aMessage
@return
*/
TInt CMessagingTestUtilitySession2::DoEventHandlerInitL()
	{
#ifdef __MOBILITY_TEST_FRAMEWORK

	RProcess me;
	me.SetPriority(EPriorityHigh);
	User::SetPriorityControl(EFalse);

	TInt r=RProperty::Define(EMyPropertyCounter,RProperty::EInt,KAllowAllPolicy,KAllowAllPolicy);
	if (r!=KErrAlreadyExists)
	    {
	    User::LeaveIfError(r);
	    }

	delete iWatcherNetTestStub;
	iWatcherNetTestStub = NULL;
	iWatcherNetTestStub = CMsgTestPropertyWatcher::NewL(KPropertyCatMsgTestUtilityServer2, EMyPropertyCounter, *this);

	iEventList.ResetAndDestroy();

#endif //__MOBILITY_TEST_FRAMEWORK

	return KErrNone;
	}

/**
DoEventHandlerAdd()
Add an event handler.

@param aMessage
@return
*/
#ifdef __MOBILITY_TEST_FRAMEWORK
TInt CMessagingTestUtilitySession2::DoEventHandlerAddL(const RMessage2& aMessage)
#else //__MOBILITY_TEST_FRAMEWORK
TInt CMessagingTestUtilitySession2::DoEventHandlerAddL(const RMessage2& /*aMessage*/)
#endif //__MOBILITY_TEST_FRAMEWORK
	{
#ifdef __MOBILITY_TEST_FRAMEWORK

	TBuf<255> buf;

	aMessage.ReadL(0, buf);
	const TMsgTestPropertyMapEntry* propertyMapFlag = TMobilityTestPropertyMapAccess::Get(&buf);

	aMessage.ReadL(1, buf);
	const TMsgTestPropertyMapEntry* propertyMapCall = TMobilityTestPropertyMapAccess::Get(&buf);

	CMsgTestPropertyMapPair* propertyMapPair = new(ELeave) CMsgTestPropertyMapPair(const_cast<TMsgTestPropertyMapEntry*>(propertyMapFlag), const_cast<TMsgTestPropertyMapEntry*>(propertyMapCall));

	if((propertyMapPair->call == NULL) || (propertyMapPair->flag == NULL))
		{
		return KErrArgument;
		}

	iEventList.Append(propertyMapPair);

#endif //__MOBILITY_TEST_FRAMEWORK

	return KErrNone;
	}

/**
DoEventHandlerCheck()
Check event handler.

@param aMessage
@return Number of events left in list
*/
TInt CMessagingTestUtilitySession2::DoEventHandlerCheck()
	{
	TInt numEvents = 0;

#ifdef __MOBILITY_TEST_FRAMEWORK

	numEvents = iEventList.Count();
	iEventList.ResetAndDestroy();

#endif //__MOBILITY_TEST_FRAMEWORK

	return numEvents;
	}
