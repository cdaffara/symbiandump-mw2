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
// msvtestutils.cpp
//

#include "msvtestutils.h"
#include "MSVSERV.H"
#include <e32uid.h>
#include <mtsr.h>
#include <msvruids.h>
#include <txtfmlyr.h>
#include <txtetext.h>
#include <txtrich.h>
#include <miuthdr.h>
#include <mtclbase.h>
#include <flogger.h>
#include <msventryscheduledata.h>
#include <csch_cli.h>
#include <sqldb.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#include "timrfc822datefield.h"
#endif

_LIT(KStoreInitFileName, "\\private\\1000484b\\StoreInit.tmp");
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
#endif


//
//
//	TestMsvOperationTimer
//
//

EXPORT_C TestMsvOperationTimer* TestMsvOperationTimer::NewLC(CConsoleBase* aConsole, CMsvOperation* aMsvOperation, RTest& aRTest)
	{
	TestMsvOperationTimer* self = new(ELeave) TestMsvOperationTimer(aConsole, aMsvOperation, aRTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C TestMsvOperationTimer::TestMsvOperationTimer(CConsoleBase* aConsole, CMsvOperation* aMsvOperation, RTest& aRTest)
	: CTimer(EPriorityStandard+3), iMsvOperation(aMsvOperation), iRTest(aRTest)
	{
	iConsole = aConsole;
	iPeriod = KPeriod;
	iClosing = EFalse;
	iCount = 0;
	}

EXPORT_C void TestMsvOperationTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	}

EXPORT_C void TestMsvOperationTimer::IssueRequest()
	{
	After(iPeriod);
	}

EXPORT_C void TestMsvOperationTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

EXPORT_C void TestMsvOperationTimer::RunL()
	{
	// display the current progress
	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);

	const TDesC8& progBuf = iMsvOperation->ProgressL();	
	paramPack.Set(progBuf);
	TMsvId progress=paramPack();	

	iRTest.Console()->SetPos(0, 10);
	iRTest.Printf(_L("   MessageId %d		\n"),progress);

	IssueRequest();
	};


//
//
//	CMsvTestUtils
//
//

EXPORT_C CMsvTestUtils::CMsvTestUtils(RTest& aRTest)
: CTestUtils(aRTest)
	{
	}

EXPORT_C CMsvTestUtils::~CMsvTestUtils()
	{
	Reset();

	delete iServerMtmDllRegistry;
	delete iMtmRegistryControl;
	}

EXPORT_C void CMsvTestUtils::Reset()
	{
	delete iMsvEntry;
	iMsvEntry = NULL;

	delete iServerEntry;
	iServerEntry = NULL;

	delete iMsvServer;
	iMsvServer = NULL;

	delete iClientMtmRegistry;
	iClientMtmRegistry = NULL;

	if(iMsvSession!=NULL)
		{
		iMsvSession->CloseMessageServer();	
		}
		
	delete iMsvSession;
	iMsvSession = NULL;

	delete iDummyObserver;
	iDummyObserver = NULL;

	iClientServer = ENeither;
	SpecifyRfc822Dir(KRfc822Files);	
	}

EXPORT_C CBaseMtm* CMsvTestUtils::InstantiateClientMtmL(TUid aMtmType, TMsvId aServiceId)
	{
	// client side
	__ASSERT_ALWAYS(iClientServer == EClientSide, Panic(KErrGeneral));

	if (!iClientMtmRegistry)
		{
		iClientMtmRegistry = CClientMtmRegistry::NewL(*iMsvSession);
		}

	CBaseMtm* newMtm = iClientMtmRegistry->NewMtmL(aMtmType);

	CMsvEntry* entry = iMsvSession->GetEntryL(aServiceId);
	CleanupStack::PushL(entry);
	newMtm->SetCurrentEntryL(entry);
	CleanupStack::Pop(); //entry
	return newMtm;
	}

EXPORT_C CBaseServerMtm* CMsvTestUtils::InstantiateServerMtmL(TUid aMtmType, TMsvId aServiceId)
	{
	// server side
	__ASSERT_ALWAYS(iServerMtmDllRegistry, Panic(KErrGeneral));
	__ASSERT_ALWAYS(iClientServer == EServerSide, Panic(KErrNotSupported));
	User::LeaveIfError(iServerEntry->SetEntry(aServiceId));  	
	CBaseServerMtm* newMtm = iServerMtmDllRegistry->NewServerMtmL(aMtmType, iServerEntry);
	return newMtm;
	}


