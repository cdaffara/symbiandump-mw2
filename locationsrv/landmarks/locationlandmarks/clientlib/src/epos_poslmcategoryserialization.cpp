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
* Description:This class contains helper methods for landmark category serialization.
*
*/


#include <lbsposition.h>

#include "EPos_CPosLandmarkCategory.h"
#include "epos_cposreadbufstorage.h"
#include "epos_cposwritebufstorage.h"
#include "epos_poslmcategoryserialization.h"

const TInt KVersion = 0x0100; // 1.0

struct TCategoryBufferHeader
    {
    TInt            iSize;
    TInt            iVersion;

    TPosLmItemId    iId;
    TPosLmGlobalCategory iGlobalCategory;
    
    TBool           iIsIconSet;
    TInt            iIconIndex;
    TInt            iIconMaskIndex;
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* PosLmCategorySerialization::PackL( const CPosLandmarkCategory& aCategory )
    {
    TCategoryBufferHeader header;
    memclr( &header, sizeof header );
    TPckg<TCategoryBufferHeader> infoPack( header );

    TPtrC name, icon;
    //coverity[check_return :FALSE]
    //coverity[unchecked_value :FALSE]
    aCategory.GetCategoryName( name );
    header.iIsIconSet = ( KErrNone == 
        aCategory.GetIcon( icon, header.iIconIndex, header.iIconMaskIndex ) );

    // calculate required memory

    TInt size = CPosWriteBufStorage::PackedSize( infoPack ) + 
                CPosWriteBufStorage::PackedSize( name ) +
                CPosWriteBufStorage::PackedSize( icon );

    // collect static-size data
    header.iSize = size;
    header.iVersion = KVersion;
    header.iId = aCategory.CategoryId();
    header.iGlobalCategory = aCategory.GlobalCategory();

    // Put data
    CPosWriteBufStorage* buf = CPosWriteBufStorage::NewLC( size );
    
    buf->Put( infoPack );
    buf->Put( name );
    buf->Put( icon );

    HBufC8* buffer = buf->FinalizeL();
    
    // Cleanup
    CleanupStack::PopAndDestroy( buf );
    return buffer;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* PosLmCategorySerialization::UnpackL( const TDesC8& aBuffer )
    {
    TPckgBuf<TCategoryBufferHeader> header;

    CPosLandmarkCategory* cat = CPosLandmarkCategory::NewLC();
    CPosReadBufStorage* buf = CPosReadBufStorage::NewLC( aBuffer );
    
    buf->GetL( header );

    if ( header().iSize != aBuffer.Size() ||
         header().iVersion != KVersion )
        {
        User::Leave( KErrCorrupt );
        }

    cat->SetCategoryIdL( header().iId );
    cat->SetGlobalCategory( header().iGlobalCategory );

    // strings
    TPtrC name, icon;

    buf->GetL( name );
    buf->GetL( icon );

    if ( name.Length() )
        {
        cat->SetCategoryNameL( name );
        }
        
    if ( header().iIsIconSet )
        {
        cat->SetIconL( icon, header().iIconIndex, header().iIconMaskIndex );
        }
    
    // cleanup
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::Pop( cat );
    return cat;
    }
