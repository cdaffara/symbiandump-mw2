/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation Trigger Key class used to identify trigger
*                information stored in the BTree
*
*/



#include "lbttriggerkey.h"
#include "lbtcontainertriggerentry.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// TLbtTriggerKey::Between
// 
// -----------------------------------------------------------------------------
//
void TLbtTriggerKey::Between( const TAny* aLeft, const TAny* aRight, 
                                                    TBtreePivot& aPivot ) const
    {
	TUint32 left = *( TUint32* ) aLeft;
	TUint32 right = *( TUint32* ) aRight;

	TUint32 mid = left + ( right - left ) / 2;
	aPivot.Copy( ( TUint8* ) &mid, sizeof( mid ) );
    }


// -----------------------------------------------------------------------------
// TLbtTriggerKey::Compare
// 
// -----------------------------------------------------------------------------
//
TInt TLbtTriggerKey::Compare( const TAny* aLeft, const TAny* aRight ) const
    { 
	TUint32 left,right;
	if( aLeft )
		{
		 left = *( TUint32* ) aLeft;	
		}
	else
		{
		return -1;
		}
		
	if( aRight )
		{
		 right = *( TUint32* ) aRight;	
		}
	else
		{
		return 1;
		}

	if( left < right )
	    {
	    return -1;
	    }
	else if( left > right )
	    {
	    return 1;
	    }
	return 0;
    }


// -----------------------------------------------------------------------------
// TLbtTriggerKey::Key
// 
// -----------------------------------------------------------------------------
//
const TAny* TLbtTriggerKey::Key( const TAny* anEntry ) const
    {
/*	CLbtContainerTriggerEntry* t = (CLbtContainerTriggerEntry* ) anEntry;
	CLbtTriggerEntry *trigEntry=t->TriggerEntry();
	return ( TAny* ) &( trigEntry->Id() );*/
	
	//krishna's fix :)
	
	CLbtContainerTriggerEntry** t1 = (CLbtContainerTriggerEntry** ) (anEntry);
	CLbtContainerTriggerEntry* t=(CLbtContainerTriggerEntry*) *t1;
	CLbtTriggerEntry *trigEntry=t->TriggerEntry();
	return ( TAny* ) &( trigEntry->Id() );

    }
