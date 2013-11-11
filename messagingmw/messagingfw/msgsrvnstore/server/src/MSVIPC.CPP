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

#include <s32std.h>

#include "MSVIPC.H"
#include "MSVIDS.H"
#include "MSVPANIC.H"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
#include "msvconsts.h"
#endif

GLDEF_C void DoUnpackEntry(TUint8*& aPtr, TMsvEntry& aEntry)
//
//
//
	{
	// get the entry from the start of the buffer
	const TMsvEntry* pEntry = (TMsvEntry*) aPtr;
	aEntry = *pEntry;
	aPtr = Align4(aPtr + sizeof(TMsvEntry));

	const TText* textPtr = (TText*)aPtr;
	TInt length=aEntry.iDescription.Length();
	aEntry.iDescription.Set(textPtr, length);
	textPtr = Align4(textPtr + length);

	length=aEntry.iDetails.Length();
	aEntry.iDetails.Set(textPtr, length);
	textPtr = Align4(textPtr + length);

	aPtr = (TUint8*) textPtr;
	}
	

GLDEF_C TInt DoPackEntry(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const TMsvEntry& aEntry)
//
// Packs an entry into the memory area defined by the two pointers.
// Fails with KErrOverflow if the packed entry is too large
// aPtrStart is always returned pointing to the end of the packed entry (even if too large)
//
	{
	// make sure the entry can fit into the memory area defined by the two pointers
	TInt sizeEntry = Align4(sizeof(TMsvEntry));
	TInt sizeString1 = Align4(aEntry.iDescription.Size());
	TInt sizeString2 = Align4(aEntry.iDetails.Size());
	TInt size =  sizeEntry + sizeString1 + sizeString2;
	if ((aPtrStart + size)>aPtrEnd)
		{
		aPtrStart += size;
		return KErrOverflow;
		}
		
	// copy the entry and descriptors into the memory area
	Mem::Copy((void*)aPtrStart, &aEntry, sizeof(TMsvEntry));
	aPtrStart += sizeEntry;

	Mem::Copy((void*)aPtrStart, aEntry.iDescription.Ptr(), aEntry.iDescription.Size());
	aPtrStart += sizeString1;
	
	Mem::Copy((void*)aPtrStart, aEntry.iDetails.Ptr(), aEntry.iDetails.Size());
	aPtrStart += sizeString2;
	
	return KErrNone;
	}
	
	
GLDEF_C void DoUnpackFilter(TUint8*& aPtr, CMsvEntryFilter& aFilter)
//
//
//
	{
	// get the entry from the start of the buffer
	const CMsvEntryFilter* pFilter = (CMsvEntryFilter*) aPtr;
	Mem::Copy(&aFilter, pFilter, sizeof(CMsvEntryFilter));
	aPtr = Align4(aPtr + sizeof(CMsvEntryFilter));
	}


GLDEF_C TInt DoPackFilter(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const CMsvEntryFilter& aFilter)
//
//
//
	{
	// make sure the entry can fit into the memory area defined by the two pointers
	TInt size = Align4(sizeof(CMsvEntryFilter));
	if ((aPtrStart + size)>aPtrEnd)
		{
		aPtrStart += size;
		return KErrOverflow;
		}
		
	// copy the filter into the memory area
	Mem::Copy((void*)aPtrStart, &aFilter, sizeof(CMsvEntryFilter));
	aPtrStart += size;
	
	return KErrNone;
	}

//**********************************
// TMsvPackedEntry
//**********************************


EXPORT_C TMsvPackedEntry::TMsvPackedEntry(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{}

EXPORT_C TInt TMsvPackedEntry::PackEntry(const TMsvEntry& aEntry)
//
//
//
	{
	// find the start and end of the buffer
	const TUint8* pS = iBuffer->Ptr();
	const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

	TInt error = DoPackEntry(pS, pE, aEntry);
	if (error==KErrNone)
		{
		// update the length of the buffer
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		}

	return error;
	}

EXPORT_C void TMsvPackedEntry::UnpackEntry(TMsvEntry& aEntry)
//
//
//
	{
#if defined(_DEBUG)
	// check that the buffer contain a valid package
	const TMsvEntry* dEntry = (TMsvEntry*) iBuffer->Ptr();
	const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(TMsvEntry)) + Align4(dEntry->iDescription.Size()) + Align4(dEntry->iDetails.Size()));
	__ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), PanicServer(EMsvEntryOverrunBuffer));
