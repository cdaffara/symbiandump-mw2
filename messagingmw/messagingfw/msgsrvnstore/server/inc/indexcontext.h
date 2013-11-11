// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__INDEXCONTEXT_H__)
#define __INDEXCONTEXT_H__

#include <e32base.h>
#include <f32file.h>
#include <msvipc.h>
#include <barsread.h>
#include "msventryfreepool.h"
#include "msvindexadapter.h"

class CMsvServer;
class CMsvIndexAdapter;

//**********************************
// TMsvConfig
//**********************************

class TMsvConfig
/**
@internalComponent
@released
*/
	{
public:
	TDriveUnit iDrive;
	TUint iUniqueID;
	TBool iDebug;
	
// these are used to compare against the above so we know if we have to write the data.
	TDriveUnit iDriveAsLoaded;
	TUint iUniqueIDAsLoaded;
	TBool iDebugAsLoaded;
	};

//**********************************
// MMsvContextObserver
//**********************************

class MMsvContextObserver
/**
@internalComponent
@released
*/
	{
public:
	virtual void ContextComplete(TInt aError, TBool aRunMailInit)=0;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	virtual void ChangeDriveComplete(TInt aError, TBool aRunMailInit, TDriveNumber aNewDrive)=0;
#endif
	};

//**********************************
// CMsvIndexContext
//**********************************

class CMsvIndexContext : public CActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvIndexContext* NewLC(CMsvServer& aServer, MMsvContextObserver& aObserver);
	static CMsvIndexContext* NewL(CMsvServer& aServer, MMsvContextObserver& aObserver);
	~CMsvIndexContext();

	inline const CMsvEntrySelection& Update() const;
	inline CMsvEntrySelection& Update();
	//
	inline const CMsvEntrySelection& Remove() const;
	inline CMsvEntrySelection& Remove();
	//
	inline TBool Initialized() const;
	inline void Initialized(TBool aInitialized);
		
	//
	inline const TDesC& MessageFolder() const;
	inline TMsvIndexLoadProgress::TIndexState State() const;
	inline const TMsvConfig& Config() const;

	const TMsvIndexLoadProgress& Progress();
	//
	const TDesC &IndexFileName() const;
	CPermanentFileStore &IndexStore();
	//
	void LocalizeStandardFoldersL();
	TBool GetAndClearIndexCorruptFlagL();

// Code changes for PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	void CreateIndexL();	
	void UpdateDriveStatusL(TUint aDriveIndex, TDriveState& aStoreStatus);
	TInt ChangeDrive(TUint aNewDriveIndex, TRequestStatus& aStatus);
	TInt ChangeDrive(TUint aNewDriveIndex, TBool aIsRemovePartial = ETrue, TBool aAsync = EFalse);
	void GetInPreparationIds(TUint aDriveId);	
	inline void MessageFolder(TUint aDriveId, TDes& aName) const;
#else
	TInt LoadStoreConfig(TBool aLoad);
	void CreateIndexL(TInt aDrive, TRequestStatus& aStatus);
	void CreateIndexL(TInt aDrive, TBool aSync);	
#endif			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	

	inline CMsvIndexAdapter*& IndexAdapter();
	inline CMsvIndexAdapter* IndexAdapter() const;
private:

	void ConstructL();
	CMsvIndexContext(CMsvServer& aServer, MMsvContextObserver& aObserver);
	
// Changes for PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void DoCreateIndexL();
	void IndexLoadingCompleted();
	void ChangeDriveCompleted(TInt aError);	
	void DoCreateServiceDirsL(TUint aDriveId);
	void CreateStoreDeleteFile(TDriveNumber aDriveNum);
	void CreateStandardEntriesFromResourceFileL(TUint aDriveId);
	void DoChangeDriveL(TUint aNewDriveIndex, TBool aIsRemovePartial, TDriveNumber& aNewDriveNumber);
#else
	void DoStoreConfigL();
	void DoCreateServiceDirsL();
	void CreateStoreDeleteFile();
	void IndexFailedToLoad(TInt aError);
	void IndexLoadingCompleted(TInt aError);
#endif

	void ResetAndCreateNewMailStoreL(TBool aDelete = ETrue);
	//
	void IndexLoaded(TMsvServerChangeNotificationType aNotification);
	//
	void DoCancel();
	void RunL();

private:
	CMsvEntrySelection* iUpdateEntryStreams;
	CMsvEntrySelection* iRemoveEntries;
	//
	CMsvServer& iServer;
	MMsvContextObserver& iObserver;
	//
	TFileName iMessageFolder;	
	TMsvIndexLoadProgress iProgress;
	TMsvConfig iConfig;
	TRequestStatus* iObserverStatus;
	TBool iSyncLoad;
	TBool iRunMailInit;
	TBool iInitialized;
	HBufC8* iBuf;
	CMsvIndexAdapter* iIndexAdapter;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestIndexContext;
	friend class CTestSearchSortDBadapter;
#endif
	};

inline const TDesC& CMsvIndexContext::MessageFolder() const
	{
	return iMessageFolder;
	}

inline const CMsvEntrySelection& CMsvIndexContext::Update() const
	{
	return *iUpdateEntryStreams;
	}

inline CMsvEntrySelection& CMsvIndexContext::Update()
	{
	return *iUpdateEntryStreams;
	}

inline const CMsvEntrySelection& CMsvIndexContext::Remove() const
	{
	return *iRemoveEntries;
	}

inline CMsvEntrySelection& CMsvIndexContext::Remove()
	{
	return *iRemoveEntries;
	}

inline TMsvIndexLoadProgress::TIndexState CMsvIndexContext::State() const
	{
	return iProgress.iState;
	}

inline const TMsvConfig& CMsvIndexContext::Config() const
	{
	return iConfig;
	}

inline TBool CMsvIndexContext::Initialized() const
	{
	return iInitialized;
	}

inline void CMsvIndexContext::Initialized(TBool aInitialized)
	{
	iInitialized=aInitialized;
	}

inline CMsvIndexAdapter*& CMsvIndexContext::IndexAdapter()
	{
	CMsvEntryFreePool::Instance()->FlushExcessMemory();
	return iIndexAdapter;
	}

inline CMsvIndexAdapter* CMsvIndexContext::IndexAdapter() const
	{
	CMsvEntryFreePool::Instance()->FlushExcessMemory();
	return iIndexAdapter;
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
inline void CMsvIndexContext::MessageFolder(TUint aDriveId, TDes& aName) const
	{
	TParse parse;
	TDriveNumber driveNum;
	CMsvPreferredDriveList::GetDriveList()->GetDriveNumber(aDriveId, driveNum);
	TPtrC drive(TDriveUnit(driveNum).Name());
	parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);
	aName = parse.DriveAndPath();
	}
#endif			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	

#endif
