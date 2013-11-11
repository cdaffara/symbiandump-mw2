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

#ifdef _DEBUG
#undef _NO_SERVER_LOGGING_
#endif

#if !defined(__MSVSERV_H__)
#define __MSVSERV_H__

#include <e32base.h>
#include <flogger.h>
#include <msvstore.h>
#include <msvipc.h>
#include <msvids.h>
#include "MSVROPS.H"
#include "indexcontext.h"
#include "MSVOP.H"
#include <mtsr.h>
#include "CMsvChangeBuffer.h"
#include <tnonoperationmtmdata.h>
#include "TMsvServerStoreManager.h"
#include "msvindexadapter.h"
#include "msventryfreepool.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "msvinifile.h"
	#include "msvpreferreddrivelist.h"
	#include "cmsvdiskchangenotifier.h"
#else
	#include "diskchange.h"
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	#include "msvmessagedbadapter.h"
	#include "cmsvconverterwaiter.h"
#endif

	
#if (defined  SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	_LIT(KMsvDbFile, "\\messaging.db");	 
#else
	_LIT(KMsvDbFile, "[1000484B]messaging.db");
#endif	  // #if (defined  SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

// Remove these to remove server logging
//#define _NO_SERVER_LOGGING_
#define _NO_SERVER_LOGGING_SERIAL_

_LIT(KServerINIFile,"\\private\\1000484b\\msgs.ini");


// Code added for PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	const TUint KMsvInvalidDriveId = 999;
#endif

// forward declarations
class CMsvServer;
class CMsvServerSession;
class CBaseServerMtm;
class CMsvBackupHandler;
class CMsvMailinitWaiter;

class CMSvSearchSortCacheManager;
class CMsvSearchSortDeltaCache;
class CMsvIndexAdapter;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	class CMsvDiskChangeNotifier;
	class CMsvPreferredDriveList;
#endif

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	class CMsvConverterWaiter;
#endif
// Client panics
/**
@internalComponent
@released
*/
enum TMsvClientPanic
	{
	EMsvBadRequest=1,
	EMsvDuplicatedChangeRequest=2,
	EMsvBadDescriptorWrite=3,
	EMsvBadDescriptorRead=4,
	EMsvIncorrectEntry=5,
	EMsvReleasingUnknownEntry=6,
	EMsvLockedStoresOnExit=7,
	EMsvOutstandingOperationsOnExit=8,
	EMsvReleasingUnknownStore=9,
	EMsvNoEntriesInDeleteSelection=10,
	EMsvRemovingNonLocalEntry=11,
	EMsvProgressBufferExceeds256=12,
	EMsvNoEntriesInMoveSelection=13,
	EMsvNoEntriesInCopySelection=14,
	EMsvNoEntriesInChangeAttributesSelection=15,
	EMsvNoEntriesInChangeSelection=16
	};

const TInt KMsvMessageDriveStreamVersionNumber = 3;
const TUid KUidMsvMessageDriveStream = {0x1000163E};

//**********************************
// CMsvMtmOperationQueue
//**********************************
//
//
//


class CMsvMtmOperationQueue: public CArrayPtrFlat<CMsvMtmOperation>
/**
@internalComponent
@released
*/
	{
public:
	CMsvMtmOperationQueue(TUid aMtmUid, TMsvId aServiceId);
	~CMsvMtmOperationQueue();
	//
	inline TUid MtmUid() const;
	inline TMsvId ServiceId() const;
public:
	CBaseServerMtm* iMtm;
private:
	TUid	iMtmUid;
	TMsvId  iServiceId;
	CArrayFixFlat<TInt> iSessionIdArray;
	//
friend class CMsvServer;
	};



//**********************************
// CMsvTimer
//**********************************
//
//
//

class CMsvTimer : public CTimer
/**
@internalComponent
@released
*/
	{
public:
	static CMsvTimer* NewL(CMsvServer& aServer, TBool aCloseServer);
private:
	CMsvTimer(CMsvServer& aServer, TBool aCloseServer);
	void RunL();
private:
	CMsvServer& iServer;
	TBool iCloseServer;
	};

//**********************************
// CMsvServer
//**********************************
//
// Main message server object
//

