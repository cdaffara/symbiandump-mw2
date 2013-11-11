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

#include "CMsvCachedStore.h"
#include "MSVSTORE.H"
#include "MSVUTILS.H"

#include <mmsvstoremanager.h>

const TInt KMsvFlatBufferChunkSize=0x400;


enum TCMsvCachedStorePanic {
	EReadingWhileCommitingStream=1,
	EReadingWhileCommitingStore=2,
	EReadingOrWritingWhileRevertingStore=3,
	EReadingOrWritingWhileDeleteStream=4
	};

_LIT(KCachedStorePanic, "CMsvCachedStore");

void Panic(TCMsvCachedStorePanic aPanic)
	{
	User::Panic(KCachedStorePanic,aPanic);
	}


class HMsvReadBuf : public TMemBuf
	{
public:
	HMsvReadBuf(const TDesC8 &aBuf, CMsvCachedStore &aStore);
private:
	virtual void DoRelease();
private:
	CMsvCachedStore &iCachedStore;
	};


class HMsvWriteBuf : public TBufBuf
	{
public:
	static HMsvWriteBuf* NewL(TUid aUid,CMsvCachedStore& aStore);
private:
	HMsvWriteBuf(TUid aUid,CMsvCachedStore& aStore);
	virtual void DoRelease();
	virtual void DoSynchL();
private:
	CBufBase*        iBuf;
	CMsvCachedStore* iStore;
	TUid			 iUid;
	};


const TUid KMsvEntryFile={0x10003C68};

TPairedTUidHBufC8::TPairedTUidHBufC8() : iBuf(NULL) {}

void TPairedTUidHBufC8::ExternalizeL(RWriteStream &aStream) const
	{
	aStream << iUid;
	aStream << *iBuf;
	}

void TPairedTUidHBufC8::InternalizeL(RReadStream &aStream)
	{
	aStream >> iUid;
	// the 0x1000000 means that it will return KErrCorrupt
	// if the file says it contains a string of longer than 16 meg
	iBuf=HBufC8::NewL(aStream,0x1000000);
	}


CMsvCachedStore* CMsvCachedStore::OpenL(TMsvId aId, MMsvStoreManager& aStoreManager, TBool aReadOnly)
	{
	CMsvCachedStore* me = new(ELeave) CMsvCachedStore(aId, aStoreManager);
	CleanupStack::PushL(me);
	me->ConstructL(aReadOnly);
	CleanupStack::Pop(me);
	return(me);
	}


CMsvCachedStore::CMsvCachedStore(TMsvId aId, MMsvStoreManager& aStoreManager)
	: iEntryId(aId), iStoreManager(aStoreManager)
	{
	}

void CMsvCachedStore::ConstructL(TBool aReadOnly)
	{
	iStreams=new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);

	RFile storeFile;
	TInt err = iStoreManager.OpenFileStoreForRead(iEntryId, storeFile);
	
	if( err != KErrNotFound )
		{
		if( err==KErrNone )
			LoadL(storeFile); // the load takes ownership of the RFile
		else
			User::Leave(err);
		}
	else if(aReadOnly)
		User::Leave(err);
	}

TInt CMsvCachedStore::Size() const
	{
	return iSize;
	}

void CMsvCachedStore::DeleteL()
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iHeaderFieldList.Count())
		{
		iStoreManager.DeleteHeaderEntryL(iMtmId, iEntryId);
		}	
#endif

	iStoreManager.DeleteFileStoreL(iEntryId);
	}



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

CMsvCachedStore* CMsvCachedStore::OpenL(TMsvId aId, MMsvStoreManager& aStoreManager, TBool aReadOnly,TUid aMtmId)
	{
	CMsvCachedStore* me = new(ELeave) CMsvCachedStore(aId, aStoreManager);
	CleanupStack::PushL(me);
	me->ConstructDBL(aReadOnly,aMtmId);
	CleanupStack::Pop(me);
	return(me);
	}


