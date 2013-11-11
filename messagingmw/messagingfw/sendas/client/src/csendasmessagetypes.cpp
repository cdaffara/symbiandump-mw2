// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32strm.h>
#include <csendasmessagetypes.h>

const TInt KMessageTypeNameArrayGranularity = 8;
const TInt16 KStreamVersionSendAs2MessageType = 1;
const TInt KMaxSendAs2MessageTypeNameLength = 1024;


/**
Create a new CSendAsMessageTypes.

@return
If the function succeeds, this is a pointer to a newly allocated and initialised
object.
*/
EXPORT_C CSendAsMessageTypes* CSendAsMessageTypes::NewL()
	{
	CSendAsMessageTypes *self = new (ELeave) CSendAsMessageTypes;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
Destructor.
*/
EXPORT_C CSendAsMessageTypes::~CSendAsMessageTypes()
	{
	delete iMessageTypeNames;
	iMessageTypeUids.Close();
	}
	
/**
The names of the available message types.

@return
An array of names for the available message types.
*/
EXPORT_C const MDesCArray& CSendAsMessageTypes::AvailableMessageTypes() const
	{
	return *iMessageTypeNames;
	}
	
/**
The UID of the message as specified by the index argument.

@param	aIndex
The index of the specified message type.

@return
The UID of the specified message type.
*/
EXPORT_C TUid CSendAsMessageTypes::MessageTypeUid(TInt aIndex) const
	{
	return iMessageTypeUids[aIndex];
	}
	
/**
The name associated with the given message type.

@param aUid
The message type UID whose associated name is to be retrieved.

@return
A descriptor holding the associated name.
*/
EXPORT_C TPtrC CSendAsMessageTypes::NameFromUidL(const TUid aUid) const
	{
	TInt nameIndex = User::LeaveIfError(iMessageTypeUids.Find(aUid));

	return (*iMessageTypeNames)[nameIndex];
	}
	
/**
The message type associated with the given name.

@param aName
The associated name of the message type whose UID is to be returned.

@return
The message type UID for the given name.
*/
EXPORT_C TUid CSendAsMessageTypes::UidFromNameL(const TDesC& aName) const
	{
	TInt uidIndex;
	if (iMessageTypeNames->Find(aName, uidIndex) != 0)
		{
		User::Leave(KErrNotFound);
		}
	return iMessageTypeUids[uidIndex];
	}
	
/**
Internalizes the object from the specified stream.

@see
RMsvReadStream

@param aReadStream
The stream to read from.

@internalComponent
*/
EXPORT_C void CSendAsMessageTypes::InternalizeL(RReadStream& aReadStream)
	{
	aReadStream.ReadInt16L(); // version - not used yet

	TCardinality cardinality;
	aReadStream >> cardinality;
	TInt count = cardinality;

	Reset();

	TUid uid;
	for (TInt i = 0; i < count; ++i)
		{
		HBufC* buf = HBufC::NewLC(aReadStream, KMaxSendAs2MessageTypeNameLength);
		aReadStream >> uid;
		AppendMessageTypeL(*buf, uid);
		
		CleanupStack::PopAndDestroy(buf);
		}
	}

/**
Externalizes the object from the specified stream.

@see
RMsvWriteStream

@param aWriteStream
The stream to write out to.

@internalComponent
*/
EXPORT_C void CSendAsMessageTypes::ExternalizeL(RWriteStream& aWriteStream) const
	{
	__ASSERT_ALWAYS((iMessageTypeNames->Count() == iMessageTypeUids.Count()), User::Invariant());
	
	// Version number
	aWriteStream.WriteInt16L(KStreamVersionSendAs2MessageType);

	TInt count = iMessageTypeNames->Count();
	// Write the count of name, uid object pairs to write out.
	aWriteStream << TCardinality(count);

	for (TInt i = 0; i < count; ++i)
		{
		if ((*iMessageTypeNames)[i].Length() > KMaxSendAs2MessageTypeNameLength)
			{
			// Limit the string length if necessary.
			aWriteStream.WriteL((*iMessageTypeNames)[i], KMaxSendAs2MessageTypeNameLength);
			}
		else
			{
			aWriteStream << (*iMessageTypeNames)[i];
			}
		aWriteStream << iMessageTypeUids[i];
		}
	}

/**
The number of message types.

@return
The number of message types.
*/
EXPORT_C TInt CSendAsMessageTypes::Count() const
	{
	return iMessageTypeUids.Count();
	}

/**
Deletes all elements from the array.
*/
EXPORT_C void CSendAsMessageTypes::Reset()
	{
	iMessageTypeNames->Reset();
	iMessageTypeUids.Reset();
	}

/**
Appends a new message type and associated name pair.

@param aMessageTypeName
The human readable name with which to associate with the given message type.

@param aMessageTypeUid
The message type to add.
*/
EXPORT_C void CSendAsMessageTypes::AppendMessageTypeL(const TDesC& aMessageTypeName, TUid aMessageTypeUid)
	{
	iMessageTypeUids.AppendL(aMessageTypeUid);
	TRAPD(err, iMessageTypeNames->AppendL(aMessageTypeName));
	if (err != KErrNone)
		{
		iMessageTypeUids.Remove(iMessageTypeUids.Count() - 1);
		User::Leave(err);
		}
	}

/**
Removes an existing message type and associated name pair.

@param aIndex
The index of the entry to remove.
*/
EXPORT_C void CSendAsMessageTypes::RemoveMessageType(TInt aIndex)
	{
	iMessageTypeNames->Delete(aIndex);
	iMessageTypeUids.Remove(aIndex);
	}

/**
Gives the size of the message types collection.

@return
The size of the message types collection.
*/
EXPORT_C TInt CSendAsMessageTypes::Size() const
	{
	const TInt msgTypeSize(sizeof(TUid));
	const TInt tIntSize(sizeof(TInt));
	TInt totalSize = sizeof(TInt16) + tIntSize; // The version and the count.
	TInt count = iMessageTypeNames->Count();

	for (TInt i = 0; i < count; ++i)
		{
		// Each message type consists of an integer which tells us the size of the
		// message type name, the message type name, and the message type uid. We must
		// add the size of each of these items to the total.
		// Note that when externalizing a message type, we don't explicitly write out an
		// integer containing the size of the message type name, but the externalize
		// routine for the descriptor will write one out, so we must include it in
		// our calculation here.
		totalSize += tIntSize + (*iMessageTypeNames)[i].Size() + msgTypeSize;
		}

	return totalSize;
	}
	
CSendAsMessageTypes::CSendAsMessageTypes()
: iMessageTypeUids(KMessageTypeNameArrayGranularity)
	{
	}
	
void CSendAsMessageTypes::ConstructL()
	{
	iMessageTypeNames = new (ELeave) CDesCArrayFlat(KMessageTypeNameArrayGranularity);
	}
