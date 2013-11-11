/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Data transfer object for harvest client data
*
*/

#include <harvesterclientdata.h>

// CHarvestClientData implementation

//==========================================================================
// Constructor
//==========================================================================
//
CHarvestClientData::CHarvestClientData()
    {
    // no implementation needed
    }

//==========================================================================
// Destructor
//==========================================================================
//
CHarvestClientData::~CHarvestClientData()
    {
    iAlbumIds.Close();
    }

//==========================================================================
// New
//==========================================================================
//
EXPORT_C CHarvestClientData* CHarvestClientData::New()
    {
    CHarvestClientData* self = NULL;
    TRAPD( err, self = NewL() );
    if ( err != KErrNone )
        {
        return NULL;
        }
    return self;
    }

//==========================================================================
// NewL
//==========================================================================
//
EXPORT_C CHarvestClientData* CHarvestClientData::NewL()
    {
    CHarvestClientData* self = new (ELeave) CHarvestClientData();
    return self;
    }

//==========================================================================
// SetAlbumIds
//==========================================================================
//
EXPORT_C void CHarvestClientData::SetAlbumIds( const RArray<TItemId>& aAlbumIds )
    {
    iAlbumIds.Reset();
    const TInt count = aAlbumIds.Count();
    iAlbumIds.Reserve( count );
    for ( TInt i = 0; i < count; i++ )
        {
        iAlbumIds.Append( aAlbumIds[i] );
        }
    }