// This is used for testing
/**
@internalComponent
@released
*/
enum TMsvFailure
	{
	EHeapFailure,
	EDiskFailure
	};
	
class TCapabilitySet;

// Commit (and notify) interval for bulk transactions
// Kept as a #define so that it will always match the type of
// the value to which it is compared.
#define KBulkCommitInterval 16
class CMsvServer : public CPolicyServer, public MMsvOperationObserver, public MMsvContextObserver, public MMsvDiskChangeObserver
/**
@internalComponent
@released
*/
	{
public:
	enum {EPriority=950};

public:
	IMPORT_C TInt ChangeDrive(TInt aDrive, TRequestStatus* aStatus);
	IMPORT_C TInt CopyStore(const TDriveUnit& aDrive, TRequestStatus* aStatus);
	IMPORT_C TInt DeleteStore(const TDriveUnit& aDrive, TRequestStatus* aStatus);   
	~CMsvServer();
	//
	virtual CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
	//
	void NotifyChanged(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1=0, TInt aParameter2=0);
	void NotifyChanged(TMsvServerChangeNotificationType aChangeType, TMsvId aId=KMsvNullIndexEntryId, TInt aParameter1=0, TInt aParameter2=0);
	void NotifyChanged(TMsvServerChangeNotificationType aChangeType, TUid aMtmTypeUid);
	void QueueNotifyChanged(TMsvServerChangeNotificationType aChangeType);

	IMPORT_C static CMsvServer* NewL();
	static CMsvServer* NewL(TBool aDebug);
	inline const CMsvIndexAdapter& IndexAdapter() const;
	inline CMsvIndexAdapter& IndexAdapter();

// Code added for 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void UpdateRepositoryL();
	IMPORT_C void CreateIndexL();
	void PrepareDriveForDeletionL(TDriveNumber aDrive);
	void RemoveDriveFromListL(TDriveNumber aDriveNumber);
	void AddDriveToListL(TDriveNumber aDriveNumber, TUint& aPriority, CMsvServerSession* aCurrentSession = NULL);
	void UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aNewPriority);  
	void ChangeDriveComplete(TInt aError, TBool aRunMailInit, TDriveNumber aNewDrive);
	void UpdateDriveStatusL(const TDriveUnit& aDrive, TDriveState aStatus);
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		static void ResetRepositoryL();
	#endif
#else
	static void CurrentConfigL(RFs& aFs, TMsvConfig& aConfig);
	IMPORT_C void CreateIndexL(TBool aSync = EFalse);
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		
	inline RFs& FileSession();
	inline TInt SessionId();
	inline const CServerMtmDllRegistry& Registry() const;
	//
	// operation related
	void StartOperationL(CMsvMtmOperation& aOperation, TInt aSessionId, const RMessage2& aMessage, TBool aHasCapability);
	// from MMsvOperationObserver
	void OperationCompleted(TMsvId aServiceId, TMsvOp aOpId);
	void OperationCancelled(const CMsvMtmOperation& aOperation);
	//
	void ClosingSession(TInt aSessionId);
	void CheckAndClose();
	//
	TInt FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray);  // Fill array with Dlls whose second uid is aMtmDllTypeUid
	TInt InstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid);
	TInt DeInstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid);
	CMtmGroupData* GetMtmGroupDataL(TUid aMtmTypeUid) const;
	void GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCaps) const;
	//
	TInt GetAndMakeFileDirectory(TMsvId aId, TDes& aDirectory);
	TInt GetFileDirectoryListing(TMsvId aId, TDes& aDirectory, CDir*& aDir);
	TInt AddEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aBulk=EFalse);
	TInt ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate, TBool aBulk=EFalse);
	void RemoveEntry(TMsvId aId);
	void CloseServer(const RMessage2& aMessage);
	void AttemptDelayedUpdating();
	void StopServiceL(const RMessage2& aMessage);
	void ServiceActive(const RMessage2& aMessage);
	const TDesC8& ServiceProgressL(TMsvId aServcieId);
	inline TInt CheckEntries(const CMsvEntrySelection& aEntries);
	TInt CheckEntries(const CMsvEntrySelection& aEntries, TInt aStartIndex, TInt aEndIndex);
	//
	TInt GetEntryName(TMsvId aId, TDes& aFileName, TBool aFolderRequired);
	TInt AddEntry(CMsvIndexContext* const& aContext, TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aBulk=EFalse);
	//
	void SetFailure(TMsvFailure aType, TInt aArg1, TInt aArg2, TInt);
	TInt OutstandingOperations();
	//
	inline CMsvTimer& Delay() const;
	inline TBool HasContext() const;
	inline CMsvIndexContext& Context() const;
	inline CMsvIndexContext* NewContext() const;
	inline void DeleteNewContext();
	inline TMsvServerChangeNotificationType StartupState() const;


	void SetStartupState(TMsvServerChangeNotificationType aState);
	const TDriveUnit &Drive() const;

	void MailinitFinished(TInt aError, TDriveNumber aDriveNum);

	MRegisteredMtmDllObserver& MtmObserver() const;
	//  