void CMsvCachedStore::ConstructDBL(TBool aReadOnly,TUid aMtmId)
	{
	iStreams=new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);

	RFile storeFile;
	TInt err = iStoreManager.OpenFileStoreForRead(iEntryId, storeFile);
	
	if(err != KErrNotFound)
		{
		if( err==KErrNone )
			{
			LoadL(storeFile); // the load takes ownership of the RFile
			if(aReadOnly)
				isNewEntry = EFalse;
			else
				isNewEntry = ETrue;
			TRAP_IGNORE(LoadHeaderEntryL(aMtmId,aReadOnly));
			}
		else
			{
			User::Leave(err);
			}
		}
	else 
		{
		TRAPD(err2, LoadHeaderEntryL(aMtmId,aReadOnly));
		if (err2 == KErrNotFound)
				{
				User::Leave(err);
				}
		}
	isDbStore=iStoreManager.DoesHeaderTableExist(aMtmId);	
	}



/**
Assign the  header Fields in to header Field list.
@param aHeaderFields : CHeaderFields*
@return None.
*/
void CMsvCachedStore::AssignL(CHeaderFields* aHeaderFields)
	{
	TUid uid = aHeaderFields->iUid;
	for(TInt index=0; index<iHeaderFieldList.Count(); index++)
		{
		if(iHeaderFieldList[index]->iUid == uid)
			{
			delete iHeaderFieldList[index];
			iHeaderFieldList[index] = aHeaderFields;
			return;
			}		
		}
	iHeaderFieldList.AppendL(aHeaderFields);
	}

/**
Get the header Fields for respective UID.
@param aUid : A Uid
@param aHeaderFields : CHeaderFields*&

@return None.
*/

void CMsvCachedStore::GetHeaderL(TUid aUid, CHeaderFields*& aHeaderFields)
	{
	for(TInt index=0; index<iHeaderFieldList.Count(); index++)
		{
		if(iHeaderFieldList[index]->iUid == aUid)
			{
			aHeaderFields = iHeaderFieldList[index];
			return;
			}	
		}
	User::Leave(KErrNotFound);
	}


/**
Load the Header Entry .
@param aMtmId A Mtm TUid.
@return None.
@internalComponent
*/

void CMsvCachedStore::LoadHeaderEntryL(const TUid aMtmId,TBool aReadOnly)
	{
	TRAPD (err ,iStoreManager.LoadHeaderEntryL(aMtmId, iEntryId, iHeaderFieldList));
	if((err == KErrNotFound) && aReadOnly)
		{
		User::Leave(err);
		}
		
	if(err == KErrNotFound)
		{
		isNewEntry = ETrue;
		iSize = 0;
		}
	else
		{
		isNewEntry = EFalse;
		if(err == KErrNone)
			{
			for(TInt index=0; index<iHeaderFieldList.Count(); index++)
				{
				for(TInt i =0 ; i < (iHeaderFieldList)[index]->iFieldPairList.Count() ;i++ )
					{
					if(((iHeaderFieldList)[index]->iFieldPairList[i]->iFieldTextValue) != NULL)
						iSize += (iHeaderFieldList)[index]->iFieldPairList[i]->iFieldTextValue->Length();
					}
				}
			}
		else
			{
			User::Leave(err);	
			}
		}
	}
	
#endif
/**

The CMsvCachedStore::Remove method

Removes the stream from the array, ignores the fact that
it might not exist.

@internalAll
@param				 aUid	Stream to remove
*/
void CMsvCachedStore::Remove(TUid aUid)
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	for(TInt index=0; index<iHeaderFieldList.Count(); index++)
		{
		if( (iHeaderFieldList)[index]->iUid == aUid )
			{
			iHeaderFieldList.Remove(index);
			TRAP_IGNORE(iStoreManager.UpdateHeaderEntryL(iMtmId, iEntryId, iHeaderFieldList)); 
			return;
			}
		}	
#endif

	DeleteStream(aUid);
	}


/**

The CMsvCachedStore::RemoveL method

Removes a stream from the array, leaves if the stream is not found

@leave 
@internalAll
@param				 aUid	Stream to remove
*/
void CMsvCachedStore::RemoveL(TUid aUid)
	{
	User::LeaveIfError(DeleteStream(aUid));
	}


/**

The CMsvCachedStore::Revert method

Reloads the array of streams from the store
ignores errors that might occur.


@internalAll
*/
void CMsvCachedStore::Revert()
	{
     TRAP_IGNORE(RevertL());
	}


/**

The CMsvCachedStore::RevertL method

  Reloads the streams from the file

@leave
@internalAll
*/
void CMsvCachedStore::RevertL()
	{
	__ASSERT_ALWAYS(iReaderCount==0 && iWriterCount==0, Panic(EReadingOrWritingWhileRevertingStore));
	CleanArray();
	iStreams=new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);
	
	RFile storeFile;
	User::LeaveIfError(iStoreManager.OpenFileStoreForRead(iEntryId, storeFile));
	LoadL(storeFile); // the load takes ownership of the RFile	

	}


