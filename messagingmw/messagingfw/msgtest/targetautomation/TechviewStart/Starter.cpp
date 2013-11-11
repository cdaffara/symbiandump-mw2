// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
			  
#include "Starter.h"
#include "AppStarter.h"
#include "ThreadWatch.h"
#include <e32property.h>
#include "MsvTestUtils.h"
#include <Start.rsg>
#include "Start.hrh"

typedef TInt (*ShellEntryPoint)(TAny *);
#if defined(__WINS__)
LOCAL_D const TUint KHeapSize=0x10000;
LOCAL_D const TUint KMaxHeapSize=0x400000;
#endif


_LIT(KStubRunResourceFileName, "Z:\\SYSTEM\\PROGRAMS\\Start.RSC");
_LIT(KGlobalSemaphore, "SPLASH");
_LIT(KStartChunkName,"Start");

const TInt KRetryTime = 100000;

/**
@internalComponent
*/
TBool definedPhonePwrValue = EFalse;


/**
* Contructor
*/
CStarter::CStarter() :	CActive(EPriorityStandard), iNrSemaphore(0)
	{}

/**
* Destructor.
*/
CStarter::~CStarter()
	{
	if (iQueHeader)
		{
		while (!(iQueHeader->IsEmpty()))
			{
			CThreadWatcher* threadWatcher = iQueHeader->First();
			iQueHeader->Remove(*threadWatcher);
			delete threadWatcher;
			}
		delete iQueHeader;
		}

	iLsSess.Close();
#if defined (__WINS__)
	iAppUiSession.Close();
#endif
	delete iStartupList;
	delete iExtrasList;
	delete iSplashExe;
	iCmdLineArgs.Close();
	iResourceFile.Close();
	iFsSession.Close();
	}

/**
* Creates an instance of a CStarter object.
* @return			a pointer to this object
*/
CStarter* CStarter::NewL()
	{
	CStarter* self = new (ELeave) CStarter;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/**
* Handles the actual initialization of this object.
*/
void CStarter::ConstructL()
	{
	// Connect to the File Server
	while (iFsSession.Connect() != KErrNone)
		{
		User::After(KRetryTime);
		}

	//Connected to File Server
	iResourceFile.OpenL(iFsSession, KStubRunResourceFileName);

	// read the first one
    HBufC8* res = iResourceFile.AllocReadLC(R_APP_START_PARAMS);
    TResourceReader theReader;
    theReader.SetBuffer(res);
	//Main configuration resource read

	// get data from it
	iSplashExe = theReader.ReadHBufCL();

	TUint32 applicationsListId = theReader.ReadInt32();
	TUint32 exeListId = theReader.ReadInt32();

	// free up memory
	CleanupStack::PopAndDestroy();	// res

	// prepare the data for the active object
	iStartupList = iResourceFile.AllocReadL(applicationsListId);
    iStartupListReader.SetBuffer(iStartupList);
	iStartupCount = iStartupListReader.ReadUint16();	//The size of the resource array

	iExtrasList= iResourceFile.AllocReadL(exeListId);
    iExtrasListReader.SetBuffer(iExtrasList);
	iExtrasCount = iExtrasListReader.ReadUint16();	//The size of the resource array

	iQueHeader = new (ELeave) TSglQue<CThreadWatcher>(_FOFF(CThreadWatcher,iLink));

	// Tell system agent the phone is on.
	// This is only appropriate for 2-box solutions, and is needed for SMS sending and receiving.
	// Also, at some point, this should probably be moved to a separate component that startup launches.
	TInt testState;
	if(KErrNone != RProperty::Get(KUidSystemCategory, KMsvTestUidPhonePwrValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KMsvTestUidPhonePwrValue, RProperty::EInt));
		definedPhonePwrValue = ETrue;
		}
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOn));

	if(definedPhonePwrValue)
		{
		User::LeaveIfError(RProperty::Delete(KUidSystemCategory, KMsvTestUidPhonePwrValue));
		}

	RChunk chunk;
	TUint8 finish=0;
	if(	chunk.CreateGlobal(KStartChunkName,1,1)== KErrNone)
		{
		if(	chunk.OpenGlobal(KStartChunkName,EFalse)== KErrNone)
			{
			TUint8* base=chunk.Base();
			*base=finish;
			chunk.Close();
			}
		}
	}

