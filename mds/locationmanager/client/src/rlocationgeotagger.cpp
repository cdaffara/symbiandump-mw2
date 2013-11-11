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
* Description:An interface to Location Manager geo tagger
*
*/

#include <f32file.h>
#include <s32mem.h>
#include <data_caging_path_literals.hrh>

#include <locationeventdef.h>
#include "rlocationgeotagger.h"
#include "locationmanagerdefs.h"
#include "locationmanagerdebug.h"

EXPORT_C RLocationGeoTagger::RLocationGeoTagger() 
            : iTagPendingData (NULL)
	{
	}

EXPORT_C RLocationGeoTagger::~RLocationGeoTagger()
	{
    delete iTagPendingData;
	}

// --------------------------------------------------------------------------
// RLocationGeoTagger::IsTagPending
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationGeoTagger::GeoTagStaus( TRequestStatus& aStatus, 
											   TGeoTaggingSatus& aTagPendingFlag )
    {
    LOG( "RLocationGeoTagger::IsTagPending(), begin" );
    
    delete iTagPendingData;
    iTagPendingData = new TPckg<TGeoTaggingSatus>( aTagPendingFlag);
    
    if ( iHandle && iTagPendingData )
        {
        SendReceive( ELocManTagPending, TIpcArgs( iTagPendingData ), aStatus );
        }
    else
        {
        if ( !iTagPendingData )
        	{
        	CompleteRequest(aStatus, KErrNoMemory);
        	}
        else
        	{
        	CompleteRequest(aStatus, KErrDisconnected);
        	}
        }
    LOG( "RLocationGeoTagger::IsTagPending(), end" );
    }

// --------------------------------------------------------------------------
// RLocationGeoTagger::StartGeoTagging
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationGeoTagger::StartGeoTagging
            (TRequestStatus& aStatus, const TConnectionOption aConnectionOption )
    {
    LOG( "RLocationGeoTagger::StartGeoTaggingL(), begin" );
    
    if ( iHandle )
        {
        SendReceive( ELocManStartGeoTaging, TIpcArgs( aConnectionOption ), aStatus);
        }
    LOG( "RLocationGeoTagger::StartGeoTaggingL(), end" );
    }

// --------------------------------------------------------------------------
// RLocationGeoTagger::CancelTagPendingRequest
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationGeoTagger::CancelTagPendingRequest()
    {
    LOG( "RLocationGeoTagger::CancelTagPendingRequest(), begin" );
    
    if ( iHandle )
        {
        SendReceive( ELocManCancelTagPendingReq);
        }
    LOG( "RLocationGeoTagger::CancelTagPendingRequest(), end" );
    }


// --------------------------------------------------------------------------
// RLocationGeoTagger::CancelTagPendingRequest
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationGeoTagger::CancelGeoTaggingRequest()
    {
    LOG( "RLocationGeoTagger::CancelGeoTaggingRequest(), begin" );
    if ( iHandle )
        {
        SendReceive( ELocManCancelGeoTaggingReq);
        }
    LOG( "RLocationGeoTagger::CancelGeoTaggingRequest(), end" );
    }


//End of File