/**

The CMsvCachedStore::Commit method

  Saves the current state of the stream array to the file

@return			error
@internalAll
*/
TInt CMsvCachedStore::Commit() 
	{
	TRAPD(error,CommitL());
	return(error);
	}


/**

The CMsvCachedStore::CommitL method

  Saves the current state of the stream array to the file
  leaves with if error occurs

@leave
@internalAll
*/
void CMsvCachedStore::CommitL()
	{
	__ASSERT_ALWAYS(iWriterCount==0,Panic(EReadingWhileCommitingStore));
	SaveL();
	}


void CMsvCachedStore::CleanArray()
	{
	if(iStreams!=NULL)
		{
		TInt count=iStreams->Count();
		while(count--) delete iStreams->At(count).iBuf;
		}
	delete iStreams;
	iStreams=NULL;
	}

/**

The CMsvCachedStore::~CMsvCachedStore method

@internalAll
*/
CMsvCachedStore::~CMsvCachedStore()
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	iHeaderFieldList.ResetAndDestroy();
	iHeaderFieldList.Close();
#endif
	CleanArray();
	}


TBool CMsvCachedStore::IsNullL() const
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iStreams->Count()==0)
		{
		if(iHeaderFieldList.Count() ==0)
			{
			return ETrue;
			}
		else
			{
			return EFalse;
			}
		}
	else
		{
		return EFalse;
		}
#else
	return(iStreams->Count()==0);
#endif
	}


/**

The CMsvCachedStore::IsPresentL method

  checks to see if the stream is in the array

@return		ETrue if it exists EFalse if not.
@internalAll
@param				 aUid	stream to check for
*/
TBool CMsvCachedStore::IsPresentL(TUid aUid) const
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	for(TInt index=0; index<iHeaderFieldList.Count(); index++)
		{
		if(iHeaderFieldList[index]->iUid == aUid)
			{
			return ETrue;
			}
		}
#endif 	
	TInt pos;
	if(FindStream(aUid,pos)==KErrNone) return(ETrue);
	else return(EFalse);
	}



/**

The CMsvCachedStore::CreateOrReplaceStreamL method

  If a stream is present with this id it replaces the
  data in that stream with aBuf, otherwise it adds a new
  stream with the data in aBuf

@internalAll
@param				 aUid	stream id
@param				 *aBuf	data, takes ownership
*/
void CMsvCachedStore::CreateOrReplaceStreamL(TUid aUid, CBufBase &aBuf)
	{
	__ASSERT_ALWAYS(iReaderCount==0, Panic(EReadingWhileCommitingStream));
	TInt pos;
	HBufC8* buffer=FlattenLC(aBuf);
	if(FindStream(aUid,pos)==KErrNone)
		{
		delete iStreams->At(pos).iBuf;
		iStreams->At(pos).iBuf=buffer;
		}
	else
		{
		TPairedTUidHBufC8 entry;
		entry.iUid=aUid;
		entry.iBuf=buffer;
		iStreams->InsertL(pos,entry);
		}
	CleanupStack::Pop(buffer);
	}

HBufC8* CMsvCachedStore::FlattenLC(CBufBase &aBuf)
	{
	TInt size=aBuf.Size();
	HBufC8 *buffer=HBufC8::NewLC(size);
	TPtr8 ptr(buffer->Des());

	RDesWriteStream writer(ptr);
	RBufReadStream reader(aBuf);
	writer.WriteL(reader,size);
	writer.CommitL();
	return(buffer);
	}


/**

The CMsvCachedStore::GetStream method

  If it returns with KErrNone it sets aData to point to the
  data in the store this id

@return			error code if the stream was not found
@internalAll
@param				 aUid	stream id
@param				 *&aData	returned pointer to data
*/
HBufC8* CMsvCachedStore::GetStreamL(TUid aUid) const
	{
	TInt pos;
	if(FindStream(aUid, pos) != 0)
		User::Leave(KErrNotFound);
	
	return iStreams->At(pos).iBuf;
	}


