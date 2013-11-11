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
// T_msgs_FindEntry.cpp
// 
//


#include <hal.h>
#include <bautils.h>
#include <e32base.h>
#include <barsc.h>
#include <msgs.rsg>
#include "Testsetup.h"
#include <msvfind.h>
#include "msvindexadapter.h"

TInt entryCount=1000; 


_LIT(KTestName, "FindEntry Test");
RTest test(KTestName);

TMsvId testId;
TTime starttime;
TTime endtime;
TTimeIntervalMicroSeconds elapsedtime;

TInt totalMem = 0;
TInt freeMem = 0;
TInt freeMemBefore = 0;

TMsvId startId;
TMsvId endId;
TMsvEntry* entryPtr;
TBuf<256> buffer;	
_LIT(KTest1, " Finding Entry");



_LIT(KMsvTestFile, "c:\\IndexTestFile");


LOCAL_C void InitL()
	{
	theUtils->WriteComment(_L("InitL"));
	}
	
LOCAL_C void CreateDefaultIndex(CMsvServerIndex& aIndex)
	{
	// find the resource file
	TFileName fileName = KServerIndexResourceFileName();
	TParse parse;
	TFindFile findfile(theUtils->FileSession());
	CDir* dirs=NULL;
	User::LeaveIfError(findfile.FindWildByDir(fileName, KServerIndexResourceFilePath, dirs));
	fileName=(*dirs)[0].iName;
	delete dirs;
	parse.Set(fileName, &findfile.File(), NULL);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(theUtils->FileSession(), fileName);

	// load the initial index structure from resource file
	RResourceFile rf;
	rf.OpenL(theUtils->FileSession(),fileName);
	CleanupClosePushL(rf);
	TResourceReader reader;
	HBufC8* buf=rf.AllocReadLC(R_SERVER_INDEX_STARTUP);
	reader.SetBuffer(buf);

	const TInt numberOfEntries=reader.ReadInt16();
	TTestMsvEntry newEntry;
	for (TInt ii=0; ii<numberOfEntries; ii++)
		{
			Mem::FillZ(&newEntry,sizeof(TTestMsvEntry));
			// values from resource file
			newEntry.SetId(reader.ReadInt32());
			newEntry.SetParent(reader.ReadInt32());
			newEntry.iServiceId = reader.ReadInt32();
			newEntry.iType.iUid = reader.ReadInt32();
			newEntry.iMtm.iUid = reader.ReadInt32();
			
			// optional information
			newEntry.SetData(reader.ReadInt32());
			newEntry.iDescription.Set(reader.ReadTPtrC());
			newEntry.iDetails.Set(reader.ReadTPtrC());
			// plus other ones
			newEntry.iDate.HomeTime();
			newEntry.iSize=0;
			// populate it
			User::LeaveIfError(aIndex.AddEntry(newEntry, KMsvServerId, EFalse));
		}

	CleanupStack::PopAndDestroy(2); // reader(buf), rf

	}

LOCAL_C void PopulateL(TInt aCount,CMsvServerIndex& aIndex, TMsvId& startId,TMsvId& endId)
	{
	if(aCount<1)
		User::Leave(KErrArgument);
	TTestMsvEntry entry;
	Mem::FillZ(&entry,sizeof(TTestMsvEntry));
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(aIndex.AddEntry(entry, KMsvServerId, ETrue));
	startId=entry.Id();
	for (int i=0;i<aCount-1;i++)
		{
		REPORT_IF_ERROR(aIndex.AddEntry(entry, KMsvServerId, ETrue));
		}		
	endId=entry.Id();
	}
	

