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

#include "MSVSTD.H"
#include "MSVRCPT.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVPANIC.H"

const TInt KMessageSelectionGranularity=32;

_LIT(KDefaultSubjectSkipString,": ");

//**********************************
// TMsvEntry
//**********************************

EXPORT_C TMsvEntry::TMsvEntry()
: iId(KMsvNullIndexEntryId), iParentId(KMsvNullIndexEntryId), iData(KMsvEntryMediumPriority), 
iPcSyncCount(0),  iReserved(0),
iServiceId(KMsvNullIndexEntryId), iRelatedId(KMsvNullIndexEntryId), 
iType(KUidMsvNullEntry), iMtm(KUidMsvNullEntry), 
iSize(0), iError(KErrNone), iBioType(0),
iMtmData1(0), iMtmData2(0), iMtmData3(0)
/** Default constructor.

Initialises the new object to null values: 0 for integer values, KMsvNullIndexEntryId 
for IDs, and KUidMsvNullEntry for UIDs. */
	{
	}

EXPORT_C TMsvEntry::TMsvEntry(const TMsvEntry& aEntry)
/** Copy constructor.

It creates a simple copy of the entry, so the TPtrC members iDescription and iDetails 
will point to the same descriptor data in the original and new objects.

@param aEntry The entry to copy */
	{
	Copy(aEntry);
	}

EXPORT_C TMsvEntry& TMsvEntry::operator=(const TMsvEntry& aEntry)
/** Overloaded assignment operator.

This acts similarly to the copy constructor: i.e. it makes a simple copy of entry, 
so the TPtrC entry description and details members will point to the same 
descriptor data in the original and new objects.

@param aEntry The entry from which to assign 
@return Object after assignment */
	{
	Copy(aEntry);
	return *this;
	}

EXPORT_C TBool TMsvEntry::operator==(const TMsvEntry& aEntry) const
/** Overloaded equality operator.

This tests whether the fields of the specified entry are equal to those of this 
object. The contents, rather than pointer values, of the TPtrC members are 
compared.

@param aEntry The entry with which to compare 
@return ETrue if the entries are equal, else EFalse */
	{
	TInt size=sizeof(*this)-2*sizeof(TPtrC);
	TBool equal = !(Mem::Compare((TUint8*)&aEntry, size, (TUint8*)this, size));
	equal &= (aEntry.iDescription==this->iDescription);
	equal &= (aEntry.iDetails==this->iDetails);
	return equal;
	}


void TMsvEntry::Copy(const TMsvEntry& aEntry)
	{
	Mem::Copy(this,&aEntry,sizeof(*this));
	}


EXPORT_C TMsvPriority TMsvEntry::Priority() const
/** Gets the value of the message priority field.

@return The value of the message priority field */
	{
	if (iData&KMsvEntryHighPriority)
		return EMsvHighPriority;
	else if (iData&KMsvEntryMediumPriority)
		return EMsvMediumPriority;
	else
		return EMsvLowPriority;
	}

EXPORT_C void TMsvEntry::SetPriority(TMsvPriority aPriority)
/** Sets the message priority.

@param aPriority Value for the message priority */
	{
	switch (aPriority)
		{
	case EMsvHighPriority:
		iData = (iData & ~KMsvEntryPriorityMask) | KMsvEntryHighPriority;
		break;
	case EMsvMediumPriority:
		iData = (iData & ~KMsvEntryPriorityMask) | KMsvEntryMediumPriority;		
		break;
	default:
		iData = (iData & ~KMsvEntryPriorityMask) | KMsvEntryLowPriority;		
		}
	}