#ifndef _NO_SERVER_LOGGING_
	void Log(TRefByValue<const TDesC> aFmt, ...);
#endif
	//Notifies observers of changes to a group of entries
	//(e.g. during bulk synchronisation of email headers)
	void NotifyChangedBulkL();
	
	// completes any outstanding bulk transactions
	void CompleteBulkTransaction(void);
	//
	// This function is for the CMsvServerSession class to access the public 
	// methods ConnectAndShareFileSessionL() and CloseFileSession()
	TMsvServerStoreManager& ServerStoreManager();

	void PoliceReadEntryL(const RMessage2& aMsg, TSecureId aOwnerId, const char* aContextText);
	void PoliceReadEntryL(const RMessage2& aMsg, TMsvId aId, const char* aContextText);
	void PoliceCreateEntryL(const RMessage2& aMsg, TMsvEntry aEntry, TBool& aIsLocal, const char* aContextText);
	void PoliceModifyEntryL(const RMessage2& aMsg, TMsvEntry aEntry, TBool& aIsLocal, const char* aContextText);
	void PoliceModifyEntryL(const RMessage2& aMsg, TMsvId aId, TBool& aIsLocal, const char* aContextText);
	void PoliceModifyEntryL(const RMessage2& aMsg, TMsvId aId, const char* aContextText);
	void PoliceMoveEntriesL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvId aSource, TBool& aTargetIsLocal, TBool& aSourceIsLocal, const char* aContextText);
	void PoliceCopyEntriesL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, TMsvId aTarget, TBool& aTargetIsLocal, const char* aContextText);
	void PoliceMtmTransferCommandL(const RMessage2& aMsg, TUid aMtm, const char* aContextText);
	void PoliceStopServiceL(const RMessage2& aMsg, TUid aMtm, const char* aContextText);

	void PoliceCopyMoveUnderOutboxL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, const char* aContextText);
	void PoliceCreateModifyEntryL(const RMessage2& aMsg, CMsvIndexAdapter::TMsvServerEntryInfo aEntryInfo, TBool aCreate, TBool& aIsLocal, const char* aContextText);
	TBool ProtectedFolder(TMsvId aFolder) const;
	TBool ProtectedRemoteServices() const;
	void GetCapsEntryCreateModifyL(const RMessage2& aMsg, TMsvId aFolder, TUid aMtm, TSecureId aOwnerId, TBool aCreateEntry, TBool aLocal, TCapabilitySet& aCaps);
	void GetCapsForMtmL(TUid aMtm, TCapabilitySet& aCaps);
	void GetNonOperationMtmDataL(TMsvId aServiceId, TNonOperationMtmDataType aMtmDataType, TPtrC8& aResultBuffer);
	void PoliceSearchSortQueryReadRequestL(const RMessage2& aMsg, const char* aContextText);	
	void CreateSearchSortCacheL();

	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	inline const CMsvMessageDBAdapter& MessageDBAdapter() const;
	inline CMsvMessageDBAdapter& MessageDBAdapter();
#endif
	
