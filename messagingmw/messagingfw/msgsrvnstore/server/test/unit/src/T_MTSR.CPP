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

#include <bautils.h>
#include <cmsvtechnologytypedefaultmtmsettings.h>
#include <msvreg.h>
#include <mtsr.h>
#include <msventry.h>
#include <mtclreg.h>
#include "Testsetup.h"

RTest test(_L("MTSR Registry Control Testrig"));
CServerMtmDllRegistry* serverDllRegistry;
CMtmRegistryControl* mtmregistryControl;

LOCAL_D const TUid KDummySrvMtmVersion1Uid={268440313};
LOCAL_D const TUid KDummyTechnologyTypeUid={268441126};
LOCAL_D const TUid KDummySrvMtmTypeUid={268440314};
LOCAL_D const TUid KDummySrvMtmTypeUid2={268440315};

LOCAL_D const TUid KEmailTechnologyTypeUid = { 0x10001671 };
LOCAL_D const TUid KSMTPServerMtmTypeUid = { 0x10001028 };
LOCAL_D const TUid KPOPServerMtmTypeUid = { 0x10001029 };

LOCAL_D const TUid KSendAsTechnologyTypeUid = { 0x1000127D };
LOCAL_D const TUid KSendAsServerMtmTypeUid = { 0x10204287 };


_LIT(KDummySrvMtmFilename, "z:\\system\\libs\\servermtm.dll");
LOCAL_D CMsvServer*	theServer;

_LIT(KMsvTestIndexFile, "\\Server MTM testing\\Index");
_LIT(KDataComponentFileName, "c:\\system\\mtm\\svrmtm.dat");
_LIT(KTestLocationOfMTMRegFiles, "c:\\system\\mtm\\");
_LIT(KDataComponentFileNameRsc, "mtm.rsc");
_LIT(KTestComponent, "msgs");
_LIT(KRomMtmDataFilePath, "Z:\\RESOURCE\\MESSAGING\\MTM\\");

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue

//

LOCAL_C void TestServerRegistryHeapFailureL()

//
// Test read file handling.
//
	{


// Open file

	TBool finished=EFalse;
	TInt failCount=0;
	while (!finished)
		{
		__UHEAP_FAILNEXT(failCount);
		CServerMtmDllRegistry* servermtmdllregistry=NULL;
		TRAPD(ret,servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession()));
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			delete servermtmdllregistry;
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(ret==KErrNoMemory);
			}
		failCount++;
		}

	
	}

LOCAL_C void RemoveCSystemMtmDirectoryL()

//
// Test read file handling.
//
	{


// Open file

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
		
		_LIT(KDefaultRegistryFileStoreName, "\\private\\1000484b\\Mtm Registry v2");
		TChar driveChar= theUtils->FileSession().GetSystemDriveChar();
		TBuf<2> systemDrive;
		systemDrive.Append(driveChar);
		systemDrive.Append(KDriveDelimiter);
		TPath pathName(systemDrive);
		pathName.Append(KDefaultRegistryFileStoreName);
		ret=theUtils->FileSession().Delete(pathName);
		if(ret != KErrNone && ret != KErrNotFound && ret != KErrPathNotFound)
			{
			User::Leave(ret);
			}
		

		}

	}

LOCAL_C void TestRegistryControlHeapFailureL()

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
		CServerMtmDllRegistry* servermtmdllregistry=NULL;
		servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		__UHEAP_FAILNEXT(failCount);
		CMtmRegistryControl* mtmregistrcontrol=NULL;
		TRAPD(ret,mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry));
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			delete mtmregistrcontrol;
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(); 
		failCount++;
		}

	
	}

