/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Object locking features
*
*/

#include "mdsobjectlocklist.h"
#include "mdsnamespacedef.h"

CMdSObjectLockList* CMdSObjectLockList::NewL()
    {
    CMdSObjectLockList* self = CMdSObjectLockList::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CMdSObjectLockList* CMdSObjectLockList::NewLC()
    {
    CMdSObjectLockList* self = new ( ELeave ) CMdSObjectLockList();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;  
    }

void CMdSObjectLockList::ConstructL()
    {
    iLockListArray.Reset();
    }

CMdSObjectLockList::~CMdSObjectLockList()
    {
    iLockListArray.Close();
    }

void CMdSObjectLockList::LockObjectL( const CMdSServerSession& aSession, 
		const CMdsNamespaceDef& aNamespace, const TItemId aObjectId )
    {
    User::LeaveIfError( iLockListArray.InsertInOrder( 
    		TMdSLockEntry( &aSession, aNamespace, aObjectId ), 
    		TLinearOrder<TMdSLockEntry>(TMdSLockEntry::Compare) ) );
    }

void CMdSObjectLockList::UnlockBySession( const CMdSServerSession& aSession )
    {
    for (TInt i = iLockListArray.Count() - 1; i >= 0; --i)
        {
        if ( iLockListArray[i].iSession == &aSession ) // pointer comparision
            {
            iLockListArray.Remove(i);
            }
        }
    }

void CMdSObjectLockList::UnlockById( const CMdsNamespaceDef& aNamespace, 
		const TItemId aObjectId )
    {
    TInt position = Find( aNamespace, aObjectId );
    if (position >= 0)
    	{
    	iLockListArray.Remove(position);
    	}

    }

TBool CMdSObjectLockList::IsLocked( const CMdsNamespaceDef& aNamespace, 
		const TItemId aObjectId ) const
    {
    const TInt position = Find( aNamespace, aObjectId );
    if (position >= 0)
    	{
    	return ETrue;
    	}
    return EFalse;
    }

TInt CMdSObjectLockList::Find( const CMdsNamespaceDef& aNamespace, 
		const TItemId aObjectId ) const
	{
	TInt position;
	TInt err = iLockListArray.FindInOrder( TMdSLockEntry( 
			(const CMdSServerSession*)NULL, aNamespace, aObjectId ),
			position, TLinearOrder<TMdSLockEntry>(TMdSLockEntry::Compare) );

	if (err != KErrNone)
		{
		position = -1;
		}
	return position;
	}

void CMdSObjectLockList::Reset()
    {
    iLockListArray.Reset();
    }

TInt TMdSLockEntry::Compare( const TMdSLockEntry& aFirst, const 
		TMdSLockEntry& aSecond )
	{
	TInt result = aFirst.iNamespace.GetId() - aSecond.iNamespace.GetId();
	if ( result )
		{
		return result;
		}
	return aFirst.iId - aSecond.iId;
	}