private:
	CMsvServer(TInt aPriority, TBool aDebug);
	void ConstructL();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		void DiskRemoved(const TDriveUnit& aDrive, TBool aTestRemoval = EFalse);
		void DiskInserted(const TDriveUnit& aDrive, TBool aTestInsert = EFalse);
		void DoDiskInsertedL(const TDriveUnit& aDrive, TBool aTestInsert);
		void DoDiskRemovedL(const TDriveUnit& aDrive, TBool aTestRemoval);  
	#else
		void DiskRemoved(const TDriveUnit& aDrive);
		void DiskInserted(const TDriveUnit& aDrive);
		void DoDiskInsertedL(const TDriveUnit& aDrive);
		void DoDiskRemovedL(const TDriveUnit& aDrive);
	#endif
	void CreatePreferredDriveListL();
	void DiskChanged(const TDriveUnit& aDrive); 
	void RunMailInitExe(TDriveNumber aDriveNum);
#else   
	TBool DiskRemoved(const TDriveUnit& aDrive);
	TBool DiskInserted(const TDriveUnit& aDrive);
	TBool DiskChanged(const TDriveUnit& aDrive, TUint aUniqueId);
	static void BuildDefaultIniFileL(TMsvConfig& aConfig);
	void RunMailInitExe();
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	void CreateRegistriesL();
	void CreateIndexFromResources();
	//
	void DoNotify(TMsvServerChangeNotificationType aChangeType,TBool aQueue = EFalse);
	void DoChangeDriveL(TInt aDrive, TRequestStatus* aStatus);
	//
	TInt CheckMtmStatus(TMsvId aServiceId, TBool& aRunning);
	void QueueOperationL(CMsvMtmOperation& aOperation, TInt aSessionId);
	CBaseServerMtm* LoadMtmL(const CMsvMtmOperation& aOperation);
	TBool SessionIdInQueue(CMsvMtmOperationQueue& aQueue, TInt aSessionId) const;
	inline void AddSessionIdToQueueL(CMsvMtmOperationQueue& aQueue, TInt aSessionId);
	//
	void StartNextOperation(CMsvMtmOperationQueue* aQueue, TInt aQueueIndex);
	void DoRemoveEntriesL(TMsvId aId);
	//
	CSession2* DoNewSessionL(const RMessage2 &aMessage);
	//
	void StartObserverExes();   
	//
	TInt GetEntryName(const CMsvIndexContext& aContext, TMsvId aId, TDes& aFileName, TBool aFolderRequired);
	//
	void ContextComplete(TInt aError, TBool aRunMailInit);
	//
	void SuspendSendingMessagesL(); 
	
#ifndef _NO_SERVER_LOGGING_
	void CreateLogL();
	mutable RFileLogger iLog;
#else
	TInt8 iBCSpacer[sizeof(RFileLogger)];
#endif

	void DoNotifyChangedBulkL(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aEntriesToNotify);

	//
private:
	TInt iSessionNumber;
	RFs iFs;
	TMsvNotifBuffer iChange;
	CServerMtmDllRegistry* iServerMtmReg;
	CMtmRegistryControl* iMtmRegControl;
	CArrayPtrFlat<CMsvMtmOperationQueue> iMtmOperationQueueArray;
#ifdef MSG_SERV_AUTO_CLOSE
	CMsvTimer* iCloseTimer;
#endif
	CMsvTimer* iDelayTimer;
	TBool iCloseServer;
	//
	CMsvIndexContext* iNewContext;
	CMsvIndexContext* iContext;

	TMsvServerChangeNotificationType iStartupState;
	TFileName iIndexFileName;
	TDriveUnit iIndexDrive;
	TInt iLoadError;
	TBool iDebug;
	
	//
	CMsvBackupHandler* iBackup;
	TMsvServerStoreManager  iServerStoreManager;
	RArray<TMsvId>		  iProtectedFolders;
	TBool				   iRemoteServicesProtected;
	CMsvMailinitWaiter*	 iMailinitWaiter;
	//variables for bulk creation of entries
	CMsvEntrySelection* iBulkCreationSelection;
	CMsvEntrySelection* iBulkChangeSelection;
	TBool iReleasedForRestore;
	TBuf<2> iSystemDrive;

	// Reference to freepool object.
	CMsvEntryFreePool*			  iFreePoolInstance;

	// Search Sort Cache Manager
	CMSvSearchSortCacheManager* iSearchSortCacheManager;