/**

The CMsvCachedStore::FindStream method

  Finds the stream of id aUid, if it returns with KErrNone then
  pos is set to the position of the 
  if it is not found pos is set to where is should be inserted

@internalAll
@param				 aUid
@param				 &pos
*/
TInt CMsvCachedStore::FindStream(TUid aUid,TInt &pos) const
	{
	TPairedTUidHBufC8 entry;
	entry.iUid=aUid;
	TKeyArrayFix key(_FOFF(TPairedTUidHBufC8,iUid),ECmpTInt32);
	return(iStreams->FindIsq(entry,key,pos));
	}


/**

The CMsvCachedStore::DeleteStream method

  deletes the stream with this id

@return			error code KErrNone unless the stream wasn't found
@internalAll
@param				 aUid	the stream id to delete
*/
TInt CMsvCachedStore::DeleteStream(TUid aUid)
	{
	__ASSERT_ALWAYS(iReaderCount==0 && iWriterCount==0,Panic(EReadingOrWritingWhileDeleteStream));
	TInt pos;
	TInt error=FindStream(aUid,pos);
	if(error==KErrNone)
		{
		delete iStreams->At(pos).iBuf;
		iStreams->Delete(pos);
		}
	return(error);
	}


/**

The CMsvCachedStore::LoadL method

loads the streams from a file

  should this create the file if it is corupt???

@leave				leaves if we can't find the file, if we run out of memory, if the file is corrupt
@internalAll
@param				 &aFile	file to load the streams from, take ownership of the file
*/
void CMsvCachedStore::LoadL(RFile &aFile)
	{
	__ASSERT_DEBUG(iStreams->Count()==0,User::Invariant());

	// Need to cache the file size
	User::LeaveIfError(aFile.Size(iSize));

	RFileReadStream in(aFile);
	in.PushL();
	TCheckedUid check;
	in >> check;
	if(check.UidType().IsValid()==EFalse) User::Leave(KErrCorrupt);
	if(check.UidType()[0]!=KMsvEntryFile) User::Leave(KErrCorrupt);
	in >> *iStreams;
	CleanupStack::PopAndDestroy(); // close RFileReadStream in
	}



/**

The CMsvCachedStore::SaveL method

Saves the current state to iFileName

@leave				leaves if we can't replace the file, if we run out of disk space,
@internalAll
*/
void CMsvCachedStore::SaveL()
	{
	RFile tempStoreFile;
	iStoreManager.OpenTempStoreFileL(iEntryId, tempStoreFile);
	RFileWriteStream out(tempStoreFile);
	out.PushL();
	WriteToFileStreamL(out);

	//get the size of the stream, in previous call the Stream is already commited to the file, the next step simply replace the file with this temp new file so size should be the same
	//the SizeL in this case will not involve any file i/o calls.
	iSize=out.Sink()->SizeL();
	
    CleanupStack::PopAndDestroy(&out); // close RFileWriteStream out
    iStoreManager.ReplaceFileStoreL(iEntryId);

	}

void CMsvCachedStore::WriteToFileStreamL(RFileWriteStream &aOut)
	{
	TCheckedUid check(TUidType(KMsvEntryFile,KMsvEntryFile,KNullUid));
	aOut << check;
	aOut << *iStreams;
	aOut.CommitL();	
	}





EXPORT_C void RMsvReadStream::OpenL(const CMsvStore& aMsvStore, TUid aUid)
/** Prepares an existing stream with UID for reading.

After this function has been called, the stream can be read from with the 
functions provided by the RReadStream base class.

@param aMsvStore The CMsvStore the stream is in. 
@param aUid The UID of the stream to open with read access 
@leave KErrNotFound There is no stream with UID aUid 
@leave Other Standard stream leave codes. */
	{
	HBufC8 *buffer=aMsvStore.iStore->GetStreamL(aUid);
	HMsvReadBuf *tidy= new (ELeave) HMsvReadBuf(*buffer, *(aMsvStore.iStore));
	Attach(tidy);
	}


EXPORT_C void RMsvReadStream::OpenLC(const CMsvStore& aMsvStore,TUid aUid)
/** Prepares an existing stream with UID for reading.

After this function has been called, the stream can be read from with the 
functions provided by the RReadStream base class.

The object is placed on the cleanup stack. 

@param aMsvStore The CMsvStore the stream is in. 
@param aUid The UID of the stream to open with read access 
@leave KErrNotFound There is no stream with UID aUid 
@leave Other Standard stream leave codes. */
	{
	OpenL(aMsvStore,aUid);
	PushL();
	}