#endif
	
	TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
	DoUnpackEntry(pS, aEntry);
	}

TInt TMsvPackedEntry::PackEntryAndService(const TMsvEntry& aEntry, const TMsvId& aServiceId)
    {
    // find the start and end of the buffer
    const TUint8* pS = iBuffer->Ptr();
    const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

    TInt error = DoPackEntry(pS, pE, aEntry);
    if (error==KErrNone)
        {
        TInt sizeDes = Align4(sizeof(TMsvId));
        if ((pS + sizeDes)>pE)
            {
            return KErrOverflow;
            }            
        // copy the entry and descriptors into the memory area
        Mem::Copy((void*)pS, &aServiceId, sizeof(TMsvId));
        pS += sizeDes;

        // update the length of the buffer
        iBuffer->Des().SetLength(pS-iBuffer->Ptr());
        }

    return error;    
    }

void TMsvPackedEntry::UnpackEntryAndService(TMsvEntry& aEntry, TMsvId& aServiceId)
    {
#if defined(_DEBUG)
    // check that the buffer contain a valid package
    const TMsvEntry* dEntry = (TMsvEntry*) iBuffer->Ptr();
    const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(TMsvEntry)) + Align4(dEntry->iDescription.Size()) + Align4(dEntry->iDetails.Size()) + Align4(sizeof(TMsvId)) );
    __ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), PanicServer(EMsvEntryOverrunBuffer));
#endif
    
    TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
    DoUnpackEntry(pS, aEntry);

    // get the service id from the current position in the buffer
    const TMsvId* pServiceId = (TMsvId*)pS;
    aServiceId = *pServiceId;
    pS = Align4(pS + sizeof(TMsvId));
    }

//**********************************
// TMsvPackedEntryArray
//**********************************


EXPORT_C TMsvPackedEntryArray::TMsvPackedEntryArray(HBufC8*& aBuffer, TInt aCount)
: iIndex(0), iCount(aCount), iBuffer(aBuffer) 
	{
	iPos = iBuffer->Ptr();
	}


EXPORT_C void TMsvPackedEntryArray::Reset()
//
//
//
	{
	iBuffer->Des().SetLength(0);
	iPos  = iBuffer->Ptr();
	iIndex=0;
	iCount=0;
	}

EXPORT_C TInt TMsvPackedEntryArray::PackEntry(const TMsvEntry& aEntry)
//
//
//
	{
	const TUint8* pS = iPos;
	const TUint8* pE = PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxSize());

	TInt error = DoPackEntry(pS, pE, aEntry);
	if (error==KErrNone)
		{
		// update the length of the buffer and the position for the next entry
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		iPos = pS;
		iIndex++;
		iCount++;
		}
	
	return error;
	}

EXPORT_C TInt TMsvPackedEntryArray::UnpackEntry(TInt aIndex, TMsvEntry& aEntry)
//
//
//
	{
	TInt error=KErrNone;

	if (iIndex!=aIndex)
		error = FindEntryInArray(aIndex);

	if (error==KErrNone)
		{
		TUint8* pS=CONST_CAST(TUint8*, iPos);
		DoUnpackEntry(pS, aEntry);
		iPos=pS;
		iIndex++;
		}	

	return error;
	}


TInt TMsvPackedEntryArray::FindEntryInArray(TInt aIndex)
//
//
//
	{
	if (aIndex>=iCount)
		return KErrNotFound;

	if (iIndex>aIndex)
		{
		// have to reset to the start of the buffer
		iPos = iBuffer->Ptr();
		iIndex=0;
		}

	while (iIndex!=aIndex)
		{
		const TMsvEntry* pEntry = (TMsvEntry*) iPos;
		iPos += Align4(sizeof(TMsvEntry)) + Align4(pEntry->iDescription.Size()) + Align4(pEntry->iDetails.Size());
		iIndex++;
		}

	__ASSERT_DEBUG(iPos < PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), PanicServer(EMsvPointerOverrunBuffer));

	return KErrNone;
	}



//**********************************
// TMsvMoveCopyDetails
//**********************************

EXPORT_C TMsvLocalOperationProgress::TMsvLocalOperationProgress()
: iType(ELocalNone), iTotalNumberOfEntries(0), iNumberCompleted(0), iNumberFailed(0), iNumberRemaining(0), iError(KErrNone), iId(KMsvNullIndexEntryId)
/** Initialises the new object to suitable zero or null values. */
	{
	}