// Code added for PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsvPreferredDriveList* iDriveList;
	RPointerArray<CMsvDiskChangeNotifier> iNotifier;
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		friend class CTestMsvServer;
		friend class CTestIndexContext;
	#endif
#else
	CMsvNotifyDiskChange* iNotify;
#endif


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	CMsvMessageDBAdapter* iMessageDBAdapter;
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		friend class CTestMessageConverterWaiter;
	#endif  
#endif

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TMsvServerChangeNotificationType iNotification; //record notification sent for test purposes
	friend class CTestOOMSearchSortCacheManager;
	friend class CTestSearchSortDBadapter;
#endif
	};


//**********************************
// TMtmGroupRefCount
//**********************************
//
//
//

class TMtmGroupRefCount
/**
@internalComponent
@released
*/
	{
public:
	TMtmGroupRefCount(TUid aMtmTypeUid);
public:
	TUid iMtmTypeUid;
	TInt iRefCount;
	};

//**********************************
// CMsvOperationData
//**********************************
//
//
//

class CMsvOperationData : public CBase
/**
@internalComponent
@released
*/
	{
public:
	CMsvOperationData(TMsvOp aOpId);
	~CMsvOperationData();
	inline TMsvOp OperationId() const;
public:
	HBufC8* iBuffer;
private:
	TMsvOp iOpId;
	};

//**********************************
// CMsvServerSession
//**********************************
//
//
//