/**
* Actually launch the splash screen exe
*/
void CStarter::StartSplashScreenL()
	{
	if (iSplashExe == NULL || iSplashExe->Length() == 0)
		{
		return;
		}

	// launch the splash screen
	TPtrC splashCmd;
	splashCmd.Set(*iSplashExe);

#if defined(__WINS__)
	TParse fileName;
	User::LeaveIfError(fileName.Set(_L(".DLL"),&splashCmd,NULL));
	User::LeaveIfError(iLib.Load(fileName.FullName()));
	ShellEntryPoint libFunc=(ShellEntryPoint)iLib.Lookup(1);
	if (!libFunc)
		{
		User::Leave(KErrGeneral);
		}
	TBuf<256> name;
	TInt num=0;
	TInt ret=KErrNone;
	do
		{
		name.Format(TRefByValue<const TDesC>(_L("Splash%d")),num++);
		ret=iSplash.Create(name,libFunc,KDefaultStackSize,&splashCmd,&iLib,NULL,/*NULL,*/KHeapSize,KMaxHeapSize,EOwnerProcess);
		} while(ret==KErrAlreadyExists);
	User::LeaveIfError(ret);
#else
	TFindFile findShell(iFsSession);
	User::LeaveIfError(findShell.FindByDir(splashCmd,_L("\\System\\Programs\\.EXE")));
	//User::LeaveIfError(iSplash.Create(findShell.File(),splashCmd));
	TInt err1 = 0;
	TRAPD(err,(err1 = iSplash.Create(findShell.File(),splashCmd)));	//Change back to commented out line when E32 bug is fixed ###
	if (err!=KErrNone && err1 != KErrNone)
		{
		iSplash.SetHandle(KNullHandle);
		User::Leave(err);
		}
#endif
	iSplash.Resume();

	}

/**
* Method to be called to procced the startup
*/
void CStarter::Start()
	{
	iStage = ESplashScreen;
	RequestNotify();
	}

/**
* Start then connect the Eikon server
*/
void CStarter::Connect()
	{
	// Connect to the AppUi session, effective starting it.
	while(iAppUiSession.Connect() != KErrNone)
		{
		User::After(KRetryTime);
		}

	// Connect to the AppLsServer
	while(iLsSess.Connect() != KErrNone)
		{
		User::After(KRetryTime);
		}

	TInt count=0;
	while(iLsSess.GetAllApps() != KErrNone) //collect data
		{
		}
  	while(iLsSess.AppCount(count) != KErrNone) //collect data
		{
		}
	RequestNotify();
	}

/**
* Proceed the next application to be launched
*/
void CStarter::LaunchNextStartupAppL()
	{
	TUint32 applicationUid = iStartupListReader.ReadUint32();
	TFullName exeFilePath = iStartupListReader.ReadTPtrC();
	TFullName dllFilePath = iStartupListReader.ReadTPtrC();
	iCmdLineArgs.Append( iStartupListReader.ReadTPtrC());
	TDesC *commandLineArgs = &iCmdLineArgs[ iCmdLineArgs.Count()-1];

	TUint32 stackSize = iStartupListReader.ReadUint32();
	stackSize = stackSize ? stackSize : KDefaultStackSize;
	TUint32 minHeapSize = iStartupListReader.ReadUint32();
	minHeapSize = minHeapSize ? minHeapSize : KMinHeapSize;
	TUint32 maxHeapSize = iStartupListReader.ReadUint32();

	TBool monitored = (TUint16)iStartupListReader.ReadUint16();
	TBool semaphore = (TUint16)iStartupListReader.ReadUint16();
	TBool viewless = (TUint16)iStartupListReader.ReadUint16();
	TInt bootType  = (TUint16)iStartupListReader.ReadUint16();
			
	if(semaphore)
		{
		iNrSemaphore++; //increase with one.
		iWaiting++;
		}

	iStartupCount--; // next app
	CAppStarter::NewLD(bootType, applicationUid, exeFilePath, dllFilePath, commandLineArgs,
						stackSize, minHeapSize, maxHeapSize, monitored, viewless,
						semaphore, iQueHeader, iLsSess, this);
	}