//**********************************
// TMsvServerOperationProgress
//**********************************

EXPORT_C TMsvServerOperationProgress::TMsvServerOperationProgress()
: iOperationType(EMsvNoOperation)
/** Default constructor.

Sets iOperationType to EMsvNoOperation. */
	{
	}

TMsvServerOperationProgress::TMsvServerOperationProgress(TMsvServerOperationType aType)
: iOperationType(aType)
	{
	}

//**********************************
// TMsvIndexProgress
//**********************************

EXPORT_C TMsvIndexProgress::TMsvIndexProgress()
: iTotal(0), iCompleted(0), iRemaining(0), iId(KMsvNullIndexEntryId)
/** Default constructor.

Data members are intialised to 0 or KMsvNullIndexEntryId as appropriate. */
	{
	}

//**********************************
// TMsvIndexLoadProgress
//**********************************

EXPORT_C TMsvIndexLoadProgress::TMsvIndexLoadProgress()
: TMsvServerOperationProgress(EMsvChangeDriveOperation), iError(KErrNone), iState(EIndexNotLoaded)
/** Default constructor.

iOperationType is set to EMsvChangeDriveOperation; iError is set to KErrNone; 
iState is set to EIndexNotLoaded. */
	{
	}
	
	
	
//**********************************
// TMsvCopyProgress
//**********************************

/** Default constructor.
iOperationType is set to EMsvCopyOperation; iError is set to KErrNone; 
iState is set to ENotYetStarted. */  

EXPORT_C TMsvCopyProgress::TMsvCopyProgress()
: TMsvServerOperationProgress(EMsvCopyOperation), iError(KErrNone), iState(ENotYetStarted)
	{
	}


//**********************************
// TMsvDeleteProgress
//**********************************

/** Default constructor.
iOperationType is set to EMsvDeleteOperation; iError is set to KErrNone; 
iState is set to ENotYetStarted. */

EXPORT_C TMsvDeleteProgress::TMsvDeleteProgress()
: TMsvServerOperationProgress(EMsvDeleteOperation), iError(KErrNone), iState(ENotYetStarted)

	{
	}


//**********************************
// TMsvChildrenDetails
//**********************************

EXPORT_C TMsvChildrenDetails::TMsvChildrenDetails()
: iParentId(KMsvNullIndexEntryId), iTotalNumberChildren(0), iNumberChildrenInArray(0), iLastEntryInArray(0)
	{
	}


//**********************************
// TMsvPackedOperation
//**********************************

EXPORT_C TMsvPackedOperation::TMsvPackedOperation(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}


EXPORT_C TInt TMsvPackedOperation::Pack(const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2)
	{
	// check the buffer is large enough
	TInt requiredSize = (aSelection.Count()+3)*4;
	if (requiredSize>iBuffer->Des().MaxSize())
		return KErrOverflow;
	// set the buffer with correct length
	iBuffer->Des().SetLength(requiredSize);

	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	*ptr++ = aSelection.Count();
	for (TInt count=0; count<aSelection.Count(); count++)
		*ptr++ = aSelection.At(count);
	*ptr++ = aParameter1;
	*ptr++ = aParameter2;
	return KErrNone;
	}


EXPORT_C void TMsvPackedOperation::UnpackL(CMsvEntrySelection& aSelection, TInt& aParameter1, TInt& aParameter2)
	{
	__ASSERT_DEBUG(aSelection.Count()==0, PanicServer(EMsvOperationUnpackSelectionNotEmpty));	
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());

	TInt count = *ptr++;
	
	TInt bufSize = iBuffer->Des().Length();
	
	// This length is calculated on the basis of length set in TMsvPackedOperation::Pack above.
	TInt len = (count+3)*4; 
	if(bufSize == len)	
		{
		while (count--)
			{
			aSelection.AppendL(*ptr++);
			}
		aParameter1 = *ptr++;
		aParameter2 = *ptr++;
		}
	else
		{
		User::Leave(KErrArgument);
		}
	}


EXPORT_C TMsvPackedChangeNotification::TMsvPackedChangeNotification(TMsvNotifBuffer& aBuffer)
: iBuffer(aBuffer)
	{
	}


