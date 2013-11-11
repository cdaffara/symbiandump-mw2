// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVDBADAPTER_H__
#define __MSVDBADAPTER_H__

#include <msvstd.h>
#include <msvids.h>
#include <msvuids.h>
#include <sqldb.h>
#include <msvuids.h>
#include "msvcacheentry.h"
#include <msvsearchsortquery.h>
#include "msvsearchsortconstants.h"

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include "msvmessagedbadapter.h"
#endif

/**
 * CONSTANT DEFINITIONS
 */
#define KMaxNumberOfDrives 8

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
class TStatusMtmIdTable;
class TConversionResumeStatus;
#endif

/**
@internalComponent
@prototype
*/

NONSHARABLE_CLASS (CMsvDBAdapter) : public CBase
	{
public:
	~CMsvDBAdapter();
	void BeginTransactionL();
	void CommitTransactionL();
	void RollbackTransactionL();
	void CreateEntryL(const TMsvEntry& aNewEntry, const TMsvId aImmediateVisibleFolder);
	void UpdateEntryL(const TMsvEntry& aEntry, const TMsvId aImmediateVisibleFolderconst, TBool aIsParentIdUpdated = ETrue);
	void GetEntryL(TMsvId aId, CMsvCacheEntry*& aEntry, TMsvId& aVisibleParentEntryId);
	void GetChildrenL(TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray);
	void GetChildrenIdL(TMsvId aParentId, CMsvEntrySelection& aChildArray);
	void GetChildrenL(const TDesC8& aQueryStr, TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray);
	void DeleteEntryL(TMsvId aEntryId);
	void DeleteEntryL(const CMsvEntrySelection& aEntryIdList);
	void GetVisibleFolderIdL(TMsvId aParentId, TMsvId& aVisibleFolderId);
	TBool EntryExistsL(TMsvId aId);
	TBool GetNextSiblingL(TMsvId aId,TMsvId aParentId,TMsvId& aNextSiblingId);
	TBool GetFirstChildIdL(TMsvId aParentId,TMsvId& aFirstChild);
	TBool GetVisibleFlagL(TMsvId aEntryId);
	void UpdateVisibleFolderL(CMsvEntrySelection* aEntryList, TMsvId aNewVisibleFolderId);
	void UpdateOwnerStatusL(TMsvId aId, const TMsvEntry& aParent, TBool aSet);
	inline RSqlDatabase& GetDbPointer();
	

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	static CMsvDBAdapter* NewL();	
	void DetachDBL(TUint aDriveId);
	inline TBool IsVisibleFolder(TInt32 aData);
	static void ValidateDatabaseL(TDriveNumber aDriveNumber);	
	void GetInPreparationIdL(CMsvEntrySelection& aSelection, TUint aDriveId=0);
	
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		static void CreateDBL(TDriveNumber aDriveNumber, TBool aCreateVersionTable=ETrue, TBool aInvalid = EFalse);
	#else
		static void CreateDBL(TDriveNumber aDriveNumber);
	#endif
	#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		static CMsvDBAdapter* OpenL(const TFileName& aDbFileName);
		void ConstructOpenDBL(const TFileName& aDbFileName);
		void InitializeLConverterStatementL();
		void CreateConversionStatusTableL();
		void RemoveConversionStatusTableL();
		void GetConversionStatusL(TConversionResumeStatus& aConversionResumeStatus);
		void CreateIndexEntryL(const TMsvEntry& aNewEntry, const TMsvId aImmediateVisibleFolder);
		void AddConversionStatusEntryL(TConversionResumeStatus& aConversionResumeStatus);
		void UpdateConversionStatusEntryL(TConversionResumeStatus& aConversionResumeStatus);
		void RemoveConversionStatusEntriesL();
		void ChangeVersionL(TInt aVersion);
		void GetAllMessageIdsL(TMsvId aServiceId,RArray<TMsvId>& aIdArray,TInt32 aMtm,TMsvId lastId = 0);
		void GetAllMimeIdsL(TMsvId aId, RArray<TMsvId>& aIdArray);
		void GetOwningServiceL(TMsvId aId, TMsvId& aOwningId);
		void GetAllServiceIDsForMtmL(TInt32 aMtm, RArray<TMsvId>& aIdArray);
		void GetAllIndexEntryTableIdsL(RArray<TMsvId>& aIdArray);
		void UpdateVisibleFolderIdL(TMsvId aId, TMsvId visibleId);
		TInt GetMessageCountL(TMsvId aServiceId);
		void GetRemainingServiceIdsL(RArray<TMsvId>& aIdArray);
		void GetVisibleParentL(TMsvId aId, TMsvId& aOwningId);
		void GetVisibleParentIdL(TMsvId aParentId, TMsvId& aVisibleFolderId);
		TBool GetParentVisibleFlagL(TMsvId parentId);
		void GetRemainingIdsL(RArray<TMsvId>& aIdArray);
		void DeleteTableL(TUid aMtmId);
		void DeleteSearchSortTable();
	#endif
#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	static CMsvDBAdapter* NewL(const TFileName& aDbFileName);
	static CMsvDBAdapter* OpenL(const TFileName& aDbFileName);
	static TInt DeleteDB(const TFileName& aDbFileName);	
	void GetMaxTMsvIdL(TMsvId& aMaxTMsvId);
	void GetInPreparationIdL(CMsvEntrySelection& aSelection);
	inline TInt Size();
	static void ValidateDatabaseL(const TFileName& aDbFile);
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	void SearchTMsvIdL(const TDesC8& aQuery, RArray<TMsvId>& aIdSelection);
	TInt AddEntriesfromIndexTableL(const TDesC8& aQuery);
	void CreateQueryEntryL(const TInt& aQueryId,const TMsvIdWithSortField& aEntry);
	void GetSearchSortEntriesL(const TInt& aQueryId, RArray<TMsvId>& aEntrySelection, const TBool aOrder = ETrue,const TBool aSortAsInt = EFalse);
	void GetLastNTMsvIdsfromTableL(const TInt& aQueryId, RArray<TMsvId>& aIdResult, TInt aLastCount, TBool aOrder = EFalse, TBool aSortAsInt = EFalse);
	void DeleteTMsvIdFromQueryL(const TInt& aQueryId,const TMsvId& aId);
	void DeleteTMsvIdFromAllQueriesL(const TMsvId& aIdSelection);
	void DeleteQueryFromTableL(const TInt& aQueryId);
	TInt ReturnIdCountInDBL(const TInt& aQueryId);
	TInt GetNextL(TMsvId& aId);
	TInt GetPrevL(TMsvId& aId);
	void GetFolderIdsL(const TMsvId aParentId, RArray<TMsvId>& aFolderIdList);
	void ReturnIdsInIteratorL(const TInt& aQueryId, const TBool aOrder = ETrue,const TBool aSortAsInt = EFalse, const TInt aLimit = 1);
	void ClearDBContentsL();
	void ConstructSortTableL();
	void InitializeSortTableL();
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	void GetMtmIdsL(const TDesC8& aQuery, RArray<TUid>& aMtmIdArray);
	void AttachDBL(TDriveUnit aDriveUnit, TUint aDriveId, TMsvId& aMaxId, CMsvMessageDBAdapter* iMessageDBAdapter = NULL);
#else
	void AttachDBL(TDriveUnit aDriveUnit, TUint aDriveId, TMsvId& aMaxId);	
#endif		// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	

private:

	CMsvDBAdapter();
	void InitializeL();
	void LoadEntryFromStatementL(RSqlStatement& aStmt, TInt aIndex, TMsvId& aVisibleParentEntryId, CMsvCacheEntry& aEntry);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void ConstructL();
	TMsvId MaxTMsvIdL(TUint aDriveId);
	static void CreateRootEntryL(RSqlDatabase& tempDB);
	void DoGetInPreparationIdL(CMsvEntrySelection& aSelection, TUint aDriveId);
#else
	void ConstructNewL(const TFileName& aDbFileName, TBool& aIsDBCreated);
	void ConstructOpenL(const TFileName& aDbFileName);
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	


// DATA		
private:


	enum iStatementType
		{
		ECreateEntry = 0,
		EUpdateEntry,			
		EUpdateEntryNoParent,
		EGetEntry,
		EGetChildEntries,
		EGetVisibleId,
		EEntryExists,
		EGetChildIds,
		EIndexSentinel
		};
		
	enum
		{
		KMsvEntryHighPriority=			0x00000002,
		KMsvEntryMediumPriority=		0x00000001,
		KMsvEntryLowPriority=			0x00000000,
		KMsvEntryNewFlag=				0x00000010,
		KMsvEntryUnreadFlag=			0x00000020,
		};

	RSqlDatabase iDatabase;
	TBool isTransactionOpen;
	RSqlStatement iStatement[EIndexSentinel];

	// column indices
	TInt iIdIndex[EIndexSentinel];
	TInt iParentIdIndex[EIndexSentinel];
	TInt iDataIndex[EIndexSentinel];
	TInt iPcSyncCountIndex[EGetVisibleId];
	TInt iReservedIndex[EGetVisibleId];
	TInt iRelatedIdIndex[EGetVisibleId];
	TInt iTypeIndex[EIndexSentinel];
	TInt iDateIndex[EGetVisibleId];
	TInt iSizeIndex[EGetVisibleId];
	TInt iErrorIndex[EGetVisibleId];
	TInt iBiotypeIndex[EGetVisibleId];
	TInt iMtmData1Index[EGetVisibleId];
	TInt iMtmData2Index[EGetVisibleId];
	TInt iMtmData3Index[EGetVisibleId];
	TInt iServiceIdIndex[EGetVisibleId];
	TInt iMtmIdIndex[EGetVisibleId];
	TInt iDescriptionIndex[EGetVisibleId];
	TInt iDetailsIndex[EGetVisibleId];	
	TInt iVisibleParent[EEntryExists];
	
	enum
		{
		EQueryCreateEntry = 0,
		EQueryGetEntryAsc,
		EQueryGetEntryDesc,
		EQueryGetEntryNumAsc,
		EQueryGetEntryNumDesc,
		EQueryGetLastNIdsAsc,
		EQueryGetLastNIdsDesc,
		EQueryGetLastNIdsNumAsc,
		EQueryGetLastNIdsNumDesc,
		EQueryCount,
		EGetFolderIds,
		EQueryIteratorNumAsc,
		EQueryIteratorNumDesc,
		EQueryIteratorAsc,
		EQueryIteratorDesc,
		EQuerySentinel
		};
	
	RSqlStatement iQueryStatement[EQuerySentinel];
	
	// column indices
	TInt iQIdQueryIndex[EQuerySentinel];
	TInt iIdQueryIndex[EQuerySentinel];
	TInt iSortfieldQueryIndex[EQuerySentinel];
	TInt iRowIdIndex[EQuerySentinel];
	TInt iIterOffset[EQuerySentinel];
	TInt iIterLimit[EQuerySentinel];
	TInt iParentIdfield;
	TInt iTMsvidfield;
	TInt iIdColumnIndex;
	
	// Iterator Variables
	TInt iOffsetValue;
	TInt iIterQuery;
	TInt iQueryId;
	TInt iLimit;
	
	/**************** Converter Statements ************************/
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	enum
	{
	ECreateConversionStatusEntry = 0,
	EUpdateConversionStatusEntry	
	};

#endif	


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	//Data structure to store maximum Ids on a given driveId.
	TBool iDbPtrInitialized;
	RArray<TBool>* iDatabasePresent;
#endif

	friend class TSearchSortDbWrapper;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	friend class CMsvMessageDBAdapter;
	friend class CMessageConverterManager;
#endif

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestDbAdapter;
	friend class CTestSearchSortDBadapter;
	friend class CTestVersion0Version1Converter;
	friend class CTestVersion1Version2Converter;
	friend class CTestConverterDBAdapter;
	friend class CTestSmsMessageV1toV2Converter;
#endif
	};
	
	

