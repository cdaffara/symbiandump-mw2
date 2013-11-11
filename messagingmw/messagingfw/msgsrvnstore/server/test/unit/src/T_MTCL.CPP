// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <f32fsys.h>
#include <msvreg.h>
#include <msvids.h>
#include <msvuids.h>
#include <mtclbase.h>
#include <mtclreg.h>
#include "MTSR.H"  //  Only for KUidDataComponent
#include "Testsetup.h"

RTest test(_L("T_MTCL test harness"));

const TUid KDummySrvMtmTypeUid={268440314};



// once the __SUPPORT_MESSAGING_API_V1__ macro is removed
// CMtmDllInfo streaming requires the v2 UID

const TUid KDummySrvMtmVersionUid = {0x10202D51};  




_LIT(KServerMtmDllName, "z:\\system\\libs\\servermtm.dll");
_LIT(KClientMtmDllName, "z:\\system\\libs\\clientmtm.dll");
_LIT(KUIMtmDllName, "z:\\system\\libs\\dummyuimtm.dll");
_LIT(KUIDataMtmDllName, "z:\\system\\libs\\dummyuidatamtm.dll");


_LIT(KDataComponentFileName, "c:\\system\\mtm\\clntmtm.dat");
_LIT(KDataComponentFileNameRsc, "mtm.rsc");
_LIT(KTestComponent, "msgs");

LOCAL_C void RemoveCSystemMtmDirectoryL()

//
// Test read file handling.
//
	{
	TEntry entry;
	TFileName path=_L("C:\\SYSTEM\\MTM\\");
	TInt ret=theUtils->FileSession().Entry(path,entry);  //  Check path exists
	if (ret!=KErrNotFound)
		{
		User::LeaveIfError(ret);
		TFileName filename=path;
#if defined (__WINS__)
		filename.Append(_L("*"));
#else
		filename.Append(_L("*.dat"));
#endif
		CDir* filelist=NULL;
		User::LeaveIfError(theUtils->FileSession().GetDir(filename,KEntryAttNormal|KEntryAttAllowUid,ESortByName,filelist));
		CleanupStack::PushL(filelist);
		for (TInt i=filelist->Count()-1; i>=0 ; i--) 
			{
			entry=(*filelist)[i];
			filename=path;
			filename.Append(entry.iName);
			User::LeaveIfError(theUtils->FileSession().Delete(filename));
			}
		CleanupStack::PopAndDestroy(); //  filelist
#if defined (__WINS__)
		User::LeaveIfError(theUtils->FileSession().RmDir(path));
#else
		filename=path; //  We don't want to delete directory under MARM because the MTM DLLs live there
		filename.Append(_L("MTM Registry v2"));
		ret=theUtils->FileSession().Entry(filename,entry);  //  Check path exists
		if (ret!=KErrNotFound)
			{
			User::LeaveIfError(ret);
			User::LeaveIfError(theUtils->FileSession().Delete(filename));  
			}
#endif
		}

	}




LOCAL_C void CreateDataComponentFileStoreL()

//
// Test read file handling.
//
	{


// Open file

//	CMsgsTestUtils::Next(_L("Create Data Component FileStore"));

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);
	
	
	

	CMtmDllInfo *mtmdllinfo1 = CMtmDllInfo::NewL( _L("My server dll info"),
												  TUidType(KDynamicLibraryUid,KUidMtmServerComponent,KDummySrvMtmVersionUid), 
												  KServerMtmDllName, 1, TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo *mtmdllinfo2 = CMtmDllInfo::NewL( _L("My client dll info"),
												  TUidType(KDynamicLibraryUid,KUidMtmClientComponent,KDummySrvMtmVersionUid), 
												  KClientMtmDllName, 1, TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	CMtmDllInfo *mtmdllinfo3 = CMtmDllInfo::NewL( _L("My ui dll info"),
												  TUidType(KDynamicLibraryUid,KUidMtmUiComponent,KDummySrvMtmVersionUid), 
												  KUIMtmDllName, 1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo3);

	CMtmDllInfo *mtmdllinfo4 = CMtmDllInfo::NewL( _L("My ui data dll info"),
												  TUidType(KDynamicLibraryUid,KUidMtmUiDataComponent,KDummySrvMtmVersionUid), 
												  KUIDataMtmDllName, 1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo4);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	capSet.AddCapability(ECapabilityLocalServices);
	capSet.AddCapability(ECapabilityPowerMgmt);
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData *mtmgroupdata = CMtmGroupData::NewL(KDummySrvMtmTypeUid,KDummySrvMtmTypeUid, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	TParse parsedname;
	User::LeaveIfError(parsedname.Set(KDataComponentFileName,NULL,NULL));
	TInt ret=theUtils->FileSession().MkDirAll(parsedname.DriveAndPath());
	if (ret!=KErrAlreadyExists)
		User::LeaveIfError(ret);
	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(),parsedname.FullName(),EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent,KDummySrvMtmVersionUid);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);							// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(); // filestore
	
	CleanupStack::PopAndDestroy();  // mtmgroupdata

	}