EXPORT_C void TMsvEntry::InternalizeL(RReadStream& aStream)
/** Reads an externalised object from the specified stream.

@param aStream Stream from which to read */
	{
	iId			= aStream.ReadInt32L();
	iParentId	= aStream.ReadInt32L();
	iData		= aStream.ReadInt32L();
	iPcSyncCount= aStream.ReadInt32L();
	iReserved	= aStream.ReadInt32L();
	//
	iServiceId	= aStream.ReadInt32L();
	iRelatedId	= aStream.ReadInt32L();
	aStream >> iType;
	aStream >> iMtm;
	//
	TInt64 time; 
	aStream >> time;
	iDate = time;
	//
	iSize	= aStream.ReadInt32L();
	iError	= aStream.ReadInt32L();
	iBioType = aStream.ReadInt32L();
	//
	iMtmData1 = aStream.ReadInt32L();
	iMtmData2 = aStream.ReadInt32L();
	iMtmData3 = aStream.ReadInt32L();
	}



EXPORT_C void TMsvEntry::ExternalizeL(RWriteStream& aStream) const
/** Writes the object into the stream specified.

@param aStream Stream to which to write */
	{
	aStream.WriteUint32L(iId);
	aStream.WriteUint32L(iParentId);
	aStream.WriteUint32L(iData & KMsvEntryPersistedFlags);
	aStream.WriteUint32L(iPcSyncCount);
	aStream.WriteUint32L(iReserved);
	//
	aStream.WriteUint32L(iServiceId);
	aStream.WriteUint32L(iRelatedId);
	aStream << iType;
	aStream << iMtm;
	//
	aStream << iDate.Int64();
	//
	aStream.WriteUint32L(iSize);
	aStream.WriteUint32L(iError);
	aStream.WriteUint32L(iBioType);
	//
	aStream.WriteUint32L(iMtmData1);
	aStream.WriteUint32L(iMtmData2);
	aStream.WriteUint32L(iMtmData3);
	}


/** Compares if the flag settings in two index entries are the same. The  
Connected and Pending Delete flags are not used in the comparison, as they 
indicate temporary conditions of the entry.

@param aEntry Index entry to compare against
@return True if the flags settings are equal, otherwise false
*/
EXPORT_C TBool TMsvEntry::PermanentDataUnchanged(const TMsvEntry& aEntry) const
//
// Returns true if the permanent data is equal
//
	{
	TMsvEntry self=*this;
	TMsvEntry that=aEntry;
	self.iData &= KMsvEntryPersistedFlags;
	that.iData &= KMsvEntryPersistedFlags;
	return self==that;
	}
	
/** Gets the pending conditions flag.

@return 
The value for the pending conditions flag. 
*/
EXPORT_C TBool TMsvEntry::PendingConditions() const
	{
	return iData & KMsvEntryPendingConditionsFlag;
	}
   
/** Sets the pending conditions flag.

@param	aScheduled 
The value for the pending conditions flag.
*/
EXPORT_C void TMsvEntry::SetPendingConditions(TBool aPendingConditions)
	{
	iData = (iData & ~KMsvEntryPendingConditionsFlag) | (aPendingConditions ? KMsvEntryPendingConditionsFlag : KMsvEntryClearFlag);
	}

/** Gets the integer value of iMtmData1

@return the integer value of iMtmData1
*/
EXPORT_C TInt32 TMsvEntry::MtmData1() const
	{
	return iMtmData1;
	}

/** Gets the integer value of iMtmData2

@return the integer value of iMtmData2
*/	
EXPORT_C TInt32 TMsvEntry::MtmData2() const
	{
	return iMtmData2;
	}

/** Gets the integer value of iMtmData3

@return the integer value of iMtmData3
*/	
EXPORT_C TInt32 TMsvEntry::MtmData3() const
	{
	return iMtmData3;
	}

/** Sets the value of iMtmData1. This API should only be called
by Symbian Software Ltd.

@param aMtmData Value of the data to be stored in iMtmData1

@internalTechnology
*/	
EXPORT_C void TMsvEntry::SetMtmData1(TInt32 aMtmData)
	{
	iMtmData1 = aMtmData;
	}

/** Sets the value of iMtmData2. This API should only be called
by Symbian Software Ltd.

@param aMtmData Value of the data to be stored in iMtmData2

@internalTechnology
*/	
EXPORT_C void TMsvEntry::SetMtmData2(TInt32 aMtmData)
	{
	iMtmData2 = aMtmData;
	}