/**
* Proceed the next extra app to be launched
*/
void CStarter::LaunchNextExtrasL()
	{
	TUint32 applicationUid = iExtrasListReader.ReadUint32();
	TFullName exeFilePath = iExtrasListReader.ReadTPtrC();
	TFullName dllFilePath = iExtrasListReader.ReadTPtrC();
	iCmdLineArgs.Append( iExtrasListReader.ReadTPtrC());
	TDesC *commandLineArgs = &iCmdLineArgs[ iCmdLineArgs.Count()-1];

	TUint32 stackSize = iExtrasListReader.ReadUint32();
	stackSize = stackSize ? stackSize : KDefaultStackSize;
	TUint32 minHeapSize = iExtrasListReader.ReadUint32();
	minHeapSize = minHeapSize ? minHeapSize : KMinHeapSize;
	TUint32 maxHeapSize = iExtrasListReader.ReadUint32();
	
	TBool monitored = (TUint16)iExtrasListReader.ReadUint16();
	TBool viewless = (TUint16)iExtrasListReader.ReadUint16();
	TInt bootType  = (TUint16)iExtrasListReader.ReadUint16();

	iExtrasCount--; // next app
	CAppStarter::NewLD(bootType, applicationUid, exeFilePath, dllFilePath, commandLineArgs,
						stackSize, minHeapSize, maxHeapSize, monitored, viewless,
						EFalse, iQueHeader, iLsSess, this);
	}

void CStarter::RestartMonitoredThreadL(TInt appType, TThreadId& aThreadId, TDesC& aExeFileName, TUint32 aAppUid, TBool aViewless)
  	{
  	if(appType == EExecutableType) 
  		{
  		User::LeaveIfError(iLsSess.StartDocument(aExeFileName, TUid::Uid(0), aThreadId));
  		} 
  	else if(aViewless)
  		{
  		TApaAppInfo info;
  		User::LeaveIfError(iLsSess.GetAppInfo(info,TUid::Uid(aAppUid)));
  		CApaCommandLine* cmdLine=CApaCommandLine::NewLC();	
  		cmdLine->SetLibraryNameL(info.iFullName);
  		cmdLine->SetCommandL(EApaCommandRunWithoutViews);
  		User::LeaveIfError(iLsSess.StartApp(*cmdLine, aThreadId));
  		CleanupStack::PopAndDestroy();	// cmdLine
		}
  	else
  		{
  		TApaAppInfo info;
  		User::LeaveIfError(iLsSess.GetAppInfo(info,TUid::Uid(aAppUid)));
  		TFileName fName;
  		if (info.iUid!=KNullUid)
  			{
  			User::LeaveIfError(iLsSess.StartDocument(fName, TUid::Uid(aAppUid), aThreadId));
  			}
  		}
   	}

/**
* Method called when the requested step have been procced
*/
void CStarter::RunL()
	{
	switch (iStage)
		{
		case ESplashScreen:
			{
			iStage =  EStarting;
			// don't care about the error - for now
			TRAPD(err, StartSplashScreenL());
			iSplashStarted = (err == KErrNone);
			RequestNotify();		
			}
			break;
		case EStarting:
			iStage = EConnected;
			Connect();
			break;
		case EConnected:
			{
			iStage = EStart;
			iSema.CreateGlobal(KGlobalSemaphore, EOwnerProcess); //For Shell
			RequestNotify();
			}
			break;
		case EStart:
			{
			iStage = EStart;
			if (iStartupCount)
				{
				LaunchNextStartupAppL();
				RequestNotify();
				}
			break;
		case EWait:
 			iStage = EStartExtras;
			if(iSema.OpenGlobal(KGlobalSemaphore, EOwnerProcess) == KErrNone)
				{
				while(iNrSemaphore > 0) //Defined in the resourcefile for apps that has to be started before the splash screen is removed
					{
					iSema.Wait();
					iNrSemaphore--;
					}
				iSema.Close();
				}

			if(iSplashStarted)
				{
				//use chunk to tell splash screen to stop by setting to 1
				RChunk chunk;
				TUint8 finish=1;
				if(	chunk.OpenGlobal(KStartChunkName,EFalse)== KErrNone)
					{
					TUint8* base=chunk.Base();
					*base=finish;
					chunk.Close();
					}
				}

			RequestNotify();
			}
			break;
		case EStartExtras:
			if(iExtrasCount)
				{
				LaunchNextExtrasL();
				RequestNotify();
				}
			break;
		default: 
			iStage = ESplashScreen;
			break;
		}
	}

TInt CStarter::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CStarter::Done(TBool aSuccess)
	{
	if(aSuccess)
		{
		iWaiting--;
		}
	else
		{
		iWaiting--;
		iNrSemaphore--;
		}
	if(iWaiting <= 0)
		{
		iStage = EWait;
		RequestNotify();
		}
	}
/**
* Cancel the execution
*/
void CStarter::DoCancel()
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrCancel);
	}

void CStarter::RequestNotify()
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}