LOCAL_C void TestInstallMtmHeapFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

		__UHEAP_FAILNEXT(failCount++);
		TInt ret=session->InstallMtmGroup(KDataComponentFileName);
		__UHEAP_RESET;

		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);

		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(4);
		}

	
	}

LOCAL_C void TestInstallMtmRscHeapFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));
	
	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

		__UHEAP_FAILNEXT(failCount++);
		TInt ret=session->InstallMtmGroup(parse.FullName());
		__UHEAP_RESET;

		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);

		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(4);
		}
	}

LOCAL_C void TestInstallMtmFileFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=session->InstallMtmGroup(KDataComponentFileName);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);
		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(4);
		failCount++;
		}

	
	}

LOCAL_C void TestInstallMtmRscFileFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=session->InstallMtmGroup(parse.FullName());
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);
		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(4);
		failCount++;
		}
	}

LOCAL_C void TestDeInstallMtmHeapFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
		__UHEAP_FAILNEXT(failCount++);
		TInt ret=session->DeInstallMtmGroup(KDataComponentFileName);
		__UHEAP_RESET;
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);

		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrNoMemory);
			User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
			}
		CleanupStack::PopAndDestroy(4);
		}

	
	}

LOCAL_C void TestDeInstallMtmRscHeapFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

		User::LeaveIfError(session->InstallMtmGroup(parse.FullName()));
		__UHEAP_FAILNEXT(failCount++);
		TInt ret=session->DeInstallMtmGroup(parse.FullName());
		__UHEAP_RESET;
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);

		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrNoMemory);
			User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
			}
		CleanupStack::PopAndDestroy(4);
		}
	}

LOCAL_C void TestDeInstallMtmFileFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;
	
		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=session->DeInstallMtmGroup(KDataComponentFileName);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);
		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrGeneral);
			User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
			}
		CleanupStack::PopAndDestroy(4);
		failCount++;
		}

	
	}

LOCAL_C void TestDeInstallMtmRscFileFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CTestActive* active = new (ELeave) CTestActive;
		CleanupStack::PushL(active);

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;
	
		User::LeaveIfError(session->InstallMtmGroup(parse.FullName()));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=session->DeInstallMtmGroup(parse.FullName());
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);
		active->StartL();
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount);
			test(!clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid));                                
			test(ret==KErrGeneral);
			User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
			}
		CleanupStack::PopAndDestroy(4);
		failCount++;
		}
	}

LOCAL_C void TestClientRegistryHeapFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();

		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		TInt mtmCount = clientmtmregistry->NumRegisteredMtmDlls();
		delete clientmtmregistry;

		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(ret,clientmtmregistry=CClientMtmRegistry::NewL(*session));
		__UHEAP_RESET;
		if (ret==KErrNone)
			{
			test(clientmtmregistry->NumRegisteredMtmDlls()==mtmCount+1);
			test(clientmtmregistry->IsPresent(KDummySrvMtmTypeUid)); 
			delete clientmtmregistry;
			finished=ETrue;
			}
		else 
			{
			test(ret==KErrNoMemory);
			}
		User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
		CleanupStack::PopAndDestroy(2);
		}

	
	}

LOCAL_C void TestNewClientMtmHeapFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		__UHEAP_FAILNEXT(failCount++);
		CBaseMtm* mtm=NULL;
		TRAPD(ret,mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid));
		__UHEAP_RESET;
		if (ret==KErrNone)
			{
			test(clientmtmregistry->IsInUse());
			test(clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			delete mtm;
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			test(ret==KErrNoMemory || ret==KErrNotFound);
			// The test for KerrNotFound was added after the defect fix DEF034133 - "WINS: Mailinit fails on clean baseline"
			//   NewServerMtmL calls CRegisteredMtmDll::GetLibrary which ultimately
			//   calls CRegisteredMtmDll::GetLibrary.  GetLibrary no longer returns
			//   specific error codes on failure, but instead returns KErrNotFound if 
			//   it cannot complete successfully.
			}
		delete clientmtmregistry;
		User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
		CleanupStack::PopAndDestroy(2);
		}

	
	}

LOCAL_C void TestNewClientMtmRscHeapFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		User::LeaveIfError(session->InstallMtmGroup(parse.FullName()));
		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		__UHEAP_FAILNEXT(failCount++);
		CBaseMtm* mtm=NULL;
		TRAPD(ret,mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid));
		__UHEAP_RESET;
		if (ret==KErrNone)
			{
			test(clientmtmregistry->IsInUse());
			test(clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			delete mtm;
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			test(ret==KErrNoMemory || ret==KErrNotFound);
			// The test for KerrNotFound was added after the defect fix DEF034133 - "WINS: Mailinit fails on clean baseline"
			//   NewServerMtmL calls CRegisteredMtmDll::GetLibrary which ultimately
			//   calls CRegisteredMtmDll::GetLibrary.  GetLibrary no longer returns
			//   specific error codes on failure, but instead returns KErrNotFound if 
			//   it cannot complete successfully.
			}
		delete clientmtmregistry;
		User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
		CleanupStack::PopAndDestroy(2);
		}
	}

LOCAL_C void TestNewClientMtmFileFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		CBaseMtm* mtm=NULL;
		TRAPD(ret,mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		if (ret==KErrNone)
			{
			test(clientmtmregistry->IsInUse());
			test(clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			delete mtm;
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
//			test(ret==KErrNoMemory);  //  We can't guarantee error
			}
		delete clientmtmregistry;
		User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
		CleanupStack::PopAndDestroy(2);
		failCount++;
		}

	
	}

LOCAL_C void TestNewClientMtmRscFileFailureL()
//
// Test read file handling.
//
	{
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		User::LeaveIfError(session->InstallMtmGroup(parse.FullName()));
		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		CBaseMtm* mtm=NULL;
		TRAPD(ret,mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		if (ret==KErrNone)
			{
			test(clientmtmregistry->IsInUse());
			test(clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			delete mtm;
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
			finished=ETrue;
			}
		else 
			{
			test(!clientmtmregistry->IsInUse());
			test(!clientmtmregistry->IsInUse(KDummySrvMtmTypeUid)); 
//			test(ret==KErrNoMemory);  //  We can't guarantee error
			}
		delete clientmtmregistry;
		User::LeaveIfError(session->DeInstallMtmGroup(parse.FullName()));                               
		CleanupStack::PopAndDestroy(2);
		failCount++;
		}
	}

LOCAL_C CMsvEntry* CreateEntryL(CMsvSession& aSession)

//
// Test read file handling.
//
	{


// Open file

	CMsvEntry* msventry=CMsvEntry::NewL(aSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(msventry);

	TMsvEntry entry;
	entry.iDescription.Set(_L("Description"));
	entry.iDetails.Set(_L("Details"));
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm = KDummySrvMtmTypeUid;

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	active->StartL();
	CMsvOperation*  opert = msventry->CreateL(entry, active->iStatus);
	CleanupStack::PushL(opert);
	CActiveScheduler::Start(); // operation complete
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalNew);
	test(package().iTotalNumberOfEntries==1);

	test(package().iError==KErrNone);
	test(package().iNumberCompleted==1);
	test(package().iNumberFailed==0);
	*(TMsvId*)&entry = package().iId;

	test(package().iNumberRemaining==0);

	CleanupStack::PopAndDestroy(2); // active

	msventry->SetEntryL(entry.Id());
	CleanupStack::Pop();  // msventry
	return msventry;
	}

LOCAL_C void TestNewEntryHeapFailureL()

//
// Test read file handling.
//
	{


// Open file


	TBool finished=EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	while (!finished)
		{
		RemoveCSystemMtmDirectoryL();
		CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
		CleanupStack::PushL(sessionObserver);
		CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
		CleanupStack::PushL(session);
		CreateDataComponentFileStoreL();
		User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));
		CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
		CleanupStack::PushL(clientmtmregistry);
		CBaseMtm* mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid);
		CleanupStack::PushL(mtm);
		test(!mtm->HasContext());
		CMsvEntry* msventry=CreateEntryL(*session);
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(ret,mtm->SetCurrentEntryL(msventry));
		__UHEAP_RESET;
		if (ret==KErrNone)
			{
			test(mtm->HasContext());
			test(mtm->Entry().Entry().iMtm==KDummySrvMtmTypeUid);
			finished=ETrue;
			}
		else 
			{
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(2);
		User::LeaveIfError(session->DeInstallMtmGroup(KDataComponentFileName));                               
		CleanupStack::PopAndDestroy(2);
		}

	
	}
	
	
