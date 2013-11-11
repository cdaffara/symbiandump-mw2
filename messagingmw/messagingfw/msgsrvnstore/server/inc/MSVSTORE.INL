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
// CMsvStore
// 
//

inline void CMsvStore::Lock()
	{
	iLockStatus=EMsvStoreLocked;
	}

inline const CMsvDictionaryStore& CMsvStore::Store() const
	{
	return *iStore;
	}


inline CMsvDictionaryStore& CMsvStore::Store()
	{
	return *iStore;
	}

inline TBool CMsvStore::IsNullL() const
/** Tests whether the message store contains any streams.

@return ETrue if the store has no streams, else EFalse */
	{
	return iStore->IsNullL();
	}

inline TBool CMsvStore::IsPresentL(TUid aUid) const
/** Tests whether the message store contains a stream identifier.

@param aUid Identifier of stream 
@return ETrue if the store has a stream aUid, else EFalse */
	{
	return iStore->IsPresentL(aUid);
	}

inline TBool CMsvStore::HasBodyTextL() const
/** Checks if the store has a body text stream.

@return ETrue if the store has a body text stream, else EFalse */
	{
	return iStore->IsPresentL(KMsvEntryRichTextBody);
	}




//**********************************
// RMsvReadStream
//**********************************

inline RMsvWriteStream::RMsvWriteStream(const MExternalizer<TStreamRef>& anExter)
	: RDictionaryWriteStream(anExter)
	{
	}