LOCAL_C void TestRegistryControlFileFailureL()

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
		CServerMtmDllRegistry* servermtmdllregistry=NULL;
		servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		CMtmRegistryControl* mtmregistrcontrol=NULL;
		TRAPD(ret,mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry));
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			delete mtmregistrcontrol;
			finished=ETrue;
			}
		else 
			{
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(); 
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		failCount++;
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

	CMtmDllInfo* mtmdllinfo1=mtmdllinfo1=CMtmDllInfo::NewL(_L("My server dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KDummySrvMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=mtmdllinfo2=CMtmDllInfo::NewL(_L("My client dll info"),TUidType(KDynamicLibraryUid,KUidMtmClientComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KDummySrvMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	CMtmDllInfo* mtmdllinfo3=CMtmDllInfo::NewL(_L("My ui dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KDummySrvMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo3);

	CMtmDllInfo* mtmdllinfo4=mtmdllinfo4=CMtmDllInfo::NewL(_L("My ui data dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiDataComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KDummySrvMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo4);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KDummySrvMtmTypeUid, KDummyTechnologyTypeUid, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	theUtils->FileSession().MkDirAll(KTestLocationOfMTMRegFiles);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(),KDataComponentFileName,EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent,KDummySrvMtmVersion1Uid);
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
	TInt failCount=0;

	while (!finished)
		{
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
		__UHEAP_FAILNEXT(failCount);
		TUid mtmtypeuid;
		TInt ret=mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			test(ret==KErrNoMemory);
			__UHEAP_RESET;
			}
		CleanupStack::PopAndDestroy(2);
		failCount++;
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
	TInt failCount=0;

	while (!finished)
		{
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		__UHEAP_FAILNEXT(failCount);
		TUid mtmtypeuid;
		TInt ret=mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			test(ret==KErrNoMemory);
			__UHEAP_RESET;
			}
		CleanupStack::PopAndDestroy(2);
		failCount++;
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
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TUid mtmtypeuid;
		TInt ret=mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid);
		if (ret==KErrNone)
			{

			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
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
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TUid mtmtypeuid;
		TInt ret=mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid);
		if (ret==KErrNone)
			{

			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
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
	TInt failCount=0;

	while (!finished)
		{
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid));
		__UHEAP_FAILNEXT(failCount);
		TInt ret=mtmregistrcontrol->DeInstallMtmGroup(mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(2);
		failCount++;
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
	TInt failCount=0;

	while (!finished)
		{
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid));
		__UHEAP_FAILNEXT(failCount);
		TInt ret=mtmregistrcontrol->DeInstallMtmGroup(mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			test(ret==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(2);
		failCount++;
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
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=mtmregistrcontrol->DeInstallMtmGroup(mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
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
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		TInt ret=mtmregistrcontrol->DeInstallMtmGroup(mtmtypeuid);
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==0);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(servermtmdllregistry->NumRegisteredMtmDlls()==1);
			test(ret==KErrGeneral);
			}
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		failCount++;
		}
	}

LOCAL_C void TestNewServerMtmHeapFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{

		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid));
		CBaseServerMtm* servermtm=NULL;

		__UHEAP_FAILNEXT(failCount);
		TRAPD(ret,servermtm=servermtmdllregistry->NewServerMtmL(KDummySrvMtmTypeUid, CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId)));
		__UHEAP_RESET;

		if (ret==KErrNone)
			{
			test(servermtmdllregistry->IsInUse());
			test(servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm!=NULL);
			finished=ETrue;
			}
		else 
			{
			test(!servermtmdllregistry->IsInUse());
			test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm==NULL);
			test(ret==KErrNoMemory || ret == KErrNotFound);
			// The test for KerrNotFound was added after the defect fix DEF034133 - "WINS: Mailinit fails on clean baseline"
			//   NewServerMtmL calls CRegisteredMtmDll::GetLibrary which ultimately
			//   calls CRegisteredMtmDll::GetLibrary.  GetLibrary no longer returns
			//   specific error codes on failure, but instead returns KErrNotFound if 
			//   it cannot complete successfully.
			}
		delete servermtm;
		test(!servermtmdllregistry->IsInUse());
		test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
		CleanupStack::PopAndDestroy(2);
		failCount++;

		}

	
	}

LOCAL_C void TestNewServerMtmRscHeapFailureL()
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

		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid));
		CBaseServerMtm* servermtm=NULL;

		__UHEAP_FAILNEXT(failCount);
		TRAPD(ret,servermtm=servermtmdllregistry->NewServerMtmL(KDummySrvMtmTypeUid, CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId)));
		__UHEAP_RESET;

		if (ret==KErrNone)
			{
			test(servermtmdllregistry->IsInUse());
			test(servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm!=NULL);
			finished=ETrue;
			}
		else 
			{
			test(!servermtmdllregistry->IsInUse());
			test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm==NULL);
			test(ret==KErrNoMemory || ret==KErrNotFound);
			// The test for KerrNotFound was added after the defect fix DEF034133 - "WINS: Mailinit fails on clean baseline"
			//   NewServerMtmL calls CRegisteredMtmDll::GetLibrary which ultimately
			//   calls CRegisteredMtmDll::GetLibrary.  GetLibrary no longer returns
			//   specific error codes on failure, but instead returns KErrNotFound if 
			//   it cannot complete successfully.
			}
		delete servermtm;
		test(!servermtmdllregistry->IsInUse());
		test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
		CleanupStack::PopAndDestroy(2);
		failCount++;

		}
	}

LOCAL_C void TestNewServerMtmFileFailureL()