LOCAL_C void TestClientMTMDefaultServiceL()

//
// Test read file handling.
//
	{
	RemoveCSystemMtmDirectoryL();
	CDummyObserver* sessionObserver = new (ELeave) CDummyObserver;
	CleanupStack::PushL(sessionObserver);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
	CleanupStack::PushL(session);
	CreateDataComponentFileStoreL();
	User::LeaveIfError(session->InstallMtmGroup(KDataComponentFileName));

	
	CMsvEntry *entry=session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entry);
	
	// remove all existing services for this MTM
	CMsvEntrySelection *sel=entry->ChildrenWithMtmL(KDummySrvMtmTypeUid);
	CleanupStack::PushL(sel);
	TInt count=sel->Count();
	while(count--)
		{
		entry->DeleteL(sel->At(count));
		}
	CleanupStack::PopAndDestroy(sel);
		
	// create a service for this MTM
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm	= KDummySrvMtmTypeUid;
	service.iServiceId = KMsvNullIndexEntryId;
	entry->CreateL(service);
	CleanupStack::PopAndDestroy(entry);
	
	CClientMtmRegistry* clientmtmregistry=CClientMtmRegistry::NewL(*session);
	CleanupStack::PushL(clientmtmregistry);
	CBaseMtm* mtm=clientmtmregistry->NewMtmL(KDummySrvMtmTypeUid);
	CleanupStack::PushL(mtm);
	
	
	TMsvId tocheck=KMsvNullIndexEntryId;
	// test the remove default service does nothing
	mtm->RemoveDefaultServiceL();
	// test the default service function returns the service we craeted
	tocheck=mtm->DefaultServiceL();
	test(tocheck==service.Id());
	// test the set default service does nothing
	mtm->ChangeDefaultServiceL(KMsvNullIndexEntryId);
	tocheck=mtm->DefaultServiceL();
	test(tocheck==service.Id());
		
	CleanupStack::PopAndDestroy(4,sessionObserver);
	}

void doTestsL()
    {
//Running only in winscw as these are heap related test cases
#if defined (__WINS__)
    CMsgsTestUtils::Start(_L("Install Mtm with heap failure"));
	TestInstallMtmHeapFailureL();
	TestInstallMtmRscHeapFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Install Mtm with file failure"));
	TestInstallMtmFileFailureL();
	TestInstallMtmRscFileFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("DeInstall Mtm with heap failure"));
	TestDeInstallMtmHeapFailureL();
	TestDeInstallMtmRscHeapFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("DeInstall Mtm with file failure"));
	TestDeInstallMtmFileFailureL();
	TestDeInstallMtmRscFileFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("New Client Registry with heap failure"));
	TestClientRegistryHeapFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("New Client Mtm with heap failure"));
	TestNewClientMtmHeapFailureL();
	TestNewClientMtmRscHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("New Client Mtm with file failure"));
	TestNewClientMtmFileFailureL();
	TestNewClientMtmRscFileFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("New Entry with heap failure"));
	TestNewEntryHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Check base implementations of the default service functions"));
	TestClientMTMDefaultServiceL();
	CMsgsTestUtils::Complete();
#endif
     }
