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
* Description:
*
*/
// CBlacklistItem

#include "blacklistitem.h"
#include "mdsutils.h"
#include "mdcserializationbuffer.h"

// ---------------------------------------------------------------------------
// Standard NewL for CBlacklistItem
// ---------------------------------------------------------------------------
//
CBlacklistItem* CBlacklistItem::NewL( const TInt64& aModified, const TDesC& aUri,
    const TUint32 aMediaId )
    {
    CBlacklistItem* self = new (ELeave) CBlacklistItem( aModified, aMediaId );
    CleanupStack::PushL( self );
    self->ConstructL( aUri );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ConstructL()
// 2nd phase construction.
// ---------------------------------------------------------------------------
//
void CBlacklistItem::ConstructL( const TDesC& aUri )
    {
    iUri = aUri.AllocL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CBlacklistItem::~CBlacklistItem()
    {
    delete iUri;
    iUri = NULL;
    }

// ---------------------------------------------------------------------------
// Constructor for CBlacklistItem
// ---------------------------------------------------------------------------
//
CBlacklistItem::CBlacklistItem( const TInt64& aModified,
    const TUint32 aMediaId )
    : iModified( aModified ), iUri( NULL ), iMediaId( aMediaId )
    {
    }

// ---------------------------------------------------------------------------
// Search a file from blacklist cache.
// ---------------------------------------------------------------------------
//
TBool CBlacklistItem::Compare( const TDesC& aUri, const TUint32 aMediaId )
    {
    if ( iUri && MdsUtils::Compare( *iUri, aUri ) == 0 && iMediaId == aMediaId )
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Returns item's last modification time.
// ---------------------------------------------------------------------------
//
const TInt64& CBlacklistItem::Modified()
    {
    return iModified;
    }

// ---------------------------------------------------------------------------
// Returns item's MediaId.
// ---------------------------------------------------------------------------
//
const TUint32& CBlacklistItem::MediaId()
    {
    return iMediaId;
    }

// ---------------------------------------------------------------------------
// Returns item's Uri.
// ---------------------------------------------------------------------------
//
const HBufC* CBlacklistItem::Uri()
    {
    return iUri;
    }
    // ---------------------------------------------------------------------------
// Returns item's Uri.
// ---------------------------------------------------------------------------
//
TInt CBlacklistItem::RequiredBufferSize()
    {
    return ( CMdCSerializationBuffer::KRequiredSizeForTInt64 +
        CMdCSerializationBuffer::KRequiredSizeForTUint32 + 
        CMdCSerializationBuffer::RequiredSize( *iUri ) );
    }
    
// End of file