EXPORT_C void CMsvTestUtils::GoClientSideL()
	{
	Reset();
 	iFs.SetSessionPath(iDriveName);
	iDummyObserver = new (ELeave) TMsvDummyObserver;
	iMsvSession = CMsvSession::OpenSyncL(*iDummyObserver);
	iMsvEntry = CMsvEntry::NewL(*iMsvSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	iClientServer = EClientSide;
	}

EXPORT_C void CMsvTestUtils::GoServerSideL()
	{
	Reset();
	
	_LIT(KMsvServerPattern, "!MsvServer*");
	TFindProcess findprocess(KMsvServerPattern);
	TFullName name;

	// wait for the server to close before trying to start
	// one within this process.
	for(;;)
		{	
		TFindServer find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			break;
		User::After(100000);
		}

	iMsvServer = CMsvServer::NewL();
	
	// wait a couple of seconds
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(5000000);
	CActiveScheduler::Start();
	delete timer;

	iServerEntry = CMsvServerEntry::NewL(*iMsvServer, KMsvRootIndexEntryId);
	iClientServer = EServerSide;
	}

EXPORT_C void CMsvTestUtils::CreateServerMtmRegL(const TUid aMsgType, const TDesC& aHumanReadableName, const TMsvTestDllInfo& aServerMtm, const TMsvTestDllInfo& aClientMtm, const TMsvTestDllInfo& aUiMtm, const TMsvTestDllInfo& aUiDataMtm, const TUid aGroup, const TDesC& aDatFile)
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(aHumanReadableName,TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),aServerMtm.iFileName,aServerMtm.iOrdinal,aServerMtm.iVersion);
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(aHumanReadableName,TUidType(KDynamicLibraryUid,KUidMtmClientComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),aClientMtm.iFileName,aClientMtm.iOrdinal,aClientMtm.iVersion);
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	CMtmDllInfo* mtmdllinfo3=CMtmDllInfo::NewL(aHumanReadableName,TUidType(KDynamicLibraryUid,KUidMtmUiComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),aUiMtm.iFileName,aUiMtm.iOrdinal,aUiMtm.iVersion);
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo3);

	CMtmDllInfo* mtmdllinfo4=CMtmDllInfo::NewL(aHumanReadableName,TUidType(KDynamicLibraryUid,KUidMtmUiDataComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),aUiDataMtm.iFileName,aUiDataMtm.iOrdinal,aUiDataMtm.iVersion);
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo4);

	// Create an empty capability set for creating a new group data object
	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(aMsgType, aGroup, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(FileSession(), aDatFile, EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid, KUidMsvDataComponent, aMsgType);

	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);	// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(2, mtmgroupdata); // filestore, mtmgroupdata
	}

EXPORT_C TMsvId CMsvTestUtils::CreateServiceL(const TUid aMtm, TBool aReadOnly, TBool aVisible)
	{
	TMsvId id = 0;
	TMsvEntry entry;
	entry.iMtm = aMtm;
	entry.iType = KUidMsvServiceEntry;
	entry.SetReadOnly(aReadOnly);
	entry.SetVisible(aVisible);
	
	SetEntryL(KMsvRootIndexEntryId);
	CreateEntryL(entry);
	id = entry.Id();
	SetEntryL(id);
	return id;
	}


EXPORT_C void CMsvTestUtils::CreateRegistryObjectAndControlL()
	{
	//	Create the registry object
	if (!iServerMtmDllRegistry)
		iServerMtmDllRegistry = CServerMtmDllRegistry::NewL(iFs);

	//	Create the registry control..
	if (!iMtmRegistryControl)
		iMtmRegistryControl = CMtmRegistryControl::NewL(iFs, *iServerMtmDllRegistry);

	iRegistryObjectAndControlCreated = ETrue;
	}

EXPORT_C void CMsvTestUtils::InstallMtmGroupL(const TDesC& aDatFile)
	{
	TInt err = KErrNone;
	
	if (iClientServer == EClientSide)
		{
		err = iMsvSession->InstallMtmGroup(aDatFile);
		}
	else
		{
		User::LeaveIfNull(iServerMtmDllRegistry);
		User::LeaveIfNull(iMtmRegistryControl);

		//	Actually load the group data...
		TUid aUid;
		err = iMtmRegistryControl->InstallMtmGroup(aDatFile, aUid);
		}

	if (err != KErrAlreadyExists)
		User::LeaveIfError(err);
	}