/** Sets the value of iMtmData3. This API should only be called
by Symbian Software Ltd.

@param aMtmData Value of the data to be stored in iMtmData3

@internalTechnology
*/	
EXPORT_C void TMsvEntry::SetMtmData3(TInt32 aMtmData)
	{
	iMtmData3 = aMtmData;
	}

//**********************************
// CMsvEntrySelection
//**********************************

EXPORT_C CMsvEntrySelection::CMsvEntrySelection()
	: CArrayFixFlat<TMsvId>(KMessageSelectionGranularity)
/** Creates an empty array. */
	{
	}


EXPORT_C CMsvEntrySelection* CMsvEntrySelection::CopyL() const
/** Creates a new CMsvEntrySelection object with the same contents as the current 
object. The client should delete the object when it is no longer required.

@leave KErrNoMemory A memory allocation failed. 
@return New CMsvEntrySelection with same selection as current object */
	{
	CMsvEntrySelection* newSelection = CopyLC();
	CleanupStack::Pop();
	return newSelection;
	}

EXPORT_C CMsvEntrySelection* CMsvEntrySelection::CopyLC() const
/** Creates a new CMsvEntrySelection object with the same contents as the current 
object. The client should delete the object when it is no longer required.

The function leaves the new object on the cleanup stack.

@leave KErrNoMemory A memory allocation failed. 
@return New CMsvEntrySelection with same selection as current object */
	{
	CMsvEntrySelection* newSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(newSelection);
	if (Count())
		{
		newSelection->ResizeL(Count(), KMsvNullIndexEntryId);
		Mem::Copy(newSelection->Back(0), Back(0), Count()*sizeof(TMsvId));	
		}
	return newSelection;
	}


EXPORT_C TInt CMsvEntrySelection::Find(TMsvId aId) const
/** Finds an item in the array by its entry ID.

@param aId ID to find 
@return The index of the position of the entry in the array, or KErrNotFound 
if the selection does not contain the ID. */
	{
	TInt count=Count();
	if (count)
		{
		const TMsvId* ptr = End(0);
		while (count--)
			if (*(--ptr)==aId)
				break;
		return count;
		}
	else
		return KErrNotFound;
	}


//**********************************
// TMsvSelectionOrdering
//**********************************

EXPORT_C TMsvSelectionOrdering::TMsvSelectionOrdering()
:iGrouping(0), iSortType(EMsvSortByNone)
,iSubjectSkipString(KDefaultSubjectSkipString)
/** Default constructor.

Creates a new object with no grouping, and sets the sorting to EMsvSortByNone. */
	{}

EXPORT_C TMsvSelectionOrdering::TMsvSelectionOrdering(TInt aGroupingKey, TMsvSorting aSorting, TBool aShowInvisible)
: iGrouping(aGroupingKey), iSortType(aSorting)
,iSubjectSkipString(KDefaultSubjectSkipString)

/** Creates a new object, sets the group, sort order, and show-invisibility options 
to those specified in the parameters.

@param aGroupingKey A bitmask of grouping options. For the values to use, 
see TMsvGrouping. 
@param aSorting Sorting order. 
@param aShowInvisible Determines whether invisible entries are included in 
the selection */
	{
	SetShowInvisibleEntries(aShowInvisible);
	}

EXPORT_C void TMsvSelectionOrdering::InternalizeL(RReadStream& aStream)
/** Reads an externalised object from the specified stream. 

@param aStream Stream from which to read */
	{
	iGrouping = aStream.ReadInt32L();
	iSortType = (TMsvSorting) aStream.ReadInt32L();
	if (iSortType<EMsvSortByNone && iSortType>EMsvSortByDescriptionReverse)
		iSortType=EMsvSortByNone;
	aStream >> iSubjectSkipString;
	}

EXPORT_C void TMsvSelectionOrdering::ExternalizeL(RWriteStream& aStream) const
/** Writes the object into the specified stream. 

@param aStream Stream to which to write */
	{
	aStream.WriteInt32L(iGrouping);
	aStream.WriteInt32L((TInt)iSortType);
	aStream << iSubjectSkipString;
	}