EXPORT_C void TMsvPackedChangeNotification::Pack(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2, TInt aStartIndex, TInt aFinishIndex)
//
// Packs the aStartIndex->aFinishIndex (inc) into the buffer
//
	{
	__ASSERT_DEBUG(aFinishIndex-aStartIndex+1<=KMsvPackedChangeLimit, PanicServer(EMsvChangeSelectionTooLarge));

	// set the buffer with correct length
	TInt requiredSize = (aFinishIndex-aStartIndex+1+KMsvChangeNotificationNumberOfTInts)*4;
	iBuffer.SetLength(requiredSize);

	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer.Ptr());
	*ptr++ = aChangeType;
	*ptr++ = aParameter1;
	*ptr++ = aParameter2;
	*ptr++ = aFinishIndex - aStartIndex + 1;
	for (TInt count=aStartIndex; count<=aFinishIndex; count++)
		*ptr++ = aSelection.At(count);
	}


EXPORT_C void TMsvPackedChangeNotification::Pack(TMsvServerChangeNotificationType aChangeType, TMsvId aId, TInt aParameter1, TInt aParameter2)
//
// Packs a single id
//
	{
	// set the buffer with correct length
	TInt requiredSize = (1+KMsvChangeNotificationNumberOfTInts)*4;
	iBuffer.SetLength(requiredSize);

	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer.Ptr());
	*ptr++ = aChangeType;
	*ptr++ = aParameter1;
	*ptr++ = aParameter2;
	*ptr++ = 1;
	*ptr++ = aId;
	}


EXPORT_C void TMsvPackedChangeNotification::UnpackL(TMsvServerChangeNotificationType& aChangeType, CMsvEntrySelection& aSelection, TInt& aParameter1, TInt& aParameter2)
//
//
//
	{
	__ASSERT_DEBUG(aSelection.Count()==0, PanicServer(EMsvChangedUnpackSelectionNotEmpty));	
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer.Ptr());
	switch (*ptr++)
		{
		case 1:
			aChangeType = EMsvEntriesCreated;
			break;
		case 2:
			aChangeType = EMsvEntriesChanged;
			break;
		case 3:
			aChangeType = EMsvEntriesDeleted;
			break;
		case 4:
			aChangeType = EMsvEntriesMoved;
			break;
		case 5:
			aChangeType = EMsvMtmGroupInstalled;
			break;
		case 6:
			aChangeType = EMsvMtmGroupDeInstalled;
			break;
		case 8:
			aChangeType = EMsvCloseSession;
			break;
		case 9:
			aChangeType = EMsvIndexLoaded;
			break;
		case 10:
			aChangeType = EMsvIndexFailedToLoad;
			break;
		case 12:
			aChangeType = EMsvMediaChanged;
			break;
		case 13:
			aChangeType = EMsvMediaUnavailable;
			break;
		case 14:
			aChangeType = EMsvMediaAvailable;
			break;
		case 15:
			aChangeType = EMsvMediaIncorrect;
			break;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		case 16:
			aChangeType = EMsvMessageStoreNotSupported;
			break;
		case 17:
			aChangeType = EMsvMessageStoreCorrupt;
			break;
		case 18:
			aChangeType = EMsvRefreshMessageView;
			break;
		case 19:
			aChangeType = EMsvDiskNotAvailable;
			break;
		case 20:
			aChangeType = EMsvUnableToProcessDiskNotification;
			break;
#endif	
		default:
			__ASSERT_DEBUG(EFalse, PanicServer(EMsvUnknownChangeType));
			aChangeType = EMsvEntriesNoChange;
		}
	aParameter1 = *ptr++;
	aParameter2 = *ptr++;
	TInt count = *ptr++;
	while (count--)
		aSelection.AppendL(*ptr++);
	}

TMsvPackedEntryFilter::TMsvPackedEntryFilter(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{}

TInt TMsvPackedEntryFilter::PackFilter(const CMsvEntryFilter& aFilter)
	{
	// find the start and end of the buffer
	const TUint8* pS = iBuffer->Ptr();
	const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

	TInt error = DoPackFilter(pS, pE, aFilter);
	if (error==KErrNone)
		{
		// update the length of the buffer
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		}

	return error;
	}

void TMsvPackedEntryFilter::UnpackFilter(CMsvEntryFilter& aFilter)
	{
#if defined(_DEBUG)
	// check that the buffer contain a valid package
	const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(CMsvEntryFilter)));
	__ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), PanicServer(EMsvEntryOverrunBuffer));
