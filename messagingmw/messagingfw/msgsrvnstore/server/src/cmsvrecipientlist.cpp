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

#include <cmsvrecipientlist.h>
#include <e32cmn.h>

const TInt KMsvRecipientListDefaultGranularity = 8;

/**
Create a new CMsvRecipientList.

@return
Pointer to created and initialised object.
*/
CMsvRecipientList* CMsvRecipientList::NewL()
	{
	CMsvRecipientList* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/**
Create a new CMsvRecipientList.

@return
Pointer to created and initialised object. A copy of the pointer is left on the 
cleanup stack.
*/
CMsvRecipientList* CMsvRecipientList::NewLC()
	{
	CMsvRecipientList* self = new (ELeave) CMsvRecipientList();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMsvRecipientList::ConstructL()
	{
	iRecipients = new (ELeave) CDesCArrayFlat(KMsvRecipientListDefaultGranularity);
	}

CMsvRecipientList::CMsvRecipientList()
: iRecipientType(KMsvRecipientListDefaultGranularity)
	{
	}

/**
Destructor.
*/
CMsvRecipientList::~CMsvRecipientList()
	{
	delete iRecipients;
	iRecipientType.Close();
	}

/**
Deletes all elements from the array.
*/
EXPORT_C void CMsvRecipientList::Reset()
	{
	iRecipients->Reset();
	iRecipientType.Reset();
	}
	
/**
The number of accounts for this message type.

@return
The number of accounts.
*/
EXPORT_C TInt CMsvRecipientList::Count() const
	{
	return iRecipients->Count();
	}

/**
Appends a new address as a 'to' recipient.

@param aPtr
The address to add.
*/	
EXPORT_C void CMsvRecipientList::AppendL(const TDesC& aPtr)
	{
	AppendL(EMsvRecipientTo, aPtr);
	}

/**
Appends a new address a the given recipient type.

@param aPtr
The address to add.

@param aValue
The type of addressee. Possible types are EMtmRecipientTo, EMtmRecipientCc and 
EMtmRecipientBcc
*/
EXPORT_C void CMsvRecipientList::AppendL(TMsvRecipientType aValue, const TDesC& aPtr)
	{
	iRecipientType.AppendL(aValue);
	TRAPD(err, iRecipients->AppendL(aPtr));
	if (err != KErrNone)
		{
		iRecipientType.Remove(iRecipientType.Count() - 1);
		User::Leave(err);
		}
	}

/**
Deletes an existing address.

@param aPos
The index of the address to delete.
*/
EXPORT_C void CMsvRecipientList::Delete(TInt aPos)
	{
	iRecipientType.Remove(aPos);
	iRecipients->Delete(aPos);
	}

/**
Deletes number of existing addresses.

@param aPos
The start index of the addresses to delete.

@param aCount
The number of sequential addresses to delete.
*/
EXPORT_C void CMsvRecipientList::Delete(TInt aPos, TInt aCount)
	{
	iRecipients->Delete(aPos, aCount);

	for (;aCount > 0; --aCount)
		{
		iRecipientType.Remove(aPos);
		}
	}
	
/**
Returns the address type specified by the index given.

@param aIndex
The index of the address type to return.
*/	
EXPORT_C TMsvRecipientType CMsvRecipientList::Type(TInt aPos) const
	{
	return iRecipientType[aPos];
	}

/**
Returns the address specified by the index given.

@param aIndex
The index of the address to return.
*/	
EXPORT_C const TDesC& CMsvRecipientList::operator[](TInt aIndex) const
	{
	HBufC16* temp=(*(HBufC16**)iRecipients->At(aIndex));
	return *temp;
	}
	
EXPORT_C const MDesCArray& CMsvRecipientList::RecipientList() const
	{
	return *iRecipients;
	}