EXPORT_C void CMsvTestUtils::CleanMessageFolderL()
	{

	//Kill the message server if its running
	_LIT(KMsvServerPattern, "!MsvServer*");
	TFindProcess findprocess(KMsvServerPattern);
	TFullName name;
	if(findprocess.Next(name)==KErrNone)
		{
		RProcess process;
    	User::LeaveIfError(process.Open(findprocess));
     	process.Kill(KErrCancel) ;
		process.Close() ;
		}
	
	// Wait for the server to close before trying to remove
	// the message folder
	
	TInt loopCount = 0;
	FOREVER
		{
		TFindServer find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			break;
		User::After(1000000);
		++loopCount;
		if(loopCount > 5)
			{
			User::Invariant();
			}
		}
	
	// remove the drive from the system ini 
	CDictionaryFileStore* store = CDictionaryFileStore::SystemLC(iFs);
	if (store->IsPresentL(KUidMsvMessageDriveStream))
		{
		store->RemoveL(KUidMsvMessageDriveStream); 
		store->CommitL();
		}
	CleanupStack::PopAndDestroy(); // store

	CFileMan* fileMan = CFileMan::NewL(iFs); 
	CleanupStack::PushL(fileMan);
	TParse parse;
	TInt error;
	TFileName fileName(KMsvDefaultFolder2); 
	
	TChar driveChar=FileSession().GetSystemDriveChar();
 	TBuf<2> systemDrive = iDriveName;

	parse.Set(fileName, &systemDrive, NULL);
	error = fileMan->RmDir(parse.DriveAndPath()); 
	error = iFs.RmDir(parse.DriveAndPath()); 
	if (!(error==KErrNotFound||error==KErrNone || error == KErrPathNotFound))
		{
        TPtrC driveAndPath = parse.DriveAndPath();
		Printf(_L("Directory %S cannot be removed. "), &driveAndPath);
		Printf(_L("Please ensure directory is not in use.\n"));
		User::Leave(KErrAccessDenied);
		}
	
	//delete DBs in C:, D: and E:.
	TPath pathNameTemp(iDriveName);
	pathNameTemp.Append(KDbFileName);
	RSqlDatabase::Delete(pathNameTemp);
	pathNameTemp = _L("D:");
	pathNameTemp.Append(KDbFileName);
	RSqlDatabase::Delete(pathNameTemp);
	pathNameTemp = _L("E:");
	pathNameTemp.Append(KDbFileName);	
	RSqlDatabase::Delete(pathNameTemp);

	// delete "StoreInit.tmp"
 	TPath pathName(iDriveName);
	pathName.Append(KStoreInitFileName);  	
	iFs.Delete(pathName); 
	CleanupStack::PopAndDestroy(fileMan);
	}

EXPORT_C void CMsvTestUtils::FindChildrenL(TMsvId aFolderToTraverse, const TFileName& aFilepath, TBool aReplace, TBool aOtherFiles)
	{
	SpecifyLogsDir(aFilepath);
	FindChildrenL(aFolderToTraverse, aReplace, aOtherFiles);
	}