#endif	
	TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
	DoUnpackFilter(pS, aFilter);
	}

/**
Constructor for TMsvPackQuery.
@internalComponent
@released
@param aBuffer: buffer for packing
*/

EXPORT_C TMsvPackQuery::TMsvPackQuery(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{}


TInt TMsvPackQuery::DoPackQuery(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const CMsvSearchSortQuery* aQuery)
	{
	//size of class - sizeof(iQueryTable pointer)
	TInt sizeOfClass = Align4(sizeof(CMsvSearchSortQuery) - sizeof(TInt));
	TInt sizeQueryTable = Align4(sizeof(TMsvQueryTable) * KMaxLevelOfSearchAndSort);
	TInt size = sizeOfClass + sizeQueryTable;
	
	if ((aPtrStart + size)>aPtrEnd)
		{
		aPtrStart += size;
		return KErrOverflow;
		}
		
	// copy the entry and descriptors into the memory area
	Mem::Copy((void*)aPtrStart, aQuery, sizeOfClass);
	aPtrStart += sizeOfClass;

	Mem::Copy((void*)aPtrStart, aQuery->iQueryTable, sizeQueryTable);
	aPtrStart += sizeQueryTable;

	return KErrNone;
	}

/**
Packs TMsvSearchSortQuery object into a buffer for sending across IPC. 
@internalComponent
@released
@param aQuery: TMsvSearchSortQuery object needs to pack.
@return: reurn KErrNone if successful else KErrOverflow.
*/
EXPORT_C TInt TMsvPackQuery::PackQuery(const CMsvSearchSortQuery* aQuery)
	{
	// find the start and end of the buffer
	const TUint8* pS = iBuffer->Ptr();
	const TUint8* pE = PtrAdd(pS, iBuffer->Des().MaxSize());

	TInt error = DoPackQuery(pS, pE, aQuery);
	if (error==KErrNone)
		{
		// update the length of the buffer
		iBuffer->Des().SetLength(pS-iBuffer->Ptr());
		}
	return error;
	}
	

void TMsvPackQuery::DoUnpackQuery(TUint8*& aPtr, CMsvSearchSortQuery* aQuery)
	{
	//size of class - sizeof(iQueryTable pointer)
	TInt sizeOfClass = Align4(sizeof(CMsvSearchSortQuery) - sizeof(TInt));
	TInt sizeQueryTable = Align4(sizeof(TMsvQueryTable) * KMaxLevelOfSearchAndSort);
	
	Mem::Copy((void*)aQuery, aPtr, sizeOfClass);
	aPtr += sizeOfClass;
	
	Mem::Copy((void*)aQuery->iQueryTable, aPtr, sizeQueryTable);
	aPtr += sizeQueryTable;
	}	

/**
Unpacks the data to a aQuery.
@internalComponent
@released
@param aQuery: Unpacked TMsvSearchSortQuery object.
*/
EXPORT_C void TMsvPackQuery::UnpackQuery(CMsvSearchSortQuery* aQuery)
	{
#if defined(_DEBUG)
	// check that the buffer contain a valid package
	const TUint8* dPos = PtrAdd(iBuffer->Ptr(), Align4(sizeof(CMsvSearchSortQuery)));
	__ASSERT_DEBUG(dPos <= PtrAdd(iBuffer->Ptr(), iBuffer->Des().MaxLength()), PanicServer(EMsvEntryOverrunBuffer));
#endif
	
	TUint8* pS = CONST_CAST(TUint8*, iBuffer->Ptr());
	DoUnpackQuery(pS, aQuery);
	}

/**
Constructor for TMsvPackedIdOperation.
@internalComponent
@released
@param aBuffer: buffer for packing
*/
EXPORT_C TMsvPackedIdOperation::TMsvPackedIdOperation(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}

/**
Packs RArray of TMsvId and count value into a buffer for sending across IPC. 
@internalComponent
@released
@param aId: RArray of TMsvIds needs to pack.
@return: reurn KErrNone if successful else KErrOverflow.
*/
EXPORT_C TInt TMsvPackedIdOperation::Pack(const RArray<TMsvId>& aId)
	{
	//place for TMsvId's and count
	TInt requiredSize = (aId.Count() + 1) * 4;
	
	// check the buffer is large enough
	if (requiredSize > iBuffer->Des().MaxSize())
		{
		return KErrOverflow;
		}
		
	// set the buffer with correct length
	iBuffer->Des().SetLength(requiredSize);
	
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	//number of TMsvId's
	*ptr++ = aId.Count();
	
	//copy TMsvId's to buffer 	
	for (TInt count=0; count<aId.Count(); count++)
		{
		*ptr++ = aId[count];
		}
		
	return KErrNone;
	}

/**
Unpacks the data in buffer to a RArray.
@internalComponent
@released
@param aId: Unpacked RArray of TMsvIds.
*/
EXPORT_C void TMsvPackedIdOperation::UnpackL(RArray<TMsvId>& aId)
	{
	__ASSERT_DEBUG(aId.Count()==0, PanicServer(EMsvOperationUnpackSelectionNotEmpty));	
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());

	//unpack the count
	TInt count = *ptr++;
	//unpack TMsvId's to RArray
	while (count--)
		{
		aId.AppendL(*ptr++);
		}
	}