class CMsvServerSession : public CSession2 
/**
@internalComponent
@released
*/
	{
public:
	CMsvServerSession(CMsvServer& aServer);
	~CMsvServerSession();
	static CMsvServerSession* NewL(CMsvServer& aServer, const RMessage2 &aMessage);
	virtual void ServiceL(const RMessage2 &aMessage);
	// callback from server
	void NotifyChangedL(TMsvNotifBuffer& iChangeBuffer, TBool aQueue = EFalse);
	void StopOperations(TMsvId aServiceId);
	//
	TBool IsInUse(TUid aMtmTypeUid);
	inline TBool IsAnObserver();
	inline TInt SessionId();
	inline TInt HaveOutstandingOperations();
	inline const TDesC& ProcessName() const;
	inline TBool ReceiveEntryEvents() const;
	//
private:
	void DoServiceL(const RMessage2& aMessage);
	void ConstructL(const RMessage2& aMessage);
	void QueueSessionEventRequestL(const RMessage2 &aMessage);
	void CancelSessionEventRequest(const RMessage2 &aMessage);
	void SendNotificationL(const RMessage2& aMessage);
	void GetEntryL(const RMessage2 &aMessage);
	void GetChildrenL(const RMessage2 &aMessage);
	void GetRemainingChildrenL(const RMessage2 &aMessage);
	void LockEntryL(const RMessage2 &aMessage);
	void ReleaseEntryL(const RMessage2 &aMessage);
	void ReadStoreL(const RMessage2 &aMessage);
	void LockStoreL(const RMessage2 &aMessage);
	void ReleaseStoreL(const RMessage2 &aMessage);
	void DecStoreReaderCountL(const RMessage2 &aMessage);

	void CancelOperationL(const RMessage2 &aMessage);
	void OperationCompletionL(const RMessage2 &aMessage);
	void OperationProgressL(const RMessage2 &aMessage);
	void OperationSystemProgressL(const RMessage2 &aMessage);
	void OperationMtmL(const RMessage2 &aMessage);
	//
	void ChangeEntryL(const RMessage2 &aMessage);
	void ChangeEntriesL(const RMessage2 &aMessage);
	void DoChangeLocalEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId);
	void DoChangeRemoteEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage);
	void DoChangeLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId, TInt aMark);
	void DoChangeRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage,TInt aMark);
	//
	void CreateEntryL(const RMessage2 &aMessage);
	void DoCreateLocalEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId);
	void DoCreateRemoteEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage);
	//
	void MakeEntryFileDirectoryL(const RMessage2 &aMessage);
	//
	void TransferCommandL(const RMessage2 &aMessage);
	//
	void MoveEntriesL(const RMessage2 &aMessage);
	void DoMoveLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage);
	void DoMoveRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, TMsvId aTarget, const RMessage2 &aMessage, TBool aSourceLocal, TBool aTargetLocal, TBool aHasCapability);
	//
	void DeleteEntriesL(const RMessage2 &aMessage);
	void DoDeleteLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage);
	void DoDeleteRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage);
	//
	void CopyEntriesL(const RMessage2 &aMessage);
	void DoCopyLocalEntriesL( CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage);
	void DoCopyRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage, TBool aSourceLocal, TBool aTargetLocal, TBool aHasCapability);
	//
	void CopyOperationDataL(const RMessage2 &aMessage);
	void CopyCommandDataL(const RMessage2 &aMessage);
	HBufC8* RecoverOperationData(TMsvOp aOpId);
	//
	void PackEntryAndWriteBufferL(const RMessage2 &aMessage, TInt aParam, const TMsvEntry& aEntry, const TMsvId& aServiceId);
	//
	void WriteBufferL(const RMessage2 &aMessage, TInt aParam);
	void WriteL(const RMessage2 &aMessage, TInt aParam, const TDesC8& aDes);
	//
	inline void ReadMainBufferL(const RMessage2 &aMessage, TInt aParam);
	void ReadBufferL(const RMessage2 &aMessage, TInt aParam, HBufC8*& aBuffer);
	void ReadL (const RMessage2 &aMessage, TInt aParam, TDes8& aDes);
	//
	void FillRegisteredMtmDllArray(const RMessage2 &aMessage);
	void DoFillRegisteredMtmDllArrayL(const RMessage2 &aMessage);
	void MtmGroupL(const RMessage2 &aMessage, TBool aInstall);
	void UseMtmGroup(const RMessage2 &aMessage);
	void ReleaseMtmGroup(const RMessage2 &aMessage);
	void GetMtmGroupDataL(const RMessage2 &aMessage);
	void GetMtmRequiredCapabilitiesL(const RMessage2& aMessage);
	//
	void ServiceProgressL(const RMessage2 &aMessage);
	//
	void RemoveEntry(const RMessage2& aMessage);
	void MessageFolderL(const RMessage2 &aMessage);
	void ChangeAttributesL(const RMessage2& aMessage);
	void GetChildIdsL(const RMessage2& aMessage);
	void CopyStoreL(const RMessage2 &aMessage);
	void DeleteStoreL(const RMessage2 &aMessage);
	void OutstandingOperationsL(const RMessage2& aMessage);
	void GetNotifySequenceL(const RMessage2& aMessage);
	//
	void SetReceiveEntryEvents(const RMessage2 &aMessage);
	//
	void PanicClient(const RMessage2 &aMessage, TMsvClientPanic aPanic) const;
	void ChangeDriveL(const RMessage2 &aMessage);
	void CreateAttachmentForWriteL(const RMessage2 &aMessage);
	void ReplaceAttachmentForWriteL(const RMessage2 &aMessage);
	void OpenAttachmentL(const RMessage2 &aMessage);
	void OpenAttachmentForWriteL(const RMessage2 &aMessage);
	void DeleteAttachmentL(const RMessage2 &aMessage);
	void RenameAttachmentL(const RMessage2 &aMessage);
	void FileExistsL(const RMessage2 &aMessage);
	void GetAttachmentFilePathL(const RMessage2& aMessage);
	void OpenFileStoreForReadL(const RMessage2 &aMessage);
	void OpenTempStoreFileL(const RMessage2 &aMessage);
	void ReplaceFileStoreL(const RMessage2 &aMessage);
	void DeleteFileStoreL(const RMessage2 &aMessage);
	void FileStoreExistsL(const RMessage2 &aMessage) const;
	void ConnectAndShareFileSessionLC(RFs& aFs );
	void UpdateOwnerIdL(TMsvId aId, TSecureId aOwnerId);
	void BodyTextFilePathL(const RMessage2 &aMessage);
	void OpenPlainTextFileL(const RMessage2 &aMessage);
	void CreatePlainTextFileL(const RMessage2 &aMessage);
	void DeletePlainTextFileL(const RMessage2& aMessage);
	void ReplacePlainTextFileL(const RMessage2 &aMessage);
	void GetNonOperationMtmDataL(const RMessage2 &aMessage);
	