LOCAL_C void doTestsL()
	{

	InitL();
	theUtils->FileSession().Delete(KMsvTestFile);
	test.Start(KTest1);
	theUtils->WriteComment(_L("Start"));
	//
	starttime.UniversalTime();
	//
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	// Create and Add entries
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	//
	endtime.UniversalTime();
	elapsedtime = endtime.MicroSecondsFrom(starttime);	
	//
	_LIT(KComment, "Create Index took %ld (mikrosec) ");
	buffer.Format(KComment, elapsedtime.Int64());
	theUtils->WriteComment(buffer);
	
	_LIT(KMemComment, "Memory total:%d free:%d used:%d diff:%d");
    HAL::Get(HALData::EMemoryRAM, totalMem);
    HAL::Get(HALData::EMemoryRAMFree, freeMem);
    
	buffer.Format(KMemComment,totalMem, freeMem, totalMem-freeMem, freeMem-freeMemBefore);
	theUtils->WriteComment(buffer);    
	freeMemBefore=freeMem;
		
	starttime.UniversalTime();
	PopulateL(entryCount,*index,startId,endId);
	endtime.UniversalTime();
	elapsedtime = endtime.MicroSecondsFrom(starttime);	
	//
	_LIT(KComment2, "Populate  %d took %ld (mikrosec)");
	buffer.Format(KComment2, entryCount, elapsedtime.Int64());
	theUtils->WriteComment(buffer);
	test.Printf(KComment2,entryCount, elapsedtime.Int64());
	
	HAL::Get(HALData::EMemoryRAM, totalMem);
    HAL::Get(HALData::EMemoryRAMFree, freeMem);
	buffer.Format(KMemComment,totalMem, freeMem, totalMem-freeMem, freeMem-freeMemBefore);
	theUtils->WriteComment(buffer);    
	freeMemBefore=freeMem;
	
	_LIT(KComment3, "startId %x %x");
	buffer.Format(KComment3, startId,endId);
	theUtils->WriteComment(buffer);
	
	TMsvId currentId=startId;
	TUint32 startTime,endTime;
	while(currentId<=endId)
	{	
		REPORT_IF_ERROR(index->GetEntry(startId, entryPtr));
		
		startTime=User::FastCounter();
		
		REPORT_IF_ERROR(index->GetEntry(currentId, entryPtr));
		
		endTime=User::FastCounter();
		
		_LIT(KComment4, "GetEntry Id %x took %d");
		buffer.Format(KComment4, currentId,endTime - startTime);
		theUtils->WriteComment(buffer);	
		HAL::Get(HALData::EMemoryRAM, totalMem);
    	HAL::Get(HALData::EMemoryRAMFree, freeMem);
    	
		buffer.Format(KMemComment,totalMem, freeMem, totalMem-freeMem, freeMem-freeMemBefore);
		theUtils->WriteComment(buffer);   
		freeMemBefore=freeMem; 
		++currentId;
	}
	
	CMsvServerIndex* index2 = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index2);

	currentId=endId;
	while(currentId>=startId)
	{	
		REPORT_IF_ERROR(index->GetEntry(startId, entryPtr));
		
		startTime=User::FastCounter();
		
		REPORT_IF_ERROR(index2->LockEntryAndStore(currentId));
		REPORT_IF_ERROR(index2->DeleteEntry(currentId));
		
		endTime=User::FastCounter();
		
		_LIT(KComment5, "DeleteEntry Id %x took %d");
		buffer.Format(KComment5, currentId,endTime - startTime);
		theUtils->WriteComment(buffer);
			
		HAL::Get(HALData::EMemoryRAM, totalMem);
	    HAL::Get(HALData::EMemoryRAMFree, freeMem);
    
		buffer.Format(KMemComment,totalMem, freeMem, totalMem-freeMem, freeMem-freeMemBefore);
		theUtils->WriteComment(buffer);    
		freeMemBefore=freeMem;
		--currentId;
	}

	
	CleanupStack::PopAndDestroy(1); // index2
	
	CleanupStack::PopAndDestroy(2); // index, teststore
	
	
	CMsgsTestUtils::Complete();
	
	
	theUtils->FileSession().Delete(KMsvTestFile);
	test.Printf(_L("End"));		
	}

