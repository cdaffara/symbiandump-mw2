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
// T_PushEntry.CPP
// 
//


    
#include <e32std.h>
#include <e32test.h>
#include <msvstd.h>
#include <msvids.h>
#include <msvuids.h>
#include <pushentry.h>
#include "WPushUtils.h"
#include "t_serviceutils.h"
#include "pushservermtm.h"



_LIT(KPushMtmDatFile, "z:\\system\\mtm\\WapPushMtmU.dat");


//
// CTestScheduler: INTERFACE //
//
class CTestScheduler : public CActiveScheduler
	{
public:
	void		Error(TInt anError) const;
	};

//
//  MACRO DEFINTIONS
//
#define __LOG_ALWAYS(a) {_LIT(name,a); WPLPrintf(name);} 

#define __LOG_ALWAYS_FORMAT1(a,b) {_LIT(name,a); WPLPrintf(name,b);} 
#define __LOG_ALWAYS_FORMAT2(a,b,c) {_LIT(name,a); WPLPrintf(name,b,c);} 

#if defined(_DEBUG)
#define __LOG_DEBUG(a) __LOG_ALWAYS(a) 
#define __LOG_DEBUG_FORMAT1(a,b) __LOG_ALWAYS_FORMAT1(a,b) 
#define __LOG_DEBUG_FORMAT2(a,b,c) __LOG_ALWAYS_FORMAT2(a,b,c) 
#define __LOG_MSG_DEBUG(a) iLog.WPLPrintf(a)
#else
#define __LOG_DEBUG(a) 
#define __LOG_DEBUG_FORMAT1(a,b) 
#define __LOG_DEBUG_FORMAT2(a,b,c) 
#define __LOG_MSG_DEBUG(a) 
#endif


//**********************************************************************************
// Global variables

RTest gTest(_L("t_PushMessage Test Harness"));
LOCAL_D CTestScheduler* gActivescheduler;
CMsvSession* gMsvSession;
CMsvEntry*	gMsvEntry;


// CWapPushLog* gWLog;

//**********************************************************************************

LOCAL_C void WPLPrintf(TRefByValue<const TDesC> aFmt,...)
	{
	VA_LIST list;
	VA_START(list, aFmt);

	// Print to log file
	const TInt KWatcherLogBuffer = 256;
	TBuf<KWatcherLogBuffer> buf;
	buf.FormatList(aFmt, list);

	_LIT(KPushLogDir, "push");
	_LIT(KPushEntryLogFile, "PushMtmLogging.txt");
	// Write to log file
	RFileLogger::Write(KPushLogDir, KPushEntryLogFile, EFileLoggingModeAppend, buf);

	// Write to console
	buf.Append('\n');
	gTest.Printf(buf);
	}

//
// CExampleScheduler: Implementation //
//
void CTestScheduler::Error(TInt aError) const
    {
    CActiveScheduler::Stop();
    gTest.Printf(_L("\nLeave signalled, reason=%d\n"),aError);
    }
// end CExampleScheduler::Error


const TInt KMaxEntries = 3;

class CWapMtmTester:public CActive, public MMsvSessionObserver
	{
public:
	enum TTestState{
					ECreating,
					EChanging,
					EDeleting,
					EComplete
		};
	static CWapMtmTester* NewL();
	~CWapMtmTester();
	void Start();
	TMsvId CreateEntryL();
	void ChangeEntryL(TMsvId aId);
	void DeleteEntryL(TMsvId aId);
	void DisplayEntriesL(const CMsvEntrySelection& selection);
	void DisplayChildrenL(TMsvId aParent);

	inline void SetServiceId(TMsvId aID){iServiceId = aID;}
	
	
	void HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {}


private:
	void ConstructL();
	CWapMtmTester();
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);
	void ReportError(TInt aError);
	void QueueOperationAsync(TInt aErr);

	TInt			iEntryNo;
	TTestState		iState;
	TMsvId			iServiceId;
	TMsvEntry		iEntry;
	CMsvSession*	iMsvSession;
	CMsvEntry*		iMsvEntry;
	CMsvEntrySelection* iSelection;
	CMsvOperation*	iOperation;
	};

CWapMtmTester* CWapMtmTester::NewL()
	{
	CWapMtmTester* self =new (ELeave) CWapMtmTester;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CWapMtmTester::~CWapMtmTester()
	{
	Cancel();
	delete iSelection;
	delete iMsvEntry;
	delete iMsvSession;
	}


void CWapMtmTester::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection;
	iMsvSession = CMsvSession::OpenSyncL(*this);
	iMsvEntry = iMsvSession->GetEntryL(KMsvRootIndexEntryId);
	iEntryNo = 0;
	CActiveScheduler::Add(this);	
	}