//
// Test read file handling.
//
	{


// Open file



	TBool finished=EFalse;
	TInt failCount=0;

	while (!finished)
		{
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		CreateDataComponentFileStoreL();
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(KDataComponentFileName,mtmtypeuid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		CBaseServerMtm* servermtm=NULL;
		TRAPD(ret,servermtm=servermtmdllregistry->NewServerMtmL(KDummySrvMtmTypeUid, CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId)));
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->IsInUse());
			test(servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm!=NULL);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(!servermtmdllregistry->IsInUse());
			test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm==NULL);
//			test(ret==KErrGeneral);
			}
		delete servermtm;
		test(!servermtmdllregistry->IsInUse());
		test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		failCount++;
		}

	
	}

LOCAL_C void TestNewServerMtmRscFileFailureL()
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
		CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
		CleanupStack::PushL(servermtmdllregistry);
		RemoveCSystemMtmDirectoryL();
		CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
		CleanupStack::PushL(mtmregistrcontrol);
		TUid mtmtypeuid;
		User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid));
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrGeneral,failCount);
#endif
		CBaseServerMtm* servermtm=NULL;
		TRAPD(ret,servermtm=servermtmdllregistry->NewServerMtmL(KDummySrvMtmTypeUid, CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId)));
		if (ret==KErrNone)
			{
			test(servermtmdllregistry->IsInUse());
			test(servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm!=NULL);
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(!servermtmdllregistry->IsInUse());
			test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
			test(servermtm==NULL);
			}
		delete servermtm;
		test(!servermtmdllregistry->IsInUse());
		test(!servermtmdllregistry->IsInUse(KDummySrvMtmTypeUid));
		CleanupStack::PopAndDestroy(2);
#if defined(_DEBUG)
		theUtils->FileSession().SetErrorCondition(KErrNone,0);
#endif
		failCount++;
		}
	}


LOCAL_C void TestCheckMtmRscSecurityCapabilitiesL()
	{
	// Test reading security capabilities from resource file
	// Resolve file name
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KDataComponentFileNameRsc, parse));

	CServerMtmDllRegistry* servermtmdllregistry=CServerMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(servermtmdllregistry);
	RemoveCSystemMtmDirectoryL();
	CMtmRegistryControl* mtmregistrcontrol=CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
	CleanupStack::PushL(mtmregistrcontrol);
	TUid mtmtypeuid;
	User::LeaveIfError(mtmregistrcontrol->InstallMtmGroup(parse.FullName(),mtmtypeuid));
	CMtmGroupData* mtmgroupdata = mtmregistrcontrol->GetMtmGroupDataL(mtmtypeuid);
	CleanupStack::PushL(mtmgroupdata);

	TCapabilitySet mtmRequiredCaps = mtmgroupdata->GetMtmRequiredCapabilities();
	for(TUint ii=ECapabilityTCB ; ii<ECapability_HardLimit; ++ii)
		{
		TCapability cap = static_cast<TCapability>(ii);
		if(cap == ECapabilityLocalServices || cap == ECapabilityPowerMgmt)
			{
			test(mtmRequiredCaps.HasCapability(cap));
			}
		else
			{			
			test(!mtmRequiredCaps.HasCapability(cap));
			}
		}
	
	CleanupStack::PopAndDestroy(mtmgroupdata);
	User::LeaveIfError(mtmregistrcontrol->DeInstallMtmGroup(mtmtypeuid));
	CleanupStack::PopAndDestroy(2, servermtmdllregistry); // mtmregistrcontrol, servermtmdllregistry
	}