EXPORT_C TBool TMsvSelectionOrdering::operator==(const TMsvSelectionOrdering& aOrdering) const
	/** Tests two TMsvSelectionOrdering objects for equality. 
	
	Two TMsvSelectionOrdering objects are equal if the same grouping and sorting 
	options have been set on each.
	
	@param aOrdering The object with which to compare 
	@return ETrue if the objects are equal, else EFalse */
	{
	return (aOrdering.iGrouping==iGrouping && aOrdering.iSortType==iSortType && iSubjectSkipString == aOrdering.iSubjectSkipString);
//	TInt size=sizeof(*this);
//	return !(Mem::Compare((TUint8*)&aOrdering, size, (TUint8*)this, size));
	}
	
EXPORT_C const TDesC& TMsvSelectionOrdering::SubjectSkipString() const
/** Get the current subject skip string

@return The current subject skip string*/
	{
	return iSubjectSkipString;
	}

EXPORT_C void TMsvSelectionOrdering::SetSubjectSkipString(const TDesC& aSubjectSkipString)
/** Set the subject skip string up to maximum length of KMaxSubjectSkipStringLength
	
@panic	USER 11, if the length of the descriptor aSubjectSkipString is greater than
		KMaxSubjectSkipStringLength (defined in <msvstd.h>) */
	{
	//Check whether SubjectSkipString is empty. If so set it to default
	if(aSubjectSkipString.Length() == 0)
		{
		iSubjectSkipString = KDefaultSubjectSkipString;	
		}
	else
		{
		iSubjectSkipString = aSubjectSkipString;		
		}	
	
	}

//**********************************
// CMsvRecipient
//**********************************


EXPORT_C CMsvRecipient::CMsvRecipient()
/** Protected base class default constructor. */
	{}

EXPORT_C void CMsvRecipient::InternalizeL(RReadStream& aStream)
	/** Internalises the object from a read stream.
	
	@param aStream The stream from which the object should be internalised. */
	{
	iStatus = (TRecipientStatus)aStream.ReadInt32L();
	iError = aStream.ReadInt32L();
	iRetries = aStream.ReadInt32L();
	TInt64 time; 
	aStream >> time;
	iTime = time;
	}

EXPORT_C void CMsvRecipient::ExternalizeL(RWriteStream& aStream) const
	/** Externalises the object to a write stream.
	
	@param aStream Stream to which the object should be externalised. */
	{
	aStream.WriteInt32L((TInt)iStatus);
	aStream.WriteInt32L(iError);
	aStream.WriteInt32L(iRetries);
	aStream << iTime.Int64();
	}

//**********************************
// CMsvEntryFilter
//**********************************

EXPORT_C CMsvEntryFilter* CMsvEntryFilter::NewLC()
/** Allocates and constructs a new entry filter, and leaves the pointer on the 
cleanup stack.

For default values, see the NewL() function.

@return Pointer to the newly created entry filter. */
	{
	CMsvEntryFilter* self = new(ELeave)CMsvEntryFilter();
	CleanupStack::PushL(self);
	return self;
	}

EXPORT_C CMsvEntryFilter* CMsvEntryFilter::NewL()
/** Allocates and constructs a new entry filter.

Default values are as follows:

1. service: KMsvNullIndexEntryId

2. MTM: KNullUid

3. entry type: KNullUid

4. last change date: no date

5. sort order defaults to EMsvSortByNone

These values mean that entries with any values for service, MTM, type and 
last change date will be returned, with no grouping or sort MTM.

@return Pointer to the newly created entry filter. */
	{
	CMsvEntryFilter* self = NewLC();
	CleanupStack::Pop(); // self
	return self;
	}

CMsvEntryFilter::CMsvEntryFilter()
: iServiceId(KMsvNullIndexEntryId), iMtm(KNullUid), iType(KNullUid), iSortMtm(KNullUid)
	{
	}

//
// DLL entry point

