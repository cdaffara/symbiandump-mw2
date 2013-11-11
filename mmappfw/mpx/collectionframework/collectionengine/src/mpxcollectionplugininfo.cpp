/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Derived plugin info class for collection tags
*
*/


#include <e32base.h>
#include "mpxcollectionplugininfo.h"

_LIT8(KMPXPluginCacheTag, "<c>" );

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionPluginInfo::CMPXCollectionPluginInfo()
    {
    
    }


// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionPluginInfo::ConstructL( const CImplementationInformation& aData )
    {
    // Base constructor
    CMPXPluginInfo::ConstructL( aData );
    }

// ---------------------------------------------------------------------------
// Two-Phased constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionPluginInfo* CMPXCollectionPluginInfo::NewL( 
                                     const CImplementationInformation& aData )
    {
    CMPXCollectionPluginInfo* self = new( ELeave ) CMPXCollectionPluginInfo;
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionPluginInfo* CMPXCollectionPluginInfo::NewLC( 
                                     const CImplementationInformation& aData )
    {
    CMPXCollectionPluginInfo* self = new( ELeave ) CMPXCollectionPluginInfo;
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    return self;
    }

// ---------------------------------------------------------------------------
// Virtual destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionPluginInfo::~CMPXCollectionPluginInfo()
    {
    iNonCachableAttrs.Close();
    }

// ---------------------------------------------------------------------------
// Extended tag parsing functionality
// ---------------------------------------------------------------------------
//
void CMPXCollectionPluginInfo::ProcessTaggedDataExL( const TDesC8& aTag,
                                                     const TDesC8& aData)
    {
    if( aTag == KMPXPluginCacheTag )
        {
        ExtractNonCacheableAttrsL( aData );
        }
    }

// ---------------------------------------------------------------------------
// Get the list of non-cacheable attributes
// ---------------------------------------------------------------------------
//
const TArray<TUid> CMPXCollectionPluginInfo::NonCacheableAttrs()
    {
    return iNonCachableAttrs.Array();
    }

// ---------------------------------------------------------------------------
// Process the non-cacheable attributes
// ---------------------------------------------------------------------------
//
void CMPXCollectionPluginInfo::ExtractNonCacheableAttrsL( const TDesC8& aData )
    {
    ExtractUidsFromTextL( aData, iNonCachableAttrs );
    }

// END OF FILE