/**
Constructor for TMsvPackedIdAndMessagePart.
@internalComponent
@released
*/
EXPORT_C TMsvPackedIdAndMessagePart::TMsvPackedIdAndMessagePart()
	{
	}

/**
Size() will returns size of the data occupied by TMsvIdWithSortField.
@internalComponent
@released
@param aData: RArray of TMsvIdWithSortField.
*/	
EXPORT_C TInt TMsvPackedIdAndMessagePart::Size(const RArray<TMsvIdWithSortField>& aData)
	{
	// number of TMsvId's in RArray
	TInt count = aData.Count();
	TInt size = 0;
	
	// to hold counter value
	size += sizeof(TInt);
	
	// calculate length of RArray<TMsvId + SortFiled>
	for(TInt index = 0; index < count; index++)
		{
		size += sizeof(TMsvId);
		size += aData[index].iContentMessagePart.Length();
		}
	return size;
	}

/**
Packs or Externalize the RArray of TMsvIdWithSortField into a aWriteStream for sending across IPC. 
@internalComponent
@released
@param aWriteStream: aData will be written to aWriteStream.
@param aData: RArray of TMsvIdWithSortField
*/	
EXPORT_C void TMsvPackedIdAndMessagePart::ExternalizeL(RWriteStream& aWriteStream, RArray<TMsvIdWithSortField>& aData) const
	{
	TInt count = aData.Count();
	aWriteStream.WriteInt32L(count);
  	
  	for(TInt index=0; index < count; ++index)
 		{
 		aWriteStream.WriteInt32L(aData[index].iMessageId);
 		aWriteStream << aData[index].iContentMessagePart;
 		}
	}

/**
Unpacks or Internalize aReadStream buffer to RArray of TMsvIdWithSortField.
@internalComponent
@released
@param aReadStream: aReadStream data will be unpacked to TMsvIdWithSortField of aData.
@param aData: RArray of TMsvIdWithSortField
*/	
EXPORT_C void TMsvPackedIdAndMessagePart::InternalizeL(RReadStream& aReadStream, RArray<TMsvIdWithSortField>& aData)
	{
	TInt count =  aReadStream.ReadInt32L();
	
	TMsvIdWithSortField tmsvidAndmessagepart;
	
	for(TInt index=0; index < count; ++index)
 		{
 		tmsvidAndmessagepart.iMessageId = aReadStream.ReadInt32L();
 		aReadStream >> tmsvidAndmessagepart.iContentMessagePart;
 		aData.AppendL(tmsvidAndmessagepart);
 		}
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
EXPORT_C TMsvPackedDriveIdOperation::TMsvPackedDriveIdOperation(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}
	
	
EXPORT_C void TMsvPackedDriveIdOperation::UnpackL(RArray<TDriveNumber>& aDriveNumber)
	{
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());

	// Unpack the count.
	TInt count = *ptr++;
	
	// Append TDriveNumber
	while (count--)
		{
		aDriveNumber.AppendL((TDriveNumber)*ptr++);
		}	
	}
	
	
EXPORT_C TInt TMsvPackedDriveIdOperation::Pack(const RArray<TDriveNumber>& aDriveNumber)
	{
	// place for TMsvId's and count
	TInt requiredSize = (aDriveNumber.Count() + 1) * 4;
	
	// check the buffer is large enough
	if (requiredSize > iBuffer->Des().MaxSize())
		{
		return KErrOverflow;
		}
		
	// set the buffer with correct length
	iBuffer->Des().SetLength(requiredSize);
	
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	//number of TMsvId's
	*ptr++ = aDriveNumber.Count();
	
	//copy TMsvId's to buffer 	
	for (TInt count=0; count<aDriveNumber.Count(); count++)
		{
		*ptr++ = aDriveNumber[count];
		}
		
	return KErrNone;
	}

