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
// CMsvMtmOperation
// 
//

inline void CMsvMtmOperation::CopyToLocal(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpCopyToLocal ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::CopyFromLocal(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpCopyFromLocal ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::CopyWithinService(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpCopyWithinService ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::MoveToLocal(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpMoveToLocal ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::MoveFromLocal(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpMoveFromLocal ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::MoveWithinService(CMsvEntrySelection* aSelection, TMsvId aDestination)
	{
	StoreParameters(EMtmOpMoveWithinService ,aSelection ,aDestination, NULL);
	}

inline void CMsvMtmOperation::DeleteAll(CMsvEntrySelection* aSelection)
	{
	StoreParameters(EMtmOpDeleteAll ,aSelection ,0, NULL);
	}

inline void CMsvMtmOperation::CreateL(const TMsvEntry& aNewEntry)
	{
	StoreParametersL(EMtmOpCreate, aNewEntry);
	}

inline void CMsvMtmOperation::ChangeL(const TMsvEntry& aNewEntry)
	{
	StoreParametersL(EMtmOpChange, aNewEntry);
	}

inline void CMsvMtmOperation::ChangeEntriesL(CMsvEntrySelection* aSelection, TInt aMark)
    {
    StoreParameters(EMtmOpChangeEntries, aSelection ,aMark, NULL);
    }

inline void CMsvMtmOperation::StartCommand(CMsvEntrySelection* aSelection, TInt aCommand, HBufC8* aParameter)
	{
	StoreParameters(EMtmOpCommand ,aSelection ,aCommand, aParameter);
	}

#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
inline void CMsvMtmOperation::SetThreadId(TThreadId aThreadId)
 	{
 	iThreadId = aThreadId;
 	}
inline void CMsvMtmOperation::SetCapability(TBool aCap)
    {
    iHasCapability = aCap;
    }
inline TBool CMsvMtmOperation::Capability()
    {
    return iHasCapability;
    }
#endif
