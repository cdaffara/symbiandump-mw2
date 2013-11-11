/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:This class contains helper methods for landmark serialization.
*
*/


#include <lbsposition.h>

#include "EPos_CPosLandmark.h"
#include "epos_cposreadbufstorage.h"
#include "epos_cposwritebufstorage.h"
#include "epos_poslandmarkserialization.h"

const TInt KVersion = 0x0100; // 1.0

// Keeps all landmark static-size info
struct TLandmarkBufferHeader
    {
    TInt            iSize;
    TInt            iVersion;

    TPosLmItemId    iId;
    TBool           iIsPartial;

    TBool           iIsIconSet;
    TInt            iIconIndex;
    TInt            iIconMaskIndex;

    TBool           iIsRadiusSet;
    TReal32         iCoverageRadius;

    TBool           iIsPositionSet;
    TLocality       iPosition;

    TInt            iCategoriesNum;
    TInt            iPositionFieldsNum;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* PosLandmarkSerialization::PackL( const CPosLandmark& aLandmark )
    {
    TLandmarkBufferHeader header;
    memclr( &header, sizeof header );
    TPckg<TLandmarkBufferHeader> headerPack( header );
    TPtrC name, desc, icon;
    //coverity[check_return :FALSE]
    //coverity[unchecked_value :FALSE]
    aLandmark.GetLandmarkName( name );
        //coverity[check_return :FALSE]
    //coverity[unchecked_value :FALSE]
    aLandmark.GetLandmarkDescription( desc );
    header.iIsIconSet = ( KErrNone == 
        aLandmark.GetIcon( icon, header.iIconIndex, header.iIconMaskIndex ) );

    // calculate required memory

    TInt size = CPosWriteBufStorage::PackedSize( headerPack ) + 
                CPosWriteBufStorage::PackedSize( name ) + 
                CPosWriteBufStorage::PackedSize( desc ) + 
                CPosWriteBufStorage::PackedSize( icon );

    RArray<TPosLmItemId> categories;
    CleanupClosePushL( categories );
    aLandmark.GetCategoriesL( categories );

    size += categories.Count() * sizeof( TPosLmItemId );

    TPositionFieldId field = aLandmark.FirstPositionFieldId();
    while ( field != EPositionFieldNone )
        {
        TPtrC fieldData;
        aLandmark.GetPositionField( field, fieldData );
        field = aLandmark.NextPositionFieldId( field );
        
        size += sizeof( TPositionFieldId ) + CPosWriteBufStorage::PackedSize( fieldData );
        }

    // collect static-size data
    header.iSize = size;
    header.iVersion = KVersion;
    header.iId = aLandmark.LandmarkId();
    header.iIsPartial = aLandmark.IsPartial();

    header.iIsRadiusSet = ( KErrNone == aLandmark.GetCoverageRadius( header.iCoverageRadius ) );
    header.iIsPositionSet = ( KErrNone == aLandmark.GetPosition( header.iPosition ) );

    header.iCategoriesNum = categories.Count();
    header.iPositionFieldsNum = aLandmark.NumOfAvailablePositionFields();

    // Put data
    CPosWriteBufStorage* buf = CPosWriteBufStorage::NewLC( size );

    buf->Put( headerPack );
    buf->Put( name );
    buf->Put( desc );
    buf->Put( icon );

    // categories
    for ( TInt i = 0; i < categories.Count(); i++ )
        {
        TPosLmItemId catId = categories[i];
        buf->Put( catId );
        }

    // position fields
    field = aLandmark.FirstPositionFieldId();
    while ( field != EPositionFieldNone )
        {
        TPtrC fieldData;
        aLandmark.GetPositionField( field, fieldData );

        buf->Put( field );        
        buf->Put( fieldData );

        field = aLandmark.NextPositionFieldId( field );
        }

    HBufC8* buffer = buf->FinalizeL();

    // Cleanup
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::PopAndDestroy( &categories );
    return buffer;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* PosLandmarkSerialization::UnpackL( const TDesC8& aBuffer )
    {
    TPckgBuf<TLandmarkBufferHeader> header;

    CPosLandmark* lm = CPosLandmark::NewLC();
    CPosReadBufStorage* buf = CPosReadBufStorage::NewLC( aBuffer );
    
    buf->GetL( header );

    if ( header().iSize != aBuffer.Size() ||
         header().iVersion != KVersion ||
         header().iCategoriesNum < 0 ||
         header().iPositionFieldsNum < 0 )
        {
        User::Leave( KErrCorrupt );
        }

    lm->SetLandmarkIdL( header().iId );
    lm->SetPartialL( header().iIsPartial );

    if ( header().iIsRadiusSet )
        {
        lm->SetCoverageRadius( header().iCoverageRadius );
        }
    
    if ( header().iIsPositionSet )
        {
        lm->SetPositionL( header().iPosition );
        }

    // strings

    TPtrC name, desc, icon;
    buf->GetL( name );
    buf->GetL( desc );
    buf->GetL( icon );

    lm->SetLandmarkNameL( name );
    lm->SetLandmarkDescriptionL( desc );

    if ( header().iIsIconSet )
        {
        lm->SetIconL( icon, header().iIconIndex, header().iIconMaskIndex );
        }

    // categories
    TPosLmItemId id = KPosLmNullItemId;
    for ( TInt i = 0; i < header().iCategoriesNum; i++ )
        {
        buf->GetL( id );
        lm->AddCategoryL( id );
        }

    // position fields
    TPtrC field;
    TPositionFieldId fieldId = EPositionFieldNone;
    for ( TInt i = 0; i < header().iPositionFieldsNum; i++ )
        {
        buf->GetL( fieldId );
        buf->GetL( field );
        lm->SetPositionFieldL( fieldId, field );
        }
        
    // cleanup
    CleanupStack::PopAndDestroy( buf ); 
    CleanupStack::Pop( lm ); 
    return lm;
    }