LOCAL_C void TestTechnologyTypeDefaultMtmSettingsL()
	{
	// First install the available MTMs
	CServerMtmDllRegistry* serverMtmReg = CServerMtmDllRegistry::NewL( theUtils->FileSession( ) );
	CleanupStack::PushL( serverMtmReg );
	CMtmRegistryControl* mtmRegControl = CMtmRegistryControl::NewL( theUtils->FileSession( ), 
																	*serverMtmReg );
	CleanupStack::PushL( mtmRegControl );
	
	TFileName filename( KRomMtmDataFilePath );
	TEntry entry;

	// scan for any mtm data files and try to load them
	if ( theUtils->FileSession( ).Entry( filename, entry ) == KErrNone )  
		{
		CDir* filelist=NULL;
		if ( theUtils->FileSession( ).GetDir( filename, 
											  KEntryAttNormal|KEntryAttAllowUid, 
											  ESortByName, 
											  filelist ) == KErrNone )
			{
			CleanupStack::PushL( filelist );	
			TInt i=filelist->Count( );
			TParse fileEntry;
			TPtrC entryName;
			//check we have files
			if( i > 0 )
				{
				//get the first entry to be dealt with and register it
				i--;
				entry=( *filelist )[i];
				fileEntry.Set( entry.iName, NULL, NULL );
				entryName.Set( fileEntry.Name( ) );

				while( i >= 0 )
					{
					filename = KRomMtmDataFilePath;
					filename.Append( entry.iName );
					TUid mtmtypeuid;

					// Use the file most appropriate to the current language
					BaflUtils::NearestLanguageFile( theUtils->FileSession( ), filename );

					TInt err = mtmRegControl->InstallMtmGroup( filename, mtmtypeuid );
					//search for the next entry with a different filename
					while( i-- )
						{
						TPtrC prevEntry=fileEntry.Name( );
						//get next entry
						entry=( *filelist )[i];
						TParse nextFileEntry;
						nextFileEntry.Set( entry.iName, NULL, NULL);
						entryName.Set( nextFileEntry.Name( ) );
						if(entryName.Compare( prevEntry )!=0)
							{
							//different filename
							break;
							}
						}
					}
				}
			CleanupStack::PopAndDestroy();
			}
		}
	CleanupStack::PopAndDestroy( 2, serverMtmReg );	// mtmRegControl, serverMtmReg
	
	// Do the tests
	CMsvTechnologyTypeDefaultMtmSettings* techTypeDefaultMtmSettings = 
			CMsvTechnologyTypeDefaultMtmSettings::NewL( theUtils->FileSession( ) );
	CleanupStack::PushL( techTypeDefaultMtmSettings );
	
	TUid mtmUid = { NULL };
	
	// Set a default MTM for the email technology type
	techTypeDefaultMtmSettings->SetDefaultMtmL( KEmailTechnologyTypeUid, KSMTPServerMtmTypeUid );
	techTypeDefaultMtmSettings->SaveSettingsL( );

	// Try setting an unregisterd mtm
	TRAPD( error, techTypeDefaultMtmSettings->SetDefaultMtmL( KDummyTechnologyTypeUid, 
															  KDummySrvMtmTypeUid ) );
	test( error == KErrArgument );
	
	// the uid should still be the correct one
	error = techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid );
	test( error == KErrNone );
	test( mtmUid == KSMTPServerMtmTypeUid );
	
	// Try setting a registered mtm with a wrong technology type uid
	TRAP( error, techTypeDefaultMtmSettings->SetDefaultMtmL( KEmailTechnologyTypeUid, 
															 KSendAsServerMtmTypeUid ) );
	test( error == KErrArgument );
	
	// the uid should still be the correct one
	error = techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid );
	test( error == KErrNone );
	test( mtmUid == KSMTPServerMtmTypeUid );
	
	// Try setting a new email mtm, but do not save
	techTypeDefaultMtmSettings->SetDefaultMtmL( KEmailTechnologyTypeUid, KPOPServerMtmTypeUid );
	error = techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid );
	test( error == KErrNone );
	test( mtmUid == KPOPServerMtmTypeUid );
	
	// Load the stored settings again
	techTypeDefaultMtmSettings->LoadSettingsL( );
	User::LeaveIfError( techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid ) );
	test( mtmUid == KSMTPServerMtmTypeUid );
	
	// Remove the default MTM
	User::LeaveIfError( techTypeDefaultMtmSettings->RemoveDefaultMtm( KEmailTechnologyTypeUid ) );
	error = techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid );
	test( error == KErrNotFound );
	
	techTypeDefaultMtmSettings->SaveSettingsL( );
	techTypeDefaultMtmSettings->LoadSettingsL( );
	error = techTypeDefaultMtmSettings->DefaultMtm( KEmailTechnologyTypeUid, mtmUid );
	test( error == KErrNotFound );
	
	CleanupStack::PopAndDestroy( techTypeDefaultMtmSettings );

	}

LOCAL_C void doTestsL()
    {
	theUtils->FileSession().MkDirAll(KMsvTestIndexFile);
	theUtils->FileSession().Delete(KMsvTestIndexFile);

	theServer = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(theServer);

	CMsgsTestUtils::Start(_L("Create Server Registry with heap failure"));
	TestServerRegistryHeapFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Create Registry Control with heap failure"));
	TestRegistryControlHeapFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Create Registry Control with file failure"));
	TestRegistryControlFileFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Install Mtm with heap failure"));
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
	
	CMsgsTestUtils::Next(_L("New Server Mtm with heap failure"));
	TestNewServerMtmHeapFailureL();
	TestNewServerMtmRscHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("New Server Mtm with file failure"));
	TestNewServerMtmFileFailureL();
	TestNewServerMtmRscFileFailureL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Check MTM security capabilities"));
	TestCheckMtmRscSecurityCapabilitiesL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(theServer);

	CMsgsTestUtils::Next(_L("Check TechnologyType Default Mtm Settings "));
	TestTechnologyTypeDefaultMtmSettingsL();
	CMsgsTestUtils::Complete();
    }

#pragma warning (default : 4238)