EXPORT_C void CMsvTestUtils::FindChildrenL(TMsvId aFolderToTraverse, TBool aReplace, TBool aOtherFiles)
	{
	iFileCount=0;

	CDir* rfcFileList;
	// Loads the any test files into an EntryList
	iFs.GetDir(iRfc822Dir, KEntryAttNormal, ESortByName, rfcFileList);
		
	TMsvSelectionOrdering ordering;
	ordering.SetShowInvisibleEntries(ETrue);

	TMsvId entryId;
	if (iClientServer==EClientSide)
		{
		entryId=iMsvEntry->Entry().Id();
		iMsvEntry->SetEntryL(aFolderToTraverse);
		iMsvEntry->SetSortTypeL(ordering);
		}
	else
		{
		entryId=iServerEntry->Entry().Id();
		iServerEntry->SetEntry(aFolderToTraverse);
		iServerEntry->SetSort(ordering);
		}


	RFile file;
	TFileName filename(iLogsDir);
	filename.Append(KFileNameEntryStructure);

	// Print out Hierarchy - Save to file
		
	if (aReplace)
		{
		// replace both Entry_Structure.txt and Entry_RichTextBodies.txt
		TInt err1 = file.Replace(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
		if (err1==KErrNotFound) // file does not exist - create it
			err1=file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
		if (err1 == KErrNone)
			ListChildrenL(aReplace, aOtherFiles, file, *rfcFileList);
		}
	else
		{
		TInt err1 = file.Open(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
		if (err1==KErrNotFound)
			file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
		else if (err1 != KErrNone)
			User::Leave(err1);
		TInt position=1;
		User::LeaveIfError(file.Seek(ESeekEnd, position));
		ListChildrenL(aReplace, aOtherFiles, file, *rfcFileList);
		}

	file.Close();
	
	if (iClientServer==EClientSide)
		iMsvEntry->SetEntryL(entryId);
	else
		iServerEntry->SetEntry(entryId);
	delete rfcFileList;
	}

EXPORT_C void CMsvTestUtils::WriteBodyDataL(TMsvId aId, const TFileName& aFilepath, CMsvStore& fileStore, TBool aReplace)
	{
	CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
	CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
	CRichText* body = CRichText::NewL(paraLayer,charLayer);

	fileStore.RestoreBodyTextL(*body);

	HBufC* pBodyText = HBufC::NewLC(body->DocumentLength()+(body->DocumentLength()/70)+1);
	TPtr pBody = pBodyText->Des();
	body->Extract(pBody, 0);

	RFile file;
	TFileName filename(aFilepath);
	filename.Append(KFileNameBodies);
	
	TInt err = KErrNone;
	if (aReplace)
		err = file.Replace(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	else
		err = file.Open(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	if(err==KErrNotFound) // file does not exist - create it
		err=file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);

	TInt offset=0;
	iRTest(file.Seek(ESeekEnd, offset)==KErrNone);

	TBuf<100> buf;
	buf.Zero();
	buf.AppendFormat(_L("*** %d *************** RichText Data ***************\n"), aId);
	buf.AppendFormat(_L("Size >>> %d\n"), body->DocumentLength());	
	WriteToFileL(file, buf);
	
	RemoveRichtextFormating(pBody);
	WriteToFileL(file, pBody);
	
	buf.Zero();
	buf.AppendFormat(_L("\n********************* end of Body ***********************\n"));
	WriteToFileL(file, buf);
	
	CleanupStack::PopAndDestroy(); // pBodyText
	file.Close();
	delete paraLayer;
	delete charLayer;
	delete body;
	}

EXPORT_C void CMsvTestUtils::WriteFileDataL(TMsvId aId, const TFileName& aFileName, const TFileName& aLogFilepath, TBool aReplace)
	{
	TParse dirPath;
	RFile attach;

	dirPath.Set(aFileName, NULL, NULL);
	TInt err = KErrNone;
	if (attach.Open(iFs, dirPath.FullName(), KEntryAttNormal) != KErrNone)
		return; // failed to find attachment


	TFileName filename(aLogFilepath);
	filename.Append(KFileNameFiles);
	RFile file;
	if (aReplace)
		err = file.Replace(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	else
		err = file.Open(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	if(err==KErrNotFound) // file does not exist - create it
		err=file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
		
	TInt offset=0;
	__ASSERT_ALWAYS(file.Seek(ESeekEnd, offset)==KErrNone, Panic(KErrGeneral));

	TBuf<100> buf;
	buf.Zero();
	buf.AppendFormat(_L("\n*** %d *************** File Data ***************\n"), aId);
	
	buf.AppendFormat(_L("Filename >>> "));	
	WriteToFileL(file, buf);
	WriteToFileL(file, dirPath.NameAndExt());	
	WriteToFileL(file,_L("\n"));
	
	HBufC8* buffer8 = HBufC8::NewLC(1024);
	TPtr8 buf8 = buffer8->Des();
	do {
		attach.Read(buf8);
		WriteToFileL(file, buf8);
		} while (buf8.Length());
	
	buf.Zero();
	buf.AppendFormat(_L("\n********************* end of File ***********************\n"));
	WriteToFileL(file, buf);
	

	CleanupStack::PopAndDestroy(); // buffer8
	attach.Close();
	file.Close();
	}


void CMsvTestUtils::ListChildrenL(TBool aReplace, TBool aOtherFiles, RFile& file, CDir& rfcFileList)
	{
	TInt count = 0;
	TInt entryCount = 0;

	ListChildrenL(file, rfcFileList, count, entryCount, aReplace, aOtherFiles);
	}

EXPORT_C void CMsvTestUtils::SetFolderType(TMsvEntry& entry, TPtrC& type)
	{

	if(entry.iType == KUidMsvRootEntry)
		type.Set(_L("ROOT"));
	else if(entry.iType == KUidMsvServiceEntry)
		type.Set(_L("SERVICE"));
	else if(entry.iType == KUidMsvFolderEntry)
		type.Set(_L("FOLDER"));
	else if(entry.iType == KUidMsvMessageEntry)
		type.Set(_L("MESSAGE"));
	else if(entry.iType == KUidMsvEmailTextEntry)
		type.Set(_L("TEXT"));
	else if(entry.iType == KUidMsvEmailHtmlEntry)
		type.Set(_L("HTML"));
	else if(entry.iType == KUidMsvAttachmentEntry)
		type.Set(_L("ATTACHMENT"));
	else
		type.Set(_L("UNKNOWN"));
	}

EXPORT_C void CMsvTestUtils::ListChildrenL(RFile& aFile, CDir& rfcFileList, TInt& aCount, TInt& aEntryCount, TBool aReplace, TBool aOtherFiles)
	{
	// Check details of attachments on current context entry

	// Get list of children IDs
	CMsvEntrySelection* msvSelection = NULL;

	TMsvEntry entry;
	if (iClientServer==EClientSide)
		{
		entry = iMsvEntry->Entry(); 
		msvSelection = iMsvEntry->ChildrenL();
		}
	else
		{
		entry = iServerEntry->Entry(); 
		msvSelection = new (ELeave) CMsvEntrySelection;
		iServerEntry->GetChildren(*msvSelection);
		}
	CleanupStack::PushL(msvSelection);

	TInt attachCount = msvSelection->Count(); 	// For each child ..
	TPtrC type;
	SetFolderType(entry, type);

	TPtrC priority;
	TMsvPriority temp=entry.Priority();
	if(temp==EMsvHighPriority)
		priority.Set(_L("High  "));
	else if(temp==EMsvMediumPriority)
		priority.Set(_L("Medium"));
	else if(temp==EMsvLowPriority)
		priority.Set(_L("Low   "));
	else
		priority.Set(_L("None  "));

	TBuf<11> other=_L("-------  ");

	TBuf<6> streams=_L("------");

	if ((iClientServer==EClientSide && iMsvEntry->HasStoreL()) ||
		(iClientServer==EServerSide && iServerEntry->HasStoreL()))
		{
		CMsvStore* fileStore;
		fileStore = (iClientServer==EClientSide) ? iMsvEntry->ReadStoreL() : iServerEntry->ReadStoreL();
		
		if(aOtherFiles && fileStore->IsPresentL(KMsvEntryRichTextBody))
			{
			WriteBodyDataL(entry.Id(), iLogsDir, *fileStore, aReplace);
			streams.Replace(2, 1, _L("B"));
			}
		delete fileStore;
		}

	if(entry.Attachment())
		streams.Replace(3, 1, _L("A"));

	if (entry.New())
		other.Replace(1, 1, _L("N"));
	if (entry.Unread())
		other.Replace(2, 1, _L("U"));
	if (entry.Complete())
		other.Replace(3, 1, _L("C"));

	TBuf<600> outputLine;

	// Put details into output string buffer
	TMsvId current=entry.Id();
	if (iClientServer==EClientSide) 
		iMsvEntry->SetEntryL(current);
	else
		iServerEntry->SetEntry(current);
	
	for(TInt i=0; i<aCount; i++)
		outputLine.AppendFormat(_L("  "));
		
	outputLine.AppendFormat(TRefByValue<const TDesC>_L("%S, 00%x, Children=%d, Size=%d, Store=%S, P=%S, Other=%S, Det:%S\r\n"),
							&type, 
							entry.Id(),
							attachCount, 
							entry.iSize,
							&streams,
							&priority,
							&other,
							&entry.iDetails);
	
	HBufC8* pOutputLine8 = HBufC8::NewLC(outputLine.Length());
	pOutputLine8->Des().Copy(outputLine);
	aFile.Write(pOutputLine8->Des());
	CleanupStack::PopAndDestroy(); // pBuf16

	for(TInt j=0; j<attachCount; j++)
		{
		// Get Context of current message
		if (iClientServer==EClientSide) 
			iMsvEntry->SetEntryL((*msvSelection)[j]);
		else
			iServerEntry->SetEntry((*msvSelection)[j]);
			
		aEntryCount++;
		aCount++;
		ListChildrenL(aFile, rfcFileList, aCount, aEntryCount, aReplace);
		aCount--;
		}	

	CleanupStack::PopAndDestroy(); // msvSelection
	}


EXPORT_C void CMsvTestUtils::CreateAllTestDirectories()
	{
	CTestUtils::CreateAllTestDirectories();

	iFs.SetSessionPath(iDriveName);

	if (!iFs.MkDir(KMsvDefaultFolder2))
		{
		Printf(_L("Created mail directory\n"));
		}

	if (!iFs.MkDir(KMtmDir))
		{
		Printf(_L("Created mtm directory\n"));
		}
	}

EXPORT_C void CMsvTestUtils::ConstructL(TUint aFlags)
	{
	if (aFlags & ETuKeepLogFile)
		CTestUtils::ConstructKeepLogFileL();
	else		
		CTestUtils::ConstructL();

	FileSession().SetSessionPath(iDriveName);

	if (aFlags & ETuCleanMessageFolder)
		{
		Printf(_L("Clean Message Folder\n"));
		CleanMessageFolderL();
		}

	if (aFlags & ETuCreateTestDirectories)
		{
		Printf(_L("Create All Test Directories\n"));
		CreateAllTestDirectories();
		}

	if (aFlags & ETuDeleteMtmRegistry)
		{
		Printf(_L("Delete Mtm Registry\n"));
		_LIT(KSystemMTMRegister, "\\system\\mtm\\MTM Registry");
		TPath pathNameTemp(iDriveName) ;
		pathNameTemp.Append(KSystemMTMRegister);		
		FileSession().Delete(pathNameTemp);
		}

	CreateRegistryObjectAndControlL();

	if (aFlags & ETuCreateServerMtmReg)
		{
		Printf(_L("Create Server Mtm Regs\n"));
		CreateServerMtmRegsL();
		}

	if (aFlags & ETuGoClientSide)
		{
		Printf(_L("Go Client Side\n"));
		__ASSERT_ALWAYS(!(aFlags & ETuGoServerSide), Panic(KErrGeneral));
		GoClientSideL();
		}

	if (aFlags & ETuGoServerSide)
		{
		Printf(_L("Go Server Side\n"));
		__ASSERT_ALWAYS(!(aFlags & ETuGoClientSide), Panic(KErrGeneral));
		GoServerSideL();
		}

	if (aFlags & ETuInstallMtmGroup)
		{
		Printf(_L("Install Mtm Groups\n"));
		InstallMtmGroupsL();
		}

	if (aFlags & ETuDeleteService)
		{
		Printf(_L("Delete Existing Services\n"));
		__ASSERT_ALWAYS(iClientServer, Panic(KErrGeneral));
		DeleteServicesL();
		}
	else if (iClientServer)
		{
		Printf(_L("Find Existing Services\n"));
		FindExistingServicesL();
		}

	if (aFlags & ETuCreateService)
		{
		Printf(_L("Create Services\n"));
		__ASSERT_ALWAYS(iClientServer, Panic(KErrGeneral));
		CreateServicesL();
		}
	TChar driveChar=RFs::GetSystemDriveChar();
 	iDriveName.Append(driveChar);
 	iDriveName.Append(KDriveDelimiter);	

	SpecifyLogsDir(KLogsDir);
	FileSession().SetSessionPath(iDriveName);
	}

EXPORT_C void CMsvTestUtils::ServiceIdL(TUid aMtm, TMsvId& rFirstId, CMsvEntrySelection* rServiceIds)
	{
//Returns the Service IDs of MTM aMtm

	rFirstId = KMsvNullIndexEntryId;

	SetEntryL(KMsvRootIndexEntryId);

//	TMsvSelectionOrdering oldOrder = aEntry.SortType();
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	SetSortTypeL(order);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = ChildrenWithTypeLC(KUidMsvServiceEntry);

	TInt count = selection->Count();

	//Find an entry for MTM aMtm
	for (TInt curChild = 0; curChild < count && (rFirstId == KMsvNullIndexEntryId || rServiceIds); curChild++)
		{
		SetEntryL(selection->At(curChild));

		if (Entry().iMtm == aMtm)
			{
			TMsvId id = Entry().Id();

			if (rFirstId == KMsvNullIndexEntryId)
				rFirstId = id;

			if (rServiceIds)
				rServiceIds->AppendL(id);
			}
		}

	//Leave if no Service Entry found for MTM aMtm
	if (rFirstId == KMsvNullIndexEntryId)
		{
		CleanupStack::PopAndDestroy(); //selection
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(); //selection
	}

EXPORT_C void CMsvTestUtils::DeleteServiceL(TUid aMtm)
	{
	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);
	TMsvId firstId = 0;
	TRAPD(err, ServiceIdL(aMtm, firstId, sel));
	
	if (!err)
		{
		TInt count = sel->Count();

		while (count--)
			{
			TMsvId del = sel->At(count);
			TRAP(err, SetEntryL(del));

			if (!err)
				{
				TRAP(err, SetEntryL(Entry().Parent()));

				if (!err)
					{
					TRAP(err, DeleteEntryL(del));
					}
				}

			if (err)
				{
				Printf(_L("Error deleting service entry\n"));
				}
			}
		}

	CleanupStack::PopAndDestroy(); //sel
	}

EXPORT_C void CMsvTestUtils::SpecifyLogsDir(const TFileName& aFilePath)
	{
	iLogsDir.Copy(aFilePath);
	}

EXPORT_C void CMsvTestUtils::SpecifyRfc822Dir(const TFileName& aFilePath)
	{
	iRfc822Dir.Copy(aFilePath);
	}

//Returns TMsvEntry.iDate for aEntryId, and the time it is scheduled for on the task scheduler
EXPORT_C TInt CMsvTestUtils::ScheduleTime(TMsvId aEntryId, TTime& rEntryTime, TTime& rTaskSchedulerTime, TTaskInfo& rTaskInfo)
	{
	TRAPD(err, DoScheduleTimeL(aEntryId, rEntryTime, rTaskSchedulerTime, rTaskInfo));
	return err;
	}

void CMsvTestUtils::DoScheduleTimeL(TMsvId aEntryId, TTime& rEntryTime, TTime& rTaskSchedulerTime, TTaskInfo& rTaskInfo)
	{
	SetEntryL(aEntryId);
	rEntryTime = Entry().iDate;

	CMsvStore* store = ReadStoreL();
	CleanupStack::PushL(store);

	TMsvEntryScheduleData data;
	data.RestoreL(*store);

	CleanupStack::PopAndDestroy(); //store

	if (data.iTaskId == KErrNotFound)
		User::Leave(KErrNotFound);

	RScheduler scheduler;
	User::LeaveIfError(scheduler.Connect());
    // Make sure that scheduler connection
    // is closed if anything fails (and we leave) in this method.
	CleanupClosePushL(scheduler);

	TSchedulerItemRef ref;
	TInt size = 0;

	User::LeaveIfError(scheduler.GetTaskDataSize(data.iTaskId, size));

	HBufC* buf = HBufC::NewLC(size);
	TPtr ptr = buf->Des();

	User::LeaveIfError(scheduler.GetTaskInfoL(data.iTaskId, rTaskInfo, ptr, ref, rTaskSchedulerTime));

	CleanupStack::PopAndDestroy(2,&scheduler); //buf,scheduler.
	}

EXPORT_C TInt CMsvTestUtils::AppendScheduleTimeL(TMsvId aEntryId, TDes& rOutput)
	{
	TTime entryTime, schTime;
	TTaskInfo info;
	TInt err = ScheduleTime(aEntryId, entryTime, schTime, info);

	if (!err)
		{
		TBuf<128> dateString;

		entryTime.FormatL(dateString, _L("\tEntry: %D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
		rOutput.Append(dateString);

		schTime.FormatL(dateString, _L("\n\tSched: %D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
		rOutput.Append(dateString);

		rOutput.AppendFormat(_L(" (taskId %d)\n"), info.iTaskId);
		}
	else
		{
		rOutput.AppendFormat(_L("\tError %d: Cannot read schedule time for msg %d\n"), err, aEntryId);
		}

	return err;
	}

EXPORT_C void CMsvTestUtils::NavigateMessageStoreL(TMsvId aParent)
	{
	//Allows the user to navigate through the message store

	if (iClientServer != EClientSide)
		return;

	TInt count = 0;
	TInt start = 0;

	while (count < 3)
		{
		TInt err = DoNavigateMessageStoreL(aParent, start);

		if (err == KErrDied)
			break;
		else if (err)
			count++;
		else
			count = 0;
		}
	}

TInt CMsvTestUtils::DoNavigateMessageStoreL(TMsvId& aParent, TInt& aStart)
	{
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	CMsvEntry* parent = CMsvEntry::NewL(*iMsvSession, aParent, order);
	CleanupStack::PushL(parent);

	iRTest.Printf(KMsvTestUtilsNewLine);
	TInt more = 0;
	const TInt KMax = 10;
	ShowChildrenL(*parent, aStart, more, KMax);
	iRTest.Printf(KMsvTestUtilsNewLine);

	iRTest.Printf(_L("Select a message or '.' (for parent) or 'N'ext or 'P'revious or e'X'it\n"));

	const TInt count = Min(KMax, parent->Count() - aStart);
	const TChar startChar = '0';
	const TChar endChar = startChar + count - 1;
	const TChar key = iRTest.Getch();

	TInt ret = KErrNotFound;

	if (key >= '0' && key <= endChar)
		{
		iRTest.Printf(KMsvTestUtilsNewLine);

		TMsvEntry entry((*parent)[key - startChar + aStart]);

		if (entry.Owner())
			{
			aStart = 0;
			aParent = entry.Id();
			}
		else
			{
			DisplayChildDetailsL(entry);
			}

		ret = KErrNone;
		}
	else if (key == '.')
		{
		aStart = 0;
		if (aParent != KMsvRootIndexEntryId)
			aParent = parent->Entry().Parent();

		ret = KErrNone;
		}
	else if (key == 'n' || key == 'N')
		{
		if (more > 0)
			{
			aStart += KMax;
			}

		ret = KErrNone;
		}
	else if (key == 'p' || key == 'P')
		{
		if (aStart > 0)
			{
			aStart -= KMax;
			}

		ret = KErrNone;
		}
	else if (key == 'x' || key == 'X')
		{
		ret = KErrDied;
		}

	CleanupStack::PopAndDestroy(parent);
	return ret;
	}

void CMsvTestUtils::ShowChildrenL(const CMsvEntry& aEntry, TInt aStart, TInt& aMore, TInt aMaxCount)
	{
	iRTest.Printf(_L("Parent: "));

	DisplayChildL(aEntry.Entry());

	iRTest.Printf(KMsvTestUtilsNewLine);
	iRTest.Printf(KMsvTestUtilsNewLine);

	if (!aEntry.Entry().Owner())
		return;

	TInt count = aEntry.Count() - aStart;

	if (aMaxCount > 0)
		count = Min(count, aMaxCount);

	for (TInt i = aStart; i < count + aStart; i++)
		{
		TBuf<16> prefix;
		prefix.Zero();
		prefix.AppendNum(i - aStart);
		prefix.Append(_L(". ("));
		prefix.AppendFormat(_L("%2d"), i + 1),
		prefix.Append(_L("/"));
		prefix.AppendFormat(_L("%2d"), aEntry.Count()),
		prefix.Append(_L(")"));
		iRTest.Printf(prefix);
		DisplayChildL(aEntry[i]);
		iRTest.Printf(KMsvTestUtilsNewLine);
		}

	aMore = aEntry.Count() - aStart - aMaxCount;

	if (aMore > 0)
		iRTest.Printf(_L("...%d more...\n"), aMore);
	}

EXPORT_C void CMsvTestUtils::DisplayChildL(const TMsvEntry& entry)
	{
	_LIT(KSpace, " ");
	_LIT(KQuote, "\"");
	HBufC* buf = HBufC::NewLC(entry.iDetails.Length() + entry.iDescription.Length() + 100);
	TPtr temp(buf->Des());
	temp.AppendFormat(_L("%7.7d"), entry.Id());
	temp.Append(KSpace);

	switch (entry.iType.iUid)
		{
		case KUidMsvServiceEntryValue:
			temp.Append(_L("Serv"));
			break;
		case KUidMsvRootEntryValue:
			temp.Append(_L("Root"));
			break;
		case KUidMsvFolderEntryValue:
			temp.Append(_L("Fold"));
			break;
		case KUidMsvMessageEntryValue:
			temp.Append(_L("Mesg"));
			break;
		case KUidMsvAttachmentEntryValue:
			temp.Append(_L("Atch"));
			break;
		default:
			temp.Append(_L("Othr"));
			break;
		}

	temp.Append(KSpace);
	temp.Append(KQuote);
	temp.Append(entry.iDetails);
	temp.Append(KQuote);
	temp.Append(_L(" Mtm: "));
	temp.AppendNum((TInt) entry.iMtm.iUid);
	temp.Append(_L(" Des: "));
	temp.Append(entry.iDescription.Left(10));

	iRTest.Printf(temp);
	CleanupStack::PopAndDestroy(buf);
	}
