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
#include <csendasaccounts.h>

const TInt KMessageAccountArrayGranularity = 8;
const TInt16 KStreamVersionSendAs2Account = 1;
const TInt KMaxSendAs2AccountNameLength = 1024;

/**
Create a new CSendAsAccount.

@return
If the function succeeds, this is a pointer to a newly allocated and initialised
object.
*/
EXPORT_C CSendAsAccounts* CSendAsAccounts::NewL()
	{
	CSendAsAccounts *self = new (ELeave) CSendAsAccounts;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Destructor.
*/	
EXPORT_C CSendAsAccounts::~CSendAsAccounts()
	{
	delete iAccountNames;
	iAccounts.Close();
	}

/**
The UID of the message type.

@return
The UID of the message type.
*/
EXPORT_C TUid CSendAsAccounts::MessageType() const
	{
	return iMessageType;
	}
	
/**
The array of names of accounts for this message type.

@return
An array of name of accounts.
*/
EXPORT_C const MDesCArray& CSendAsAccounts::AccountNames() const
	{
	return (*iAccountNames);
	}
	
/**
The account specified by the index argument.

@param	aIndex
The index of the specified account.

@return
The account specified by the index argument.
*/
EXPORT_C TSendAsAccount CSendAsAccounts::Account(TInt aIndex) const
	{
	return iAccounts[aIndex];
	}
	
/**
The number of accounts for this message type.

@return
The number of accounts.
*/
EXPORT_C TInt CSendAsAccounts::Count() const
	{
	return iAccountNames->Count();
	}

/**
Deletes all elements from the array.
*/
EXPORT_C void CSendAsAccounts::Reset()
	{
	iAccountNames->Reset();
	iAccounts.Reset();
	}
	
/**
The name associated with the given account.

@param aAccount
The account whose associated name is to be returned.

@return
A descriptor holding the associated name.
*/
EXPORT_C TPtrC CSendAsAccounts::NameFromAccountL(const TSendAsAccount aAccount) const
	{
	TInt nameIndex = User::LeaveIfError(iAccounts.Find(aAccount));
	return (*iAccountNames)[nameIndex];
	}
	
/**
The account associated with the given name.

@param aName
The associated name for the account to be returned.

@return
An account for the associated name.

@leave  KErrNotFound
An account with the given name cannot be found.
*/
EXPORT_C TSendAsAccount CSendAsAccounts::AccountFromNameL(const TDesC& aName) const
	{
	TInt acctIndex;
	if (iAccountNames->Find(aName, acctIndex) != 0)
		{
		User::Leave(KErrNotFound);
		}
	return iAccounts[acctIndex];
	}

/**
Internalizes the object from the specified stream.

@see
RMsvReadStream

@param aReadStream
The stream to read from.

@internalComponent
*/
EXPORT_C void CSendAsAccounts::InternalizeL(RReadStream& aReadStream)
	{
	aReadStream.ReadInt16L(); // version - not used yet

	aReadStream >> iMessageType;

	TInt count = aReadStream.ReadInt32L();

	Reset();

	TSendAsAccount sendAsAccount;
	for (TInt i = 0; i < count; ++i)
		{
		HBufC* buf = HBufC::NewLC(aReadStream, KMaxSendAs2AccountNameLength);
		aReadStream >> sendAsAccount;
		AppendAccountL(*buf, sendAsAccount);
		
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
EXPORT_C void CSendAsAccounts::ExternalizeL(RWriteStream& aWriteStream) const
	{
	__ASSERT_ALWAYS((iAccountNames->Count() == iAccounts.Count()), User::Invariant());
	
	aWriteStream.WriteInt16L(KStreamVersionSendAs2Account); // Version number
	aWriteStream << iMessageType; // Message type

	TInt count = iAccountNames->Count();	
	aWriteStream.WriteInt32L(count); // The count of name, uid object pairs to write out.

	for (TInt i = 0; i < count; ++i)
		{
		if ((*iAccountNames)[i].Length() > KMaxSendAs2AccountNameLength)
			{
			// Limit the string length if necessary.
			aWriteStream.WriteL((*iAccountNames)[i], KMaxSendAs2AccountNameLength);
			}
		else
			{
			aWriteStream << (*iAccountNames)[i];
			}
		aWriteStream << iAccounts[i];
		}
	}

/**
Sets UID of the message type.

@param aMessageType
The UID of the message type.
*/
EXPORT_C void CSendAsAccounts::SetMessageType(TUid aMessageType)
	{
	iMessageType = aMessageType;
	}

/**
Appends a new account and associated name pair.

@param aAccountName
The human readable name with which to associate with the given account.

@param aAccount
The account to add.
*/	
EXPORT_C void CSendAsAccounts::AppendAccountL(const TDesC& aAccountName, TSendAsAccount aAccount)
	{
	iAccounts.AppendL(aAccount);
	TRAPD(err, iAccountNames->AppendL(aAccountName));
	if (err != KErrNone)
		{
		iAccounts.Remove(iAccounts.Count() - 1);
		User::Leave(err);
		}
	}	

/**
Removes an existing account and associated name pair.

@param aIndex
The index of the entry to remove.
*/
EXPORT_C void CSendAsAccounts::RemoveAccount(TInt aIndex)
	{
	iAccountNames->Delete(aIndex);
	iAccounts.Remove(aIndex);
	}

/**
Gives the size of the accounts collection.

@return
The size of the accounts collection.
*/
EXPORT_C TInt CSendAsAccounts::Size() const
	{
	const TInt acctSize(sizeof(TSendAsAccount));
	const TInt tIntSize(sizeof(TInt));
	TInt totalSize = sizeof(TInt16) + sizeof(TUid); // The version and message type.
	TInt count = iAccountNames->Count();

	totalSize += tIntSize; // Size of the number of accounts

	for (TInt i = 0; i < count; ++i)
		{
		// Each account consists of an integer which tells us the size of the
		// account name, the account name, and the the TSendAsAccount data. We must
		// add the size of each of these items to the total.
		// Note that when externalizing an account, we don't explicitly write out an
		// integer containing the size of the account name, but the externalize
		// routine for the descriptor will write one out, so we must include it in
		// our calculation here.
		totalSize += tIntSize + (*iAccountNames)[i].Size() + acctSize;
		}

	return totalSize;
	}
	
CSendAsAccounts::CSendAsAccounts()
: iMessageType(KNullUid), iAccounts(KMessageAccountArrayGranularity)
	{
	}
	
void CSendAsAccounts::ConstructL()
	{
	iAccountNames = new (ELeave) CDesCArrayFlat(KMessageAccountArrayGranularity);
	}
