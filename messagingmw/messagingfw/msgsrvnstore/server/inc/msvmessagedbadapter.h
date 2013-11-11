// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors: Neeraj Nayan
//
// Description: The class provides interface to read/write header and
// body entry from the database.
//

#ifndef __MSVMESSAGEDBADAPTER_H__
#define __MSVMESSAGEDBADAPTER_H__


/**
 * HEADER FILES
 */
#include <msvstd.h>
#include <sqldb.h>
#include <msvapi.h>


/**
 * FORWARD DECLARATIONS
 */
class CMsvDBAdapter;



/**
@internalComponent
@prototype
*/
NONSHARABLE_CLASS (CMsvMessageDBAdapter) : public CBase
	{
public:
	struct CFieldClass;
	struct CHeaderMetaData;
	
	~CMsvMessageDBAdapter();


	static CMsvMessageDBAdapter* NewL(CMsvDBAdapter* aDBAdapter,TBool aDbInCurrentDrive = EFalse);

	TBool IsHeaderTableExistsL(const TUid& aMtmId);
	void CreateHeaderTableL(const TUid& aMtmId, const RPointerArray<CFieldPair>& aFieldDetails, TPtrC& aErrorMessage);
	void CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& iFieldPairList);
	void LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList);
	void DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId);
	void CopyHeaderEntryL(const TUid& aMtmId, const TMsvId& aSrcEntryId, const TMsvId& aDestEntryId);
	void UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList);
	TBool CheckHeaderTableAndStdColumnFields(const TUid& aMtmId, TInt aHeaderFilelds);
	TBool DoesAnyStoreExistsL(TMsvId aId, TUid aMtmId);
private:
	void ConstructL(TBool aDbInCurrentDrive);
	void UpdateMetadataStructureL(TUint aDriveId);
	CMsvMessageDBAdapter(CMsvDBAdapter* aDBAdapter);
	inline TInt GetMetadataEntryIndex(const TUid& aMtmId);
	void DoCreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList, TInt aMetadataEntryIndex);
 	void DoUpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, CHeaderFields* aHeaderFields, TInt aMetadataEntryIndex);
 	void DoProcessHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList, RArray<TInt>& aUidList);
 	void DoInsertHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, CHeaderFields* aHeaderFields, TInt aMetadataEntryIndex);
	void CheckStdColumnsAdded(HBufC* aFieldName, TInt& aColStatus);
	void DoLoadHeaderEntryL(RSqlStatement& aSqlStmt, const RPointerArray<CFieldClass>& aMetadataList, RPointerArray<CHeaderFields>& aFieldPairList);
	void DoCreateHeaderTableCreationQueryL(const RPointerArray<CFieldPair>& aFieldDetails, RBuf16& aHeaderTableQuery, CHeaderMetaData*& aHeaderMetaData);

// DATA		
private:

	// Hold information for a column in the header table.
	struct CFieldClass
		{
		~CFieldClass()
			{
			delete iName;
			}
		TSqlColumnType iType;		// Column Type
		HBufC* iName;				// Column Name
		};
	
	// Holds information for a header table.
	struct CHeaderMetaData
		{
		CHeaderMetaData()
			{
			iStdColumnStatus = 0;
			}
		~CHeaderMetaData()
			{
			iFieldList.ResetAndDestroy();
			}
		RPointerArray<CFieldClass> iFieldList;		// Column details
		TInt iStdColumnStatus;						// Status of standard columns in the header table.
		TUid iMtmId;								// MTM id of the header table.
		};
	
	CMsvDBAdapter* iDBAdapter;
	RSqlDatabase& iDatabaseRef;
	
	// Header metadata structure. The object holds a separate 
	// CHeaderMetaData entry for each header table in the database. 
	RPointerArray<CHeaderMetaData> iHeaderDataList;
	TBool iNonAttachedDrive;
	
	friend class CMsvDBAdapter;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestMessageDbAdapter;
	friend class CTestMsvServer;
#endif
	};
	
	
	
	


/*
 * GetMetadataEntryIndex()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @return index: Index in the metadata structure
 *
 * The function returns the index of the header metadata information
 * in the metadata structure. If the header information is not 
 * present in the metadata structure, it reads the metadata structure
 * from the database and return the index.
 */ 
inline TInt CMsvMessageDBAdapter::GetMetadataEntryIndex(const TUid& aMtmId)
	{
	// Check header structure in metadata structure.
	for(TInt index=0; index<iHeaderDataList.Count(); index++)
		{
		if(iHeaderDataList[index]->iMtmId == aMtmId)
			{
			return index;
			}
		}	
	return KErrNotFound;
	}
	


#endif		// #ifndef __MSVMESSAGEDBADAPTER_H__
