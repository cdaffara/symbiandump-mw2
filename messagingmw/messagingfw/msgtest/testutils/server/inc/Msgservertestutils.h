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

#ifndef __MSGSERVERTESTUTILS_H__
#define __MSGSERVERTESTUTILS_H__

#include <e32base.h>
#include <s32file.h>
#include <f32file.h>
#include <e32test.h>
#include <e32uid.h>
#include "msvtestutilsbase.h"
#include <msvapi.h>
#include <msvstd.h>
#include "MSVSERV.H"
#include <msvruids.h>


const TUid KUidMsvIndexFile = {0x10003C6B};
const TInt KMsvIndexStoreVersionNumber = 1;

class CMsgServerTestUtils : public CTestUtils
	{
public:
	IMPORT_C static CMsgServerTestUtils* NewL(RTest& aRTest);
	IMPORT_C static CMsgServerTestUtils* NewLC(RTest& aRTest);

	IMPORT_C ~CMsgServerTestUtils();

	IMPORT_C void SetupL();
	IMPORT_C void Close(TInt aRet);

	IMPORT_C void SetConfigL(TMsvConfig& aConfig);
	IMPORT_C void SetDebugL(TBool aDebug);
	IMPORT_C void GetConfigL(TMsvConfig& aConfig);
	IMPORT_C void CleanMessageFolderL(const TDesC& aDes = KNullDesC);
	IMPORT_C void SetDriveL(TInt aDrive, TBool aStoreId = EFalse);
	IMPORT_C CMsvServer* CreateServerL(const TDesC& aIndexFile);
	IMPORT_C void FileExists(const TDesC& aDes);
	IMPORT_C void FileMissing(const TDesC& aDes);
	IMPORT_C void CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName);
	IMPORT_C void InstallMtmL(const TDesC& aDataFile, const TDesC& aMtmName, TUid aId, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName);
	IMPORT_C TBool WaitForKeyL(TTimeIntervalMicroSeconds32 aDelay, TKeyCode* aKeyCode = NULL);
	IMPORT_C void WaitForServerClose();
	IMPORT_C void RemoveMailFolderL(TInt aDrive);
	IMPORT_C void CorruptIndexFileL(TInt aMode, const TDesC& aDes = KNullDesC);
	IMPORT_C void RemoveIndexFile(TInt aDrive);
	IMPORT_C TInt IndexSizeL(TInt aDrive = EDriveC, const TDesC& aIndexFile = KMsvDefaultIndexFile2);
	
	IMPORT_C void Start(const TDesC& aDes);
	IMPORT_C void Next(const TDesC& aDes);
	IMPORT_C void Complete();
	IMPORT_C void End();

	IMPORT_C TInt ReadInteger(const TDesC& aDes,TInt &aResult);
	IMPORT_C void ChooseDriveL(TDriveUnit &aResult);

	IMPORT_C TMsvId CreateServiceL(const TUid aMtm, TBool aReadOnly, TBool aVisible);
private:
	CMsgServerTestUtils(RTest& aRTest);
	void ConstructL();
	void Panic(TInt aPanic);

private:
	TDriveUnit iRemovableDrive;
	TInt	   iTestNum;
public:
	};


class CTestScheduler : public CActiveScheduler
	{
public:
	IMPORT_C CTestScheduler();
private:
	
	IMPORT_C void Error(TInt aError) const;
public:
	TInt iSchedulerError;
	TBool iSchedulerActive;
	TBool iPanicSchedulerOnError;
	};


class TTestMsvEntry : public TMsvEntry
	{
public:
	void SetOwner(TBool aFlag) {TMsvEntry::SetOwner(aFlag);};
	void SetId(TMsvId aId) {TMsvEntry::SetId(aId);};
	void SetParent(TMsvId aId) {TMsvEntry::SetParent(aId);};
	void SetDeleted(TBool aFlag) {TMsvEntry::SetDeleted(aFlag);};
	TInt PcSyncCount() {return TMsvEntry::PcSyncCount();};
	TInt Reserved() {TInt* ptr = (TInt*)&iServiceId; return *--ptr;};
	void SetReserved(TInt aValue) {TInt* ptr = (TInt*)&iServiceId; *--ptr=aValue;};
	void SetData(TInt aData) {*(((TInt*)this)+2) = aData;};
	};


#endif