inline RSqlDatabase& CMsvDBAdapter::GetDbPointer()
	{
	return iDatabase;
	}
	
	
/**
 * Local Functions
 */


//Get the index of the given parameter name
inline TInt GetParamIndexL(RSqlStatement& aStatement, const TDesC& aParameterName)
	{
	TInt index = aStatement.ParameterIndex(aParameterName);
	User::LeaveIfError(index);
	return index;
	}




//Get the index of the given column name
inline TInt GetColumnIndexL(RSqlStatement& aStatement, const TDesC& aColumnName)
	{
	TInt index = aStatement.ColumnIndex(aColumnName);
	User::LeaveIfError(index);
	return index;
	}




//Binds the given value with the index given in the sql statement
inline void BindIntL(RSqlStatement& aStatement, TInt aIndex, TInt32 aId)
	{
	User::LeaveIfError(aStatement.BindInt(aIndex, aId));
	}




//Binds the given value with the index given in the sql statement
inline void BindInt64L(RSqlStatement& aStatement, TInt aIndex, TInt64 aId)
	{
	User::LeaveIfError(aStatement.BindInt64(aIndex, aId));
	}




inline void BindTextL(RSqlStatement& aStatement,TInt aIndex, const TDesC& aParameterText)
	{
	User::LeaveIfError(aStatement.BindText(aIndex,aParameterText));	
	}




//Gets the value of Int32 column
inline TInt ColumnInt(RSqlStatement& aStatement, TInt aIndex)
	{
	return aStatement.ColumnInt(aIndex);
	}



//Gets the value of Int64 column
inline TInt64 ColumnInt64(RSqlStatement& aStatement, TInt aIndex)
	{
	return aStatement.ColumnInt64(aIndex);
	}




//Get the value of text column
inline HBufC* ColumnTextL(RSqlStatement& aStatement, TInt aIndex)
	{
	TInt size = aStatement.ColumnSize(aIndex);
	HBufC* data = HBufC::NewL(size); 

	TPtr ptrData(data->Des());
	TInt err = aStatement.ColumnText(aIndex,ptrData);

	if(err != KErrNone)
		{
		delete data;
		User::Leave(err);
		}
	return data;	
}

	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	

inline TBool CMsvDBAdapter::IsVisibleFolder(TInt32 aData)
	{
	TMsvEntry dummy;
	dummy.iData = aData;
	return dummy.Visible();
	}
	
#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	
	
inline TInt CMsvDBAdapter::Size()
	{
	return iDatabase.Size();
	}

#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

#endif		// #ifndef __MSVDBADAPTER_H__
