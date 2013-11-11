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

#if !defined(__MCLIENT_H__)
#define __MCLIENT_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#if !defined(__MSVIPC_H__)
#include <msvipc.h>
#endif

#if !defined(__MSVREG_H__)
#include <msvreg.h>
#endif

#include <mmsvstoremanager.h>
#include <tnonoperationmtmdata.h>



#if !defined(__MSVSEARCHSORTQUERY_H__)
#include <msvsearchsortquery.h>
#endif
class CMsvSearchSortQuery;
class CMSvSearchSortOperation;

// Forward declarations
class CMsvClientEntry;
class TCapabilitySet;
class TMsvSystemProgress;


//**********************************
//RMsvSession
//**********************************
//
// NOTE most functions are exported for test purposes only
//
/**
Low level client-side API to access the message server. 

It should not be used directly by client applications. They should use higher-level helper classes such as 
CMsvSession, CMsEntry, and CMsvStore. These helper classes access the message server through RMsvServerSession.

@internalTechnology
*/
class RMsvServerSession : public RSessionBase, public MRegisteredMtmDllObserver, public MMsvStoreManager
	{
public:
	IMPORT_C RMsvServerSession();
	IMPORT_C virtual ~RMsvServerSession();
	IMPORT_C TInt Connect(RFs& aFs);
	IMPORT_C TVersion Version() const;
	// Synchronous message
	IMPORT_C TInt GetEntry(TMsvId aId, TMsvId& aService, TMsvEntry& aEntry);
	IMPORT_C TInt GetChildren(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering);
	IMPORT_C TInt LockEntry(TMsvId aId);
	IMPORT_C TInt ReleaseEntry(TMsvId aId);
	IMPORT_C TInt ReadStore(TMsvId aId);
	IMPORT_C TInt LockStore(TMsvId aId);
	IMPORT_C TInt ReleaseStore(TMsvId aId);
	TInt DecStoreReaderCount(TMsvId aId);
	IMPORT_C TInt OperationProgress(TMsvOp aId, TDes8& aProgress);
	IMPORT_C TInt OperationSystemProgress(TMsvOp aId, TMsvSystemProgress& aProgress);
	IMPORT_C TInt OperationCompletion(TMsvOp aId, TDes8& aProgress);
	IMPORT_C TInt CancelOperation(TMsvOp aId, TDes8& aProgress);
	IMPORT_C TInt CancelSessionEventRequest();
	IMPORT_C TInt OperationMtmL(TMsvId aId1, TMsvId aId2, TUid& aMtm, TMsvId& aService);
	inline TInt OperationMtmL(TMsvId aId1, TUid& aMtm, TMsvId& aService);
	IMPORT_C void CreateEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId);
	IMPORT_C void ChangeEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId);
	IMPORT_C void MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId);
	IMPORT_C void CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId);
	IMPORT_C void DeleteEntriesL(const CMsvEntrySelection& aSelection, TMsvOp aOperationId);
	IMPORT_C void ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	IMPORT_C void GetChildIdsL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);
	void ChangeDriveL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aRequestStatus);

	TInt OutstandingOperationsL();
	// Asynchronous messages
	IMPORT_C void CreateEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus);
	IMPORT_C void ChangeEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus);
	void ChangeEntriesL(const CMsvEntrySelection& aSelection,TBool aMark, TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus);
	
	IMPORT_C void DeleteEntriesL(const CMsvEntrySelection& aSelection, TMsvOp aOperationId, TRequestStatus& aRequestStatus);
	IMPORT_C void MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId, TRequestStatus& aRequestStatus);
	IMPORT_C void CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId, TRequestStatus& aRequestStatus);
	IMPORT_C void TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TMsvOp aOperationId, TRequestStatus& aStatus);
	IMPORT_C TInt TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TMsvOp aOperationId);
	IMPORT_C void QueueSessionEventRequest(TDes8& aChange, TDes8& aSequence, TRequestStatus& aRequestStatus);
	void CopyStoreL(const TDriveUnit& aDrive, TMsvOp aOperationId, TRequestStatus& aStatus);
	void DeleteStoreL(const TDriveUnit& aDrive, TMsvOp aOperationId, TRequestStatus& aStatus);
	
	//search sort
	void SearchSortRequestOnHeaderBodyL(const CMsvSearchSortQuery* aQuery, TMsvOp aOperationId, TBool aMarkQuery, TRequestStatus& aRequestStatus);
	void SearchSortRequestOnEntryL(const CMsvSearchSortQuery* aQuery, TMsvOp aOperationId, TBool aMarkQuery, TInt aIterator, TRequestStatus& aRequestStatus);
	void GetResultsForQueryId(TInt& aQueryId, TMsvOp aOperationId, TInt aIterator, TRequestStatus& aRequestStatus);
	void GetResultAsIdL(RArray<TMsvId>& aId);
	void GetResultAsIdL(RArray<TMsvId>& aId, TInt& aResultStatus);
	void GetSearchSortRequestQueryIdL(TInt& aQueryId);
	void UnmarkSearchSortRequestQueryIdL(TInt aQueryId);
	void SendResultantListL(TMsvOp aOperationId, RArray<TMsvIdWithSortField> aTMsvIdWithSortField, TRequestStatus& aRequestStatus);	
	void SendNewResultsToServerForSortL(TMsvOp aOperationId, RArray<TMsvId>& aIdArray, TRequestStatus& aRequestStatus);
	void GetResultCountL(TInt& aCount);
	void GetNextIdL(TMsvId& aId, TInt& aCount);
	void GetNextEntryL(TMsvEntry& aEntry, TInt& aCount);
	void GetQueryFromServerL(TMsvOp aOperationId, TInt aQueryId, CMsvSearchSortQuery* aQuery);
	void SearchSortOperationProgressL(TMsvOp aOperationId, TInt aOperationProgress);
	int CancelSearchSortOperation(TMsvOp aOperationId);
	
	// MTM 
	IMPORT_C TInt FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);  // Fill array with Dlls whose second uid is aMtmDllTypeUid
	IMPORT_C TInt InstallMtmGroup(const TDesC& aFullName);
	IMPORT_C TInt DeInstallMtmGroup(const TDesC& aFullName); 
	IMPORT_C TInt UseMtmGroup(TUid aMtmTypeUid);
	IMPORT_C TInt ReleaseMtmGroup(TUid aMtmTypeUid);  
	IMPORT_C CMtmGroupData* GetMtmGroupDataL(TUid aMtmTypeUid);  
	void GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCapSet) const;
	TBool GetAndClearIndexCorruptFlagL();
	TBool DriveContainsStoreL(TDriveUnit aDrive);
	// Server
	IMPORT_C void CloseMessageServer();
	IMPORT_C void RemoveEntry(TMsvId aId);
	IMPORT_C TInt GetMessageDirectory(TDes& aDirectory);
	TInt SetSessionAsObserver();
	TUint32 NotifySequenceL();
	IMPORT_C TInt SetReceiveEntryEvents(TBool aReceive);
	// Services
	IMPORT_C TInt StopService(TMsvId aServiceId);
	IMPORT_C TBool ServiceActive(TMsvId aServiceId);
	IMPORT_C TInt ServiceProgress(TMsvId aServiceId, TDes8& aProgress);
	//
	IMPORT_C void SetFailure(TInt aType, TInt aArg1 = 0, TInt aArg2 = 0, TInt aArg3 = 0);
	TInt GetMessageDrive();

	TInt GetNonOperationMtmData(TMsvId aServiceId, const TNonOperationMtmDataType& aType, TDes8& aMtmData);

	// from MMsvStoreManager
	IMPORT_C TBool CreateAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	IMPORT_C void ReplaceAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	IMPORT_C void OpenAttachmentL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile);
	IMPORT_C void OpenAttachmentForWriteL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile);
	IMPORT_C TInt DeleteAttachment(TMsvId aEntryId, const TDesC& aFilePath);
	IMPORT_C TInt RenameAttachment(TMsvId aEntryId, const TDesC& aOldFilePath, const TDesC& aNewName);
	IMPORT_C TBool FileExistsL(const TDesC& aFilePath);
	IMPORT_C void AttachmentFilePathL(TMsvId aEntryId, TDes& aFilePath);
	IMPORT_C TInt OpenFileStoreForRead(TMsvId aEntryId, RFile& aFile);
	IMPORT_C void OpenTempStoreFileL(TMsvId aEntryId, RFile& aFile);
	IMPORT_C void ReplaceFileStoreL(TMsvId aEntryId);
	IMPORT_C void DeleteFileStoreL(TMsvId aEntryId);
	IMPORT_C TBool FileStoreExistsL(TMsvId aEntryId) const;
	IMPORT_C TBool MessageStoreDrivePresentL();
	IMPORT_C TBool CreateShareProtectedAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile);
	IMPORT_C void BodyTextFilePathL(TMsvId aBodyTextId, TDes& aFilePath);
	IMPORT_C void OpenBodyTextFileForReadL(RFile& aFile, TMsvId aBodyTextId, const TDesC& aFilePath);
    IMPORT_C void CreatePlainTextFileL(RFile& aFile, TMsvId aBodyTextId);
	IMPORT_C void DeletePlainTextFileL(TMsvId aBodyTextId);
	IMPORT_C void ReplacePlainTextFileL(TMsvId aBodyTextId);

