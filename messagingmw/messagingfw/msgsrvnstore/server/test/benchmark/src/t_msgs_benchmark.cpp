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
//

#include <e32test.h>
#include <msvapi.h>
#include <msvids.h>
#include "msvtestutils.h"
#include "msvtestconfig.h"
#include "msgservertestutils.h"
#include <miutset.h>


enum Tests {
	ECreateTime,
	EMoveTime,
	ESetTime,
	ECopyTime,
	EDeleteTime,
	ENumberOfProfiles
	};

CMsvOperation* gOp=NULL;
CMsvEntrySelection* gSel;
TMsvId gFolderId1;
TMsvId gFolderId2;

//
//	Profile
//

#define PSTART() TMsgProfile::StartProfile()
#define PEND(id) TMsgProfile::EndProfile(id)
TInt64 gResults[ENumberOfProfiles];
TTime gTime;

class TMsgProfile
	{
public:	
	static void StartProfile();
	static void EndProfile(TInt aId);
	};

void TMsgProfile::StartProfile()
	{
	gTime.UniversalTime();
	}

void TMsgProfile::EndProfile(TInt aId)
	{
	TTime now;
	now.UniversalTime();
	
	const TInt64 t =now.MicroSecondsFrom(gTime).Int64();
	gResults[aId]=t;
	}


TInt LineNo;

_LIT(KTestName,"Message Server Benchmarking test");
RTest tests(KTestName);
RDebug debug;
CMsgServerTestUtils* theUtils = NULL;

class TDummyObserver: public MMsvSessionObserver
	{
public:
	virtual void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*){};
	};	


RFile OpenCSVPrintHeaderLC()
	{
	LineNo=1;
	RFile samples;
	TParse sampleName;
	TFileName filename;
	
	TDriveUnit drive(MessageServer::CurrentDriveL(theUtils->FileSession()));
	TInt end = drive.Name().LocateReverse(':');
	if (end == KErrNotFound)
		end = drive.Name().Length();
	filename.Append(drive.Name().Left(end));

#if defined(__WINS__)
	filename.Append(_L("_wins_"));
#endif
#if (defined(__THUMB__) || defined(__MARM_THUMB__))
	filename.Append(_L("_thumb_"));
#endif
#if (defined(__ARMI__) || defined(__MARM_ARMI__))
	filename.Append(_L("_armi_"));
#endif
#if (defined(__ARM4__) || defined(__MISA__) || defined(__MARM_ARM4__))
	filename.Append(_L("_arm4_"));
#endif

#if defined(_DEBUG)
	filename.Append(_L("deb.csv"));
#else
	filename.Append(_L("rel.csv"));
#endif

	theUtils->ResolveLogFile(filename,sampleName);
	User::LeaveIfError(samples.Replace(theUtils->FileSession(),sampleName.FullName(),EFileWrite|EFileShareAny));
	CleanupClosePushL(samples);
	
	TTimeIntervalMicroSeconds32 period;
	UserHal::TickPeriod(period);
	TBuf8<256> buffer;
	
	TMachineInfoV1Buf machineInfo;
	UserHal::MachineInfo(machineInfo);

	buffer.Format(_L8("Machine uid, %u, %d, ClockSpeed, %d, Speed factor, %d, Rom version, "),
		I64LOW(machineInfo().iMachineUniqueId),
		I64HIGH(machineInfo().iMachineUniqueId),
		machineInfo().iProcessorClockInKHz,
		machineInfo().iSpeedFactor);
	samples.Write(buffer);

	buffer.Copy(machineInfo().iRomVersion.Name());	
	samples.Write(buffer);
	samples.Write(_L8("\n"));
	LineNo++;

	buffer.Format(_L8("\"Tick period\",\"%u\"\n"),period);
	samples.Write(buffer);
	LineNo++;

	TVolumeInfo volume;
	theUtils->FileSession().Volume(volume,drive);

	TMemoryInfoV1Buf memInfo;
	UserHal::MemoryInfo(memInfo);
	buffer.Format(_L8("\"Ram Used\",\"%d\",\"Disk Used\",\"%d\"\n"),
		memInfo().iTotalRamInBytes-memInfo().iFreeRamInBytes,
		volume.iSize-volume.iFree);
	samples.Write(buffer);
	LineNo++;

	_LIT8(KTableHead,"\"No\",\"Create\",\"Move\",\"Set\",\"Copy\",\"Delete\",\"Mem Used\",\"Disk Used\"\n");

	samples.Write(KTableHead);
	LineNo++;
	return(samples);
	}

void CreateFolderL(CMsvEntry &aEntry, TMsvId &aFolderId)
	{
	aEntry.SetEntryL(KMsvDraftEntryId);
	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	aEntry.CreateL(folder);
	aFolderId=folder.Id();
	}

void RunWhilePending(const TRequestStatus &aStatus)
	{
	TInt err;
	while(aStatus==KRequestPending)
		{
		CActiveScheduler::Current()->RunIfReady(err,0);
		User::WaitForAnyRequest();
		}
	}