#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)





#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
 * TMsvPackedHeaderStructure()
 * 
 * TMsvPackedHeaderStructure Constructor.
 *
 * @param HBufC8*&
 * @return None.
 * @leave None.
 */	
EXPORT_C TMsvPackedHeaderStructure::TMsvPackedHeaderStructure(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}
	

/**
 * UnpackL()
 * 
 * Unpacks the buffer to fill header structure.
 *
 * @param RPointerArray<CFieldPair>&: Header Structure.
 * @return None.
 * @leave KErrNoMemory
 */	
EXPORT_C void TMsvPackedHeaderStructure::UnpackL(RPointerArray<CFieldPair>& aFieldDetails)
	{
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());

	// Unpack the count.
	TInt count = *ptr++;

	TInt size = 0;
	TPtrC16 ptrBuf;	

	while (count--)
		{
		CFieldPair* fieldPair = new(ELeave) CFieldPair();
		CleanupStack::PushL(fieldPair);
		size = *ptr++;
		
		const TText* textPtr = (TText*)ptr;
		ptrBuf.Set(textPtr, (size/2));			// ptrBuf is 16 bits.
		ptr += Align4(size)/sizeof(TInt);
		
		fieldPair->iFieldName = ptrBuf.AllocL();
		fieldPair->iFieldType = (EFieldType) *ptr++;
		aFieldDetails.AppendL(fieldPair);
		CleanupStack::Pop();			// fieldPair
		}	
	}
	


/**
 * Pack()
 * 
 * Packs the header structure to a buffer.
 *
 * @param RPointerArray<CFieldPair>&: Header Structure.
 * @return TInt: KErrOverflow, if buffer does not have sufficient memory.
 */		
EXPORT_C TInt TMsvPackedHeaderStructure::Pack(const RPointerArray<CFieldPair>& aFieldDetails)
	{
	TInt count = aFieldDetails.Count();
	TInt requiredSize = 0;
	
	// Calculate the size of the data to be written.
	for(TInt index=0; index<count; index++)
		{
		CFieldPair* fieldPair = aFieldDetails[index]; 	
		requiredSize += Align4(fieldPair->iFieldName->Des().Size());
		requiredSize += 8;		// 4 bytes for EFieldType and 4 bytes for size of iFieldName
		}
	
	// 4 bytes are needed to store the array count.
	requiredSize += 4;
	if(requiredSize > iBuffer->Des().MaxSize())
		{
		return KErrOverflow;
		}

	// Set the buffer with correct length
	iBuffer->Des().SetLength(requiredSize);
		
	// Start writing to the buffer.
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	*ptr++ = count;
	
	for(TInt index=0; index<count; index++)
		{
		CFieldPair* fieldPair = aFieldDetails[index];
		TInt size = fieldPair->iFieldName->Des().Size();
		
		*ptr++ = size;
		Mem::Copy((void*)ptr, fieldPair->iFieldName->Des().Ptr(), size);
		ptr += (Align4(size))/sizeof(TInt);
		*ptr++ = (TInt)(fieldPair->iFieldType);
		}

	// place for TMsvId's and count
	return KErrNone;
	}




/**
 * TMsvPackedHeaderData()
 * 
 * TMsvPackedHeaderData Constructor.
 *
 * @param HBufC8*&
 * @return None.
 * @leave None.
 */	
EXPORT_C TMsvPackedHeaderData::TMsvPackedHeaderData(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}
	

/**
 * UnpackL()
 * 
 * Unpacks the buffer to fill header data structure.
 *
 * @param RPointerArray<CHeaderFields>&: Header Structure.
 * @return None.
 * @leave KErrNoMemory
 */		