// Changed for PREQ 557. Preferred list related APIs.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	IMPORT_C TInt GetChildrenAll(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering);	
	IMPORT_C void GetChildIdsAllL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);
	IMPORT_C void CurrentDriveInfoL(TDriveNumber& aDriveNumber, TUint& aPriority);
	IMPORT_C void DriveListL(RArray<TDriveNumber>& aDriveList);
	IMPORT_C void AvailableDriveListL(RArray<TDriveNumber>& aDriveList);	
	IMPORT_C void AddDriveL(TDriveNumber aDriveNumber, TUint& aPriority);
	IMPORT_C void RemoveDriveL(TDriveNumber aDriveNumber);
	IMPORT_C void UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aPriority);
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		void ResetRepositoryL();
		IMPORT_C void PrintCache();
	#endif
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	void CreateStoreL(const TUid& aMtmId, const RPointerArray<CFieldPair>& aFieldDetails);	
	TBool DoesStoreExistsL(const TUid& aMtmId);
	void LastErrorMessageL(HBufC*& aErrorMsg);
	
	// Function from MMsvStoreManager
	IMPORT_C void CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList);
	IMPORT_C void LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList);
	IMPORT_C void DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId);
	IMPORT_C void UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList);
	IMPORT_C TBool DoesAnyStoreExists(TMsvId aId, TUid aMtmId);
	IMPORT_C TBool DoesHeaderTableExist(const TUid& aMtmId);

	void GetConvertibleDriveListL(RArray<TDriveNumber>& aDriveList);
	void ConvertMessageStore(TDriveNumber aDriveList,TRequestStatus& aRequestStatus);
	void GetConversionStatusL(TDriveNumber aDrive);
	TInt CancelConversion(TDriveNumber aDrive);