// Code added for PREQ 1667.
	void SearchSortOnHeaderAndBodytMsgL(const RMessage2 &aMessage);
	void SendSearchSortIndexEntryResultL(const RMessage2 &aMessage);
	void SearchSortOnIndexEntryL(const RMessage2& aMessage);
	void SendResultOrIdsToSearchL(const RMessage2& aMessage);
	void GetSearchSortResultCountL(const RMessage2& aMessage);
	void GetResultInIteratorL(const RMessage2& aMessage);
	void GetNextEntryInIteratorL(const RMessage2& aMessage);
	void GetQueryIDL(const RMessage2& aMessage);
	void QueryUnMarkedL(const RMessage2& aMessage);
	void UpdateSearchSortCacheWithSortFiledL(const RMessage2& aMessage);
	void GetResultForQueryIDL(const RMessage2& aMessage);
	void GetSearchSortProgressInfoL(const RMessage2& aMessage);
	void SearchSortOperationCancelL(const RMessage2& aMessage);
	void UpdateSearchSortResultsAndSortByEntryL(const RMessage2& aMessage);
	void CopyQueryDataL(const RMessage2& aMessage);

// Code added for PREQ 557. API for preferred drive list.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void GetChildrenAllL(const RMessage2 &aMessage);
	void GetChildIdsAllL(const RMessage2& aMessage);
	void MessageDrive(const RMessage2 &aMessage);
	void CurrentDriveInfoL(const RMessage2 &aMessage);
	void DriveListL(const RMessage2 &aMessage);
	void AvailableDriveListL(const RMessage2 &aMessage);
	void AddDriveL(const RMessage2 &aMessage);
	void RemoveDriveL(const RMessage2 &aMessage);
	void UpdateDrivePriorityL(const RMessage2 &aMessage);
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		void ResetRepositoryL(const RMessage2 &aMessage);
		void PrintCache(const RMessage2 &aMessage);
	#endif
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	void CreateHeaderTableL(const RMessage2 &aMessage);
	void DoesStoreExistsL(const RMessage2 &aMessage);
	void LastErrorMessageL(const RMessage2 &aMessage);
	
	void CreateBodyEntryL(const RMessage2 &aMessage);
	void LoadBodyEntryL(const RMessage2 &aMessage);
	void UpdateBodyEntryL(const RMessage2 &aMessage);
	void DeleteBodyEntryL(const RMessage2 &aMessage);
	
	void CreateHeaderEntryL(const RMessage2 &aMessage);
	void LoadHeaderEntryL(const RMessage2 &aMessage);
	void DeleteHeaderEntryL(const RMessage2 &aMessage);
	void UpdateHeaderEntryL(const RMessage2 &aMessage);
	void DoesAnyStoreExists(const RMessage2 &aMessage);
	void DoesHeaderTableExist(const RMessage2 aMessage);
	// Converter API's
	void GetConvertibleDriveListL(const RMessage2 &aMessage);
	void ConvertMessageStoreL(const RMessage2 &aMessage);
	void CancelConversionRequestL(const RMessage2 &aMessage);
	void GetConversionStatus(const RMessage2 &aMessage);
	TBool FoundUnSupportedDrives();
#endif	  // #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)


private:
	TInt		iSessionId;
	CMsvServer& iMsvServer;
	RMessage2   iQueuedMessage;
	TBool	   iRequestQueued;
	TBool	   iObserverOnly;
	HBufC8*	 iBuffer;
	CMsvChangeBuffer* iChangeBuffer;
	CArrayPtrFlat<CMsvServerOperation> iOperations;
	CArrayFixFlat<TMtmGroupRefCount> iMtmGroupRefCountArray;
	CArrayPtrFlat<CMsvOperationData> iOperationDataArray;
	TMsvChildrenDetails iChildrenDetails;
	CArrayPtrSeg<const TMsvEntry>* iChildrenSelection;
	CMsvEntrySelection* iChildrenSelectionIds;
	CMsvEntrySelection iLockedStores;
	TName iProcessName;
	TBool	   iReceiveEntryEvents;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TPtrC iLastDBErrorMessage;
	CMsvConverterWaiter* iConverterWaiter;
#endif
	};

#include "MSVSERV.INL"

#endif
