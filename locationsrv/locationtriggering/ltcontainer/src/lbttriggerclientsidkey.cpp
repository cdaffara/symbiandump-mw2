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



#include "lbttriggerclientsidkey.h"
#include "lbtramtriggerclientsidtree.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// TLbtTriggerClientSIDKey::Between
// 
// -----------------------------------------------------------------------------
//
void TLbtTriggerClientSIDKey::Between( const TAny* aLeft, const TAny* aRight, 
                                                    TBtreePivot& aPivot ) const
    {
	TUint32 left = *( TUint32* ) aLeft;
	TUint32 right = *( TUint32* ) aRight;

	TUint32 mid = left + ( right - left ) / 2;
	aPivot.Copy( ( TUint8* ) &mid, sizeof( mid ) );
    }


// -----------------------------------------------------------------------------
// TLbtTriggerClientSIDKey::Compare
// 
// -----------------------------------------------------------------------------
//
TInt TLbtTriggerClientSIDKey::Compare( const TAny* aLeft, const TAny* aRight ) const
    { 
	TUint32 left = *( TUint32* ) aLeft;
	TUint32 right = *( TUint32* ) aRight;

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
const TAny* TLbtTriggerClientSIDKey::Key( const TAny* anEntry ) const
    {
	CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID* t = ( CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID* ) anEntry;
	CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID* t1= ( CLbtRamTriggersClientSIDTree::TTriggerTreeBasedOnSID*) t;
	return ( TAny* ) &( t1->ownerSID );
    }