#endif
private:
	void DoGetChildrenL(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering);
	void DoGetRemainingChildrenL(TPckgBuf<TMsvChildrenDetails>& aDetails, CArrayPtrFlat<CMsvClientEntry>& aEntries);
	void PackEntryIntoBufferL(const TMsvEntry& aEntry);
	void SendEntryDataL(TMsvOp aOperationId, const TMsvEntry& aEntry);
	void SendOperationDataL(TMsvOp aOperationId, const CMsvEntrySelection& aSelection, TInt aParameter1=0, TInt aParameter2=0);
	void SendCommandDataL(TMsvOp aOperationId, const CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter);
	void TestSlotAvailableL();
	void PackOperationDataL(const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void DoGetChildrenAllL(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering);
#endif
	
private:
	HBufC8* iBuffer;
	};

/**
Gets the MTM that would perform an operation on a specified entry.

@param aId1 ID of the entry
@param aMtm On return, the UID of the MTM that would be used 
@param aService On return, the ID of the service that would be used 
@return System wide error code

@capability None
*/
inline TInt RMsvServerSession::OperationMtmL(TMsvId aId1, TUid& aMtm, TMsvId& aService)
	{
	return OperationMtmL(aId1, aId1, aMtm, aService);
	}
	
#endif // __MCLIENT_H__