CWapMtmTester::CWapMtmTester()
:CActive(EPriorityLow)
	{
	}


void CWapMtmTester::DoCancel()
	{
	}

void CWapMtmTester::RunL()
	{
	if (iStatus.Int() != KErrNone)
		{
		ReportError(iStatus.Int());
		}
	TPckgBuf<TPushMTMProgress> progressBuf;
	switch (iState)
		{
		case ECreating:
			{
			TInt error=KErrNone;
			if (iEntryNo != 0)
				{
				progressBuf.Copy(iOperation->ProgressL());
				error = progressBuf().iError;
   
				if (error != KErrNone)
					{
					ReportError(error);
					User::Leave(error);
					}
				error = iStatus.Int();
				if (error != KErrNone)
					{
					ReportError(error);
					User::Leave(error);
					}
				}
			else
				{
				if (iEntryNo <KMaxEntries)
					{
					iEntryNo++;
					CreateEntryL();
					}
				else
					iState = EComplete;
				}
			QueueOperationAsync(error);
			break;
			}
		case EComplete: //finished
			User::After(10000);
			Cancel();
			CActiveScheduler::Stop();
			break;
		default:
			break;
		}

	}

void CWapMtmTester::Start()
	{
	iState =ECreating;
	QueueOperationAsync(KErrNone);
	}


void CWapMtmTester::QueueOperationAsync(TInt aErr)
    {
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }


TInt CWapMtmTester::RunError(TInt aError)
	{
	ReportError(aError);
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CWapMtmTester::ReportError(TInt aError)
	{
	__LOG_DEBUG_FORMAT1("Active Scheduler Error %X", aError);
	}


TMsvId CWapMtmTester::CreateEntryL()
	{
	iState = ECreating;	


	switch (iEntryNo)
		{
		case 0:
			{
			iEntry.iType = KUidMsvMessageEntry;
			iEntry.iDate.HomeTime();
			iEntry.iMtm = KUidMtmWapPush;
			iEntry.iBioType = KUidWapPushMsgSI.iUid;
			iEntry.iServiceId = iServiceId;
			iEntry.iDetails.Set( _L("Push MTM Test  Msg 1"));
			iEntry.iDescription.Set(_L("Tests creatings, changing, deleting entries"));

//			if (iOperation)
//				{
//				delete iOperation;
//				iOperation = NULL;
//				}
			}
			break;
		case 1:
			iEntry.iBioType = KUidWapPushMsgSL.iUid;
			iEntry.iDetails.Set( _L("Push MTM Test  Msg 2"));
			iEntry.iDescription.Set(_L("Second message to store"));
//			iOperation = iMsvEntry->CreateL(iEntry, iStatus);
			break;
		case 2:
			iEntry.iBioType = 0x000111;
			iEntry.iDetails.Set(_L("Push Mtm Test Msg 3"));
			iEntry.iDescription.Set(_L("Another message to store"));
			break;
		default:
			break;
		}
	iEntryNo++;
	iMsvEntry->SetEntryL(iServiceId);
	CMsvOperationWait* wait=CMsvOperationWait::NewLC();
	CMsvOperation* op= iMsvEntry->CreateL(iEntry, wait->iStatus);
	CleanupStack::PushL(op);
	wait->Start();
	CActiveScheduler::Start();
	const TInt err=op->iStatus.Int();

	TPckgBuf<TPushMTMProgress> progressBuf;
	progressBuf.Copy(op->ProgressL());
	TPushMTMProgress progress = progressBuf();
	TMsvId newEntry = progress.iMsgId;
   
	CleanupStack::PopAndDestroy(2); //op, wait
 	User::LeaveIfError(err);
	User::LeaveIfError(progress.iError);

	__LOG_DEBUG("*************************************************");
	__LOG_DEBUG_FORMAT1("Created Entry     %X", newEntry);

	return newEntry;
	}

			
void CWapMtmTester::ChangeEntryL(TMsvId aId)
	{
	iMsvEntry->SetEntryL(aId);
	TMsvEntry entry = iMsvEntry->Entry();
	entry.iBioType = KUidWapPushMsgUnknown.iUid;
	TBuf<50> temp;
	temp.Format(_L("Some random words for %X"), aId);
	entry.iDetails.Set(temp);
	entry.iDescription.Set(_L("A changed Push Message "));

	CMsvOperationWait* wait=CMsvOperationWait::NewLC();
	CMsvOperation* op= iMsvEntry->ChangeL(entry, wait->iStatus);
	CleanupStack::PushL(op);
	wait->Start();
	CActiveScheduler::Start();
	const TInt err=op->iStatus.Int();

	TPckgBuf<TPushMTMProgress> progressBuf;
	progressBuf.Copy(op->ProgressL());
   
 	User::LeaveIfError(err);
	User::LeaveIfError(progressBuf().iError);

	__LOG_DEBUG("*************************************************");
	__LOG_DEBUG_FORMAT1("Changed Entry     %X", aId );
	CleanupStack::PopAndDestroy(2); //op, wait

	}


void CWapMtmTester::DeleteEntryL(TMsvId aId)
	{

	CMsvOperationWait* wait=CMsvOperationWait::NewLC();
	iMsvEntry->SetEntryL(aId);
	iMsvEntry->SetEntryL(iMsvEntry->Entry().Parent());
	CMsvOperation* op= iMsvEntry->DeleteL(aId, wait->iStatus);
	CleanupStack::PushL(op);
	wait->Start();
	CActiveScheduler::Start();
	const TInt err=op->iStatus.Int();

	TPckgBuf<TPushMTMProgress> progressBuf;
	progressBuf.Copy(op->ProgressL());
   
	CleanupStack::PopAndDestroy(2); //op, wait
 	User::LeaveIfError(err);
	User::LeaveIfError(progressBuf().iError);
	__LOG_DEBUG("*************************************************");
	__LOG_DEBUG_FORMAT1("Deleted Entry     %X", aId );

	}
void CWapMtmTester::DisplayEntriesL(const CMsvEntrySelection& selection)
	{
	TInt error= KErrNone;

	for (TInt count=0; count< selection.Count(); count++)
		{
		__LOG_DEBUG("*************************************************");
		__LOG_DEBUG_FORMAT1("Details of Entry No %d", (count+1) );

		TRAP(error, iMsvEntry->SetEntryL(selection.At(count)) );

		if (error==KErrNone)
			{
			__LOG_DEBUG_FORMAT1("Entry Id               %X", iMsvEntry->Entry().Id());
			__LOG_DEBUG_FORMAT1("Entry Parent           %X", iMsvEntry->Entry().Parent());
			__LOG_DEBUG_FORMAT1("Entry Service Id       %X", iMsvEntry->Entry().iServiceId);
			__LOG_DEBUG_FORMAT1("Entry Type             %X", iMsvEntry->Entry().iType);
			__LOG_DEBUG_FORMAT1("Entry MTM              %X", iMsvEntry->Entry().iMtm);
			__LOG_DEBUG_FORMAT1("Entry iBioType         %X", iMsvEntry->Entry().iBioType);
			__LOG_DEBUG_FORMAT1("Entry Details          %S", &(iMsvEntry->Entry().iDetails));
			__LOG_DEBUG_FORMAT1("Entry Description      %S", &(iMsvEntry->Entry().iDescription));
			}
		else 
			{
			if (error ==KErrNotFound)
				{
				__LOG_DEBUG_FORMAT1("Message details for entry %X not available - doesn't exist", selection.At(count));
				}
			else 
				{
				__LOG_DEBUG_FORMAT1("Server error %d occured ", error);
				}
			}
		}
	}



void CWapMtmTester::DisplayChildrenL(TMsvId aParent)
	{
	iMsvEntry->SetEntryL(aParent);
	CMsvEntrySelection* selection = iMsvEntry->ChildrenL();
	CleanupStack::PushL(selection);

	if (selection->Count() ==0)
		{
		__LOG_DEBUG("*************************************************");
		__LOG_DEBUG_FORMAT1("No children for entry %d", aParent );
		return;

		}
	__LOG_DEBUG("*************************************************");
	__LOG_DEBUG_FORMAT1("Details for children for entry %X", aParent );
	for (TInt count = 0; count<selection->Count(); count++)
		{
		iMsvEntry->SetEntryL(selection->At(count));

		__LOG_DEBUG_FORMAT1("Details of Entry No %d", (count+1) );
		__LOG_DEBUG_FORMAT1("Entry Id               %d", iMsvEntry->Entry().Id());
		__LOG_DEBUG_FORMAT1("Entry Parent           %X", iMsvEntry->Entry().Parent());
		__LOG_DEBUG_FORMAT1("Entry Service Id       %X", iMsvEntry->Entry().iServiceId);
		__LOG_DEBUG_FORMAT1("Entry Type             %X", iMsvEntry->Entry().iType);
		__LOG_DEBUG_FORMAT1("Entry MTM              %X", iMsvEntry->Entry().iMtm);
		__LOG_DEBUG_FORMAT1("Entry iBioType         %X", iMsvEntry->Entry().iBioType);
		__LOG_DEBUG_FORMAT1("Entry Details          %S", &(iMsvEntry->Entry().iDetails));
		__LOG_DEBUG_FORMAT1("Entry Description      %S", &(iMsvEntry->Entry().iDescription));
		}
	CleanupStack::PopAndDestroy();
	}




LOCAL_C void InitTestUtils()
	{
	// Setup test console
	gTest.Start(_L("CPushEntry classes"));
	gTest.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-PUSHMTM-T_PUSHMTM-0001 "));	

	gTest.Console()->ClearScreen();

	gActivescheduler = new CTestScheduler;
	gTest(gActivescheduler!=NULL);
	CActiveScheduler::Install(gActivescheduler);


	// Set up CWatcherLog
//	gWLog = CWapPushLog::NewL(*gTest.Console());
	}


LOCAL_C void CloseTestUtils()
	{
// 	delete gWLog;
	delete gActivescheduler;
	gTest.End();
	gTest.Close();
	}

LOCAL_C void doMainL()
	{
	InitTestUtils();
	__LOG_DEBUG("Getting Service Id");
	CMsvEntrySelection* idArray = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(idArray);

	CWapPushTestMsgUtils* myTestUtils = CWapPushTestMsgUtils::NewL(); 
	CleanupStack::PushL(myTestUtils);
	TMsvId pushService;

	// Uncomment this if you want to remove existing service entries
	myTestUtils->ClearExistingServiceEntriesL(KUidMtmWapPush);

	myTestUtils->PushServiceIdL(KUidMtmWapPush,pushService,idArray);
	if ((pushService == KMsvNullIndexEntryId) && (idArray->Count() ==0) ) // Found no service
		{
		__LOG_DEBUG("*************************************************");
		__LOG_DEBUG("No Service found - creating new one");
		pushService = myTestUtils->CreateServiceL();
		__LOG_DEBUG_FORMAT1("Created New Service Entry Id %X", pushService);
		}
	else
		{
		__LOG_DEBUG("*************************************************");
		__LOG_DEBUG_FORMAT1( "Found %d service entries\n", idArray->Count() );
		for (TInt count=0; count < idArray->Count(); count++)
			{
			__LOG_DEBUG_FORMAT2("Entry %d  - Service Id %X\n", count, idArray->At(count));
			}
		}
	myTestUtils->InstallMtmGroupL(KPushMtmDatFile);
	__LOG_DEBUG("*************************************************");
	__LOG_DEBUG("Installing Mtm Group");

	CleanupStack::PopAndDestroy();  //myTestUtils
	CleanupStack::PopAndDestroy(); //idArray

	idArray = NULL;
	idArray =  new (ELeave) CMsvEntrySelection(); // Create a new empty array
	CleanupStack::PushL(idArray);

	TMsvId myEntry;
	TInt count;
	CWapMtmTester*  myTester=CWapMtmTester::NewL();
	CleanupStack::PushL(myTester);
	myTester->SetServiceId(pushService);

	for (count =0; count <3; count++)
		{
		myEntry = myTester->CreateEntryL();
		idArray->AppendL(myEntry);
		}
	myTester->DisplayChildrenL(pushService);

	myTester->DisplayEntriesL(*idArray);
	
	for (count = 0; count < idArray->Count(); count++)
		myTester->ChangeEntryL(idArray->At(count));

	myTester->DisplayEntriesL(*idArray);
	myTester->DeleteEntryL(idArray->At(idArray->Count()-1));
	myTester->DisplayEntriesL(*idArray);
	myTester->DeleteEntryL(idArray->At(idArray->Count()-2));
	myTester->DisplayEntriesL(*idArray);

	CleanupStack::PopAndDestroy(2);//myTester

	
	gTest.Printf(_L("Press a key to continue\n"));
	gTest.Getch();
	CloseTestUtils();
	}


GLDEF_C TInt E32Main()
//----------------------------------------------------------------------------------------
	{	
	__UHEAP_MARK;
	CTrapCleanup*	theCleanup = CTrapCleanup::New();

	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);

	delete theCleanup;	
	__UHEAP_MARKEND;
	return(KErrNone);
	}
