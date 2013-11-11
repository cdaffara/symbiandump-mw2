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
* Description:  Rename objects
*
*/


#include <badesca.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/tmtptypeuint32.h>
#include <mtp/cmtpobjectmetadata.h>

#include "cabstractmediamtpdataproviderrenameobject.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpmetadataaccesswrapper.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderRenameObject::NewL
// Two phase constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderRenameObject* CAbstractMediaMtpDataProviderRenameObject::NewL( MMTPDataProviderFramework& aFramework,
    CMmMtpDpMetadataAccessWrapper& aWrapper )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderRenameObject::NewL" ) );

    CAbstractMediaMtpDataProviderRenameObject* self = new ( ELeave ) CAbstractMediaMtpDataProviderRenameObject( aFramework,
        aWrapper );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderRenameObject::NewL" ) );
    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderRenameObject::CAbstractMediaMtpDataProviderRenameObject
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderRenameObject::CAbstractMediaMtpDataProviderRenameObject( MMTPDataProviderFramework& aFramework,
    CMmMtpDpMetadataAccessWrapper& aWrapper ) :
        CRenameObject( aFramework, aWrapper )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderRenameObject::~CAbstractMediaMtpDataProviderRenameObject
// destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderRenameObject::~CAbstractMediaMtpDataProviderRenameObject()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderRenameObject::ConstructL
//
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderRenameObject::ConstructL()
    {
    CRenameObject::ConstructL();
    }

void CAbstractMediaMtpDataProviderRenameObject::PerformAdditionalActionL()
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderRenameObject::PerformAdditionalActionL" ) );

    // modified the dummy array
    if ( iObjectInfo->Uint( CMTPObjectMetaData::EFormatCode )
        == EMTPFormatCodeAbstractAudioVideoPlaylist )
        {
        iWrapper.DeleteDummyFile( iOldFileName );
        iWrapper.AddDummyFileL( iFileName );
        // Do not update MPX db to keep the same behavior in mass storage and device file manager.
        }

     TRAPD( err, iWrapper.RenameObjectL( *iObjectInfo, iFileName ) );

     // should not fail for 1 file, keep it going, as folder already renamed
     if ( err != KErrNone )
         PRINT1( _L( "MM MTP <> CRenameObject::PerformAdditionalActionL err = %d" ), err );

    // the reference DB use PUID
    // so we needn't to update the reference DB

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderRenameObject::PerformAdditionalActionL" ) );
    }

//end of file