/**

The RMsvReadStream::OpenLC method

ties the read stream to a HMsvReadBuf


@leave				leaves if we can't find the stream
@param				 aStore   aStore is the CMsvCachedStore within which we look for the stream
@param				 aUid		id of stream to open for reading
*/
void RMsvReadStream::OpenLC(CMsvCachedStore& aStore,TUid aUid)
	{
	HBufC8 *buffer=aStore.GetStreamL(aUid);
	HMsvReadBuf *tidy= new (ELeave) HMsvReadBuf(*buffer, aStore);
	Attach(tidy);
	}



HMsvWriteBuf* HMsvWriteBuf::NewL(TUid aUid,CMsvCachedStore& aStore)
	{
	HMsvWriteBuf* self=new (ELeave) HMsvWriteBuf(aUid,aStore);
	self->PushL();
	self->iBuf=CBufSeg::NewL(KMsvFlatBufferChunkSize);
	CleanupStack::Pop(self);
	self->Set(*(self->iBuf),0);
	return(self);
	}

HMsvWriteBuf::HMsvWriteBuf(TUid aUid,CMsvCachedStore& aStore) : iBuf(NULL), iStore(&aStore), iUid(aUid)
	{
	iStore->iWriterCount++;
	}

void HMsvWriteBuf::DoRelease()
	{
	__ASSERT_DEBUG(iStore==NULL || iStore->iWriterCount>0,User::Invariant());
	delete iBuf;
	// we decrememt the writer count if it wasn't done in DoSynchL (called if client commits)
	if(iStore) iStore->iWriterCount--;
	iStore=NULL;
	delete this;
	}

void HMsvWriteBuf::DoSynchL()
	{
	// if we have already sync'd it will be null so ignore the sync
	if(iStore!=NULL)
		{
		__ASSERT_DEBUG(iStore->iWriterCount>0,User::Invariant());
		TBufBuf::DoSynchL();
		iStore->CreateOrReplaceStreamL(iUid,*iBuf);
		// decrement the writer count, after a commit all writes are ignored, this is the
		// same as other stores, also set iStore to NULL so we don't decrement the writer
		// count in DoRelease as well.
		iStore->iWriterCount--;
		iStore=NULL;
		}
	}




HMsvReadBuf::HMsvReadBuf(const TDesC8 &aBuf,CMsvCachedStore &aStore) : iCachedStore(aStore)
	{ 
	TUint8* ptr=(TUint8 *)aBuf.Ptr();
	Set(ptr,ptr+aBuf.Length(),ERead);
	aStore.iReaderCount++;
	}

void HMsvReadBuf::DoRelease()
	{
	__ASSERT_ALWAYS(iCachedStore.iReaderCount>0,User::Invariant());
	iCachedStore.iReaderCount--;
	delete this;
	}


EXPORT_C void RMsvWriteStream::AssignLC(CMsvStore &aMsvStore, TUid aUid)
/** Prepares a stream for writing; the object is placed on the cleanup stack. 

After this function has been called, the stream can be written to with the 
functions provided by the RWriteStream base class.

If the stream does not exist, it is created.

@param aMsvStore The CMsvStore the stream is in 
@param aUid The UID of the stream to open with write access 
@leave KErrAccessDenied Store is not open for writing 
@leave Other Standard stream leave codes */
	{
	if (aMsvStore.iLockStatus==CMsvStore::EMsvStoreUnlocked)
		User::Leave(KErrAccessDenied);
	AssignL(aMsvStore,aUid);
	PushL();
	}

EXPORT_C void RMsvWriteStream::AssignL(CMsvStore &aMsvStore, TUid aUid)
/** Prepares a stream for writing. 

After this function has been called, the stream can be written to with the 
functions provided by the RWriteStream base class.

If the stream does not exist, it is created.

@param aMsvStore The CMsvStore the stream is in 
@param aUid The UID of the stream to open with write access 
@leave KErrAccessDenied Store is not open for writing 
@leave Other Standard stream leave codes */
	{
	if (aMsvStore.iLockStatus==CMsvStore::EMsvStoreUnlocked)
		User::Leave(KErrAccessDenied);

	Attach(HMsvWriteBuf::NewL(aUid,*(aMsvStore.iStore)));
	}

void RMsvWriteStream::AssignLC(CMsvCachedStore &aStore, TUid aUid)
	{
	Attach(HMsvWriteBuf::NewL(aUid,aStore));
	PushL();
	}