static void DoCreateL(TInt aCount, TMsvEntry& aMessage, CMsvEntry& aEntry)
	{
	TRequestStatus status;
	PSTART();
	
	while(aCount--)
		{
		gOp=aEntry.CreateL(aMessage,status);
		RunWhilePending(status);
		delete gOp;
		gOp=NULL;
		}
	PEND(ECreateTime);
	}

static void DoMoveL(CMsvEntry& aEntry)
	{
	aEntry.SetEntryL(KMsvRootIndexEntryId);
	aEntry.SetEntryL(gFolderId1);
	
	TRequestStatus status;
	
	gSel=aEntry.ChildrenL();
	CleanupStack::PushL(gSel);
		
	PSTART();
	
	gOp=aEntry.MoveL(*gSel,gFolderId2,status);
	RunWhilePending(status);
	delete gOp;
	gOp=NULL;
		
	PEND(EMoveTime);
	
	CleanupStack::PopAndDestroy(gSel);
	gSel=NULL;
	}

static void DoSetL(CMsvEntry& aEntry)
	{
	PSTART();

	aEntry.SetEntryL(gFolderId2);
	
	PEND(ESetTime);
	}


static void DoCopyL(CMsvEntry& aEntry)
	{
	PSTART();
	
	TRequestStatus status;

	gOp=aEntry.CopyL(*gSel,gFolderId1,status);
	RunWhilePending(status);

	PEND(ECopyTime);
	
	delete gOp;
	gOp=NULL;
	}


static void DoDeleteL(CMsvEntry& aEntry)
	{
	PSTART();
	TRequestStatus status;
	
	gOp=aEntry.DeleteL(*gSel,status);
	RunWhilePending(status);
	
	PEND(EDeleteTime);
	
	delete gOp;
	gOp=NULL;
	}

static void DisplayResults()
	{
	TBuf<512> line;
	_LIT(KTableHead,"Create|Move|Set|Copy|Delete (times in ms)\n");

	TInt t0 = gResults[ECreateTime]/1000;
	TInt t1 = gResults[EMoveTime]/1000;
	TInt t2 = gResults[ESetTime]/1000;
	TInt t3 = gResults[ECopyTime]/1000;
	TInt t4 = gResults[EDeleteTime]/1000;
	line.Format(_L("%d|%d|%d|%d|%d\n"),
			t0, t1, t2, t3, t4 );
			
	tests.Printf(KTableHead);
	tests.Printf(line);
	User::After(1*1000000);
	}
	
void RunTestsL(CMsvEntry &aEntry)
	{
	CreateFolderL(aEntry,gFolderId1);
	CreateFolderL(aEntry,gFolderId2);

	_LIT(KAnAddress,"Address 9012345678901234567890");
	_LIT(KADescription,"Content 901234567890123456789012345678901234567890123456789012345678901234567890");

	TMsvEntry message;
	message.iDetails.Set(KAnAddress);
	message.iDescription.Set(KADescription);
	message.iMtm = KUidMsvLocalServiceMtm;
	message.iServiceId = KMsvLocalServiceIndexEntryId;
	message.iType=KUidMsvMessageEntry;


	//initialize array
	//TInt64 gResults[ENumberOfProfiles];
	for (int i=0; i<ENumberOfProfiles;i++)
		{
		gResults[i]=0.0;
		}
	//end init
	TInt entries=50;

	aEntry.SetEntryL(gFolderId1);

	DoCreateL(entries, message, aEntry);
	DoMoveL(aEntry);
	DoSetL(aEntry);
	
	gSel=aEntry.ChildrenL();
	CleanupStack::PushL(gSel);
	
	DoCopyL(aEntry);
	DoDeleteL(aEntry);

	CleanupStack::PopAndDestroy(gSel);
	gSel=NULL;

	CTestTimer *timer=CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	DisplayResults();
	}

void DoMainL()
	{
	TInt drive=EDriveC;

	theUtils=CMsgServerTestUtils::NewL(tests);
	CActiveScheduler::Install(new(ELeave)CTestScheduler);

	theUtils->SetDriveL(drive);

	theUtils->CleanMessageFolderL();
	
	TDummyObserver sObs;
	CMsvSession* session=CMsvSession::OpenSyncL(sObs);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry=session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(cEntry);

	const TInt number=10;
	int i;
	for(i=0;i<number;i++)
		{
		CMsvSession *session2=CMsvSession::OpenSyncL(sObs);
		session2->SetReceiveEntryEvents(EFalse);
		CleanupStack::PushL(session2);
		}
	TInt testno=1;


	theUtils->TestStart(testno,_L("Timing messages"));
 
	RunTestsL(*cEntry);
	
	theUtils->TestFinish(testno++,KErrNone);

	for(i=0;i<number;i++)
		CleanupStack::PopAndDestroy();

	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);

	theUtils->Close(KErrNone);
	delete theUtils;
	delete CActiveScheduler::Current();
	}


GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(ret, DoMainL());
	tests(ret==KErrNone);
	tests.Close();
	delete cleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}


EXPORT_C TInt WinsMain()
	{
#if defined(__WINS__)
	E32Main();
#endif
	return KErrNone;
	}



