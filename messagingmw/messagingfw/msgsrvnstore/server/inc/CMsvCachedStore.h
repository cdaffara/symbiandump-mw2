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
// Contributors:
//
// Description:
//

#ifndef __CMSVCACHEDSTORE_H__
#define __CMSVCACHEDSTORE_H__

#include <e32base.h>
#include <f32file.h>
#include <s32mem.h>

class CMsvStore;
class RFileWriteStream;

#include "MSVSTD.H"

class MMsvStoreManager;

/**

The TPairedTUidHBufC8  class

 Implementes a uid/HBufC8 pair, with functions to load/save them to/from a file

*/
class TPairedTUidHBufC8
/**
@internalComponent
@released
*/
	{
public:
	TPairedTUidHBufC8();
	TUid	iUid;
	HBufC8 *iBuf;
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	};


/**

The CMsvCachedStore  class

 Implementes a set of uid/HBufC8 pairs, and persists them to a file
 Replaces a Dictionary Store in the message server. The difference
 being that this class keeps all the pairs in memory.

*/
class CMsvCachedStore : public CBase
/**
@internalComponent
@released
*/
	{
public:

	static CMsvCachedStore* OpenL(TMsvId aId, MMsvStoreManager& aStoreManager, TBool aReadOnly);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	static CMsvCachedStore* OpenL(TMsvId aId, MMsvStoreManager& aStoreManager, TBool aReadOnly,TUid aMtmId);
	void AssignL(CHeaderFields* aHeaderFields);
	void GetHeaderL(TUid aUid, CHeaderFields*& aHeaderFields);
	void LoadHeaderEntryL(const TUid aMtmId,TBool aReadOnly);
#endif	

	TInt Size() const;
	void DeleteL();

	TBool IsNullL() const;
	TBool IsPresentL(TUid aUid) const;
	void Remove(TUid aUid);
	void RemoveL(TUid aUid);
	void Revert();
	void RevertL();
	TInt Commit(); 
	void CommitL();

	~CMsvCachedStore();
private:
	HBufC8* GetStreamL(TUid aUid) const;
	void CreateOrReplaceStreamL(TUid aUid, CBufBase &aBuf);
	TInt FindStream(TUid aUid,TInt &pos) const;
	TInt DeleteStream(TUid aUid);

	CMsvCachedStore(TMsvId aId, MMsvStoreManager& aStoreManager);

	void CleanArray();
	static HBufC8* FlattenLC(CBufBase &aBuf);

	void ConstructL(TBool aReadOnly);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	void ConstructDBL(TBool aReadOnly,TUid aMtmId);
#endif
	
	void LoadL(RFile &aFile);
	void SaveL();
	void WriteToFileStreamL(RFileWriteStream &aOut);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
public:
	TUid iMtmId;
	RPointerArray<CHeaderFields> iHeaderFieldList;
	TBool isBodyPartDeleted;
	TBool isNewEntry;
	TBool isDbStore;
#endif		
private:


	CArrayFix<TPairedTUidHBufC8> *iStreams;
	TInt   iReaderCount;
	TInt   iWriterCount;
	
	const TMsvId iEntryId;
	MMsvStoreManager& iStoreManager;
	TInt iSize;

//
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
friend class CMsvStore;
#endif
friend class RMsvReadStream;
friend class RMsvWriteStream;
friend class HMsvReadBuf;
friend class HMsvWriteBuf;
};


#endif
