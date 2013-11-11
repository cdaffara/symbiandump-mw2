// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "AppStarter.h"
#include "Starter.h"
#include "ThreadWatch.h"
#include "Start.hrh"

#if defined(__WINS__)
_LIT(KExtension, ".dll");
#else 
_LIT(KExtension, ".exe");
#endif

_LIT(KGlobalSemaphore, "SPLASH");

const TInt KRetryWait = 10*1000;
const TInt KMaxRetries = (10*1000*100)/KRetryWait;

void CAppStarter::NewLD(TInt aAppType, TUint32 aAppUid, TDesC& aExeFileName, TDesC& aDllFileName, TDesC* aCmdLineArgs, TUint32 aStackSize,
						TUint32 aMinHeapSize, TUint32 aMaxHeapSize, TBool aMonitoring, TBool aViewless, TBool aSemaphore,
						TSglQue<CThreadWatcher>* aQue, RApaLsSession& aSession, CStarter* aStarter)
	{
	CAppStarter* self = new(ELeave)CAppStarter(aSession);
	self->iAppType = aAppType;
	self->iAppUid = aAppUid;
	self->iExeFileName = aExeFileName;
	self->iDllFileName = aDllFileName;
	self->iCmdLineArgs = aCmdLineArgs;
	self->iStackSize = aStackSize;
	self->iMinHeapSize = aMinHeapSize;
	self->iMaxHeapSize = aMaxHeapSize;	
	self->iMonitoring = aMonitoring;
	self->iViewless = aViewless;
	self->iSemaphore = aSemaphore;
	self->iQue = aQue;
	self->iStarter = aStarter;

	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	CActiveScheduler::Add(self);
	self->RunL();
	}

void CAppStarter::RunL()
	{
	if(iRetries++ > KMaxRetries)
		{
		iState = EAppFailed;
		}

	switch(iState)
		{
		case EStartApp:
			StartL();
			break;
		case EAppStarted:
			Done(ETrue);
			break;
		default:
		case EAppFailed:
			Done(EFalse);
			break;
		}
	}

CAppStarter::CAppStarter(RApaLsSession& aSession) : CTimer(EPriorityStandard), iState(EStartApp), iSession(aSession), iRetries(0)
	{

	}

CAppStarter::~CAppStarter()
	{

	}

TInt CAppStarter::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CAppStarter::StartL()
	{
	TFileName newname = iExeFileName;

	TThreadId threadId = 0;

	switch(iAppType)
		{
		case EApplicationType:
			{
			TApaAppInfo info;
			if(iSession.GetAppInfo(info,TUid::Uid(iAppUid)) != KErrNone)
				{
				After(KRetryWait);
				return;
				}
			
			if(iViewless)
				{
				TRAPD(err, TryStartViewlessAppL(info, threadId));
				if (err != KErrNone)
					{
					After(KRetryWait);
					return; //ignore error
					}
				}
			else if (info.iUid!=KNullUid)
				{
				if(iSession.StartDocument(newname, TUid::Uid(iAppUid), threadId) != KErrNone)
					{
					After(KRetryWait);
					return;
					}
				}
			else
				{
				iState = EAppFailed;
				Next();
				}
			}
			break;
		case EExecutableType:
			{
			newname.Append(KExtension);
			if(iSession.StartDocument(newname, TUid::Uid(0), threadId) != KErrNone)
				{
				After(KRetryWait);
				return;
				}
			}
			break;
		case ECmdLnArgExecutableType:
			{
			ASSERT( !iMonitoring); // Not imlpemented yet
#if defined(__WINS__) || defined(__WINSCW__)
			TName libName = iDllFileName;
			libName.Append(KExtension);
			RLibrary lib;
			TInt error = lib.Load(libName);
			if (error!=KErrNone)
				{
				After(KRetryWait);
				return;
				}
			TThreadFunction serverFunc=reinterpret_cast<TThreadFunction>(lib.Lookup(1));
			RThread server;
			error=server.Create(libName,serverFunc, iStackSize, iCmdLineArgs, &lib,NULL,
								iMinHeapSize,iMaxHeapSize,EOwnerProcess);
			lib.Close();	// if successful, server thread has handle to library now
#else
			RProcess server;
			TInt error = server.Create(newname, *iCmdLineArgs);
#endif
			if( error != KErrNone)
				{
				After(KRetryWait);
				return;
				}
			server.Resume();
			server.Close();
			}
			break;
		default:
			iState = EAppFailed;
			Next();
		}
	
	if (iMonitoring)
		{
		CThreadWatcher *threadWatcher=NULL;
		TRAPD(err, threadWatcher = CThreadWatcher::NewL(iAppType, threadId, newname, iStarter, iAppUid, iViewless));
		if (err == KErrNone) //ignore errors
			iQue->AddLast(*threadWatcher);
		}
	iState = EAppStarted;
	Next();
	}

void CAppStarter::Done(TBool aSuccess)
	{
	if(iSemaphore)
		{
		iStarter->Done(aSuccess);
		}
	delete this;
	}

void CAppStarter::Next()
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}
	
void CAppStarter::TryStartViewlessAppL(TApaAppInfo aInfo, TThreadId aThreadId)
	{
	CApaCommandLine* cmdLine=CApaCommandLine::NewLC();
	cmdLine->SetLibraryNameL(aInfo.iFullName);
	cmdLine->SetCommandL(EApaCommandRunWithoutViews);
	User::LeaveIfError(iSession.StartApp(*cmdLine, aThreadId));
	CleanupStack::PopAndDestroy();	// cmdLine
	}