EXPORT_C void TMsvPackedHeaderData::UnpackL(RPointerArray<CHeaderFields>& aFieldDetails)
	{
	aFieldDetails.ResetAndDestroy();
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());

	// Unpack the count.
	TInt count = *ptr++;

	TPtrC16 ptrBuf;
	while (count--)
		{
		CHeaderFields* headerRow = new(ELeave) CHeaderFields();
		CleanupStack::PushL(headerRow);
		
		headerRow->iUid = TUid::Uid(*ptr++);
		TInt colCount = *ptr++;
		while(colCount--)
			{
			CFieldPair* fieldObj = new(ELeave) CFieldPair();
			CleanupStack::PushL(fieldObj);
			
			if(EIntegerField == (EFieldType)*ptr++)
				{
				TUint32 rVal = *ptr++;
				fieldObj->iFieldNumValue = rVal;
				TUint64 lVal = (*ptr++);
				lVal = lVal << 32;
				fieldObj->iFieldNumValue |= lVal;
				}
			else
				{
				TInt size = *ptr++;
				const TText* textPtr = (TText*)ptr;
				ptrBuf.Set(textPtr, (size/2));
				ptr += Align4(size)/sizeof(TInt);
				
				fieldObj->iFieldTextValue = ptrBuf.AllocL();
				}

			headerRow->iFieldPairList.AppendL(fieldObj);
			CleanupStack::Pop(fieldObj);
			}
		
		aFieldDetails.AppendL(headerRow);
		CleanupStack::Pop(headerRow);
		}	
	}
	



/**
 * Pack()
 * 
 * Packs the header data to a buffer.
 *
 * @param RPointerArray<CHeaderFields>&: Header Data.
 * @return TInt: KErrOverflow, if buffer does not have sufficient memory.
 */		
EXPORT_C TInt TMsvPackedHeaderData::Pack(const RPointerArray<CHeaderFields>& aFieldDetails)
	{
	TInt count = aFieldDetails.Count();
	TInt requiredSize = 0;
	
	// Calculate the size of the data to be written.
	for(TInt headerRow=0; headerRow<count; headerRow++)
		{
		RPointerArray<CFieldPair>& fieldPairList = aFieldDetails[headerRow]->iFieldPairList;
		for(TInt fieldIndex=0; fieldIndex<fieldPairList.Count(); fieldIndex++)	
			{
			requiredSize += 4;			// Store the data type.
			// If it is a text field
			if(fieldPairList[fieldIndex]->iFieldTextValue)
				{
				requiredSize += 4; 		// 4 bytes to store size of iFieldTextValue
				requiredSize += Align4(fieldPairList[fieldIndex]->iFieldTextValue->Des().Size());				
				}
			else	// For Int or date field.
				{
				requiredSize += sizeof(TInt64);
				}				
			}
		requiredSize += 8;		// 4 bytes for TUid, 4 bytes for size of iFieldPairList
		}
	requiredSize += 4; 			// 4 bytes are needed to store the array count. (count)
	
	if(requiredSize > iBuffer->Des().MaxSize())
		{
		return KErrOverflow;
		}

	// Set the buffer with correct length
	iBuffer->Des().SetLength(requiredSize);
	
	// Start writing to the buffer.
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	*ptr++ = count;
	
	for(TInt headerRow=0; headerRow<count; headerRow++)
		{
		// For each header row...
		
		// Store UID of the row.
		*ptr++ = aFieldDetails[headerRow]->iUid.iUid;
		
		// Store field list.
		RPointerArray<CFieldPair>& fieldPairList = aFieldDetails[headerRow]->iFieldPairList;
		*ptr++ = fieldPairList.Count();
		
		for(TInt fieldIndex=0; fieldIndex<fieldPairList.Count(); fieldIndex++)
			{
			CFieldPair* fieldPair = fieldPairList[fieldIndex];
			if(fieldPair->iFieldTextValue)
				{
				// Data type
				*ptr++ = ETextField;				
				// Data Size
				TInt textSize = fieldPair->iFieldTextValue->Des().Size();
				*ptr++ = textSize;
				// Data
				Mem::Copy((void*)ptr, fieldPair->iFieldTextValue->Des().Ptr(), textSize);
				ptr += Align4(textSize)/sizeof(TInt);
				}
			else
				{
				// Data type
				*ptr++ = EIntegerField;
				// Data (64 bits)
				TUint64 maskVal = KMaxTUint32;
				*ptr++ = (fieldPair->iFieldNumValue) & maskVal;
				maskVal = ~maskVal;
				*ptr++ = (fieldPair->iFieldNumValue & maskVal) >> 32;
				}
			}
		}
		
	return KErrNone;
	}
	


#endif		// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)




