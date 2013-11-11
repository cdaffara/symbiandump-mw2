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
* Description:  Media Fetch API
*
*/


#include    <e32std.h>
#include    <MGFetch.h>
#include    <MMGFetchCanceler.h>
#include    "mediafetch.h"

// ======== MEMBER FUNCTIONS ========

EXPORT_C TBool MGFetch::RunL( CDesCArray& aSelectedFiles,
                              TMediaFileType aMediaType,
                              TBool aMultiSelect,
                              MMGFetchVerifier* aVerifier )
    {

    return RunL( aSelectedFiles, aMediaType, aMultiSelect,
                 KNullDesC, KNullDesC, NULL, aVerifier );
    }

EXPORT_C TBool MGFetch::RunL( CDesCArray& aSelectedFiles,
                              TMediaFileType aMediaType,
                              TBool aMultiSelect,
                              const TDesC& aSelectionSoftkey,
                              const TDesC& aHeading,
                              MMGFetchVerifier* aVerifier )
    {

    return RunL( aSelectedFiles, aMediaType, aMultiSelect, aSelectionSoftkey,
                 aHeading, NULL, aVerifier );
    }

EXPORT_C TBool MGFetch::RunL( CDesCArray& aSelectedFiles,
                              TMediaFileType aMediaType,
                              TBool aMultiSelect,
                              const TDesC& aSelectionSoftkey,
                              const TDesC& aHeading,
                              MDesCArray* aMimeTypes,
                              MMGFetchVerifier* aVerifier
                              )
    {
    // We just need a pointer to pass as reference, not used
    MMGFetchCanceler* dummyCanceler = NULL;

    return RunL( aSelectedFiles, aMediaType, aMultiSelect, aSelectionSoftkey,
                 aHeading, aMimeTypes, dummyCanceler, aVerifier );

    }

EXPORT_C TBool MGFetch::RunL( CDesCArray& aSelectedFiles,
                              TMediaFileType aMediaType,
                              TBool aMultiSelect,
                              const TDesC& aSelectionSoftkey,
                              const TDesC& aHeading,
                              MDesCArray* aMimeTypes,
                              MMGFetchCanceler*& aCanceler,
                              MMGFetchVerifier* aVerifier
                              )
    {
    CMediaFetch* fetch = CMediaFetch::NewLC( (TInt)aMediaType );

    // Pass optional parameters for plug-in implemention
    fetch->SetMultiSelectionL( aMultiSelect );
    if( aMimeTypes )
    	{
	    fetch->SetMimeTypesL( *aMimeTypes );
    	}
    if( aSelectionSoftkey != KNullDesC )
    	{
		fetch->SetSelectionSoftkeyL( aSelectionSoftkey );
    	}
    if( aHeading != KNullDesC  )
    	{
	    fetch->SetHeadingL( aHeading );
    	}
    if( aVerifier )
    	{
	    fetch->SetVerifierL( *aVerifier );
    	}

    // Get canceler interface
    fetch->GetCancelerL( aCanceler );

    // Launch fetcher
    
    TBool result( EFalse );
    TRAPD( error, result = fetch->LaunchL( aSelectedFiles, aMediaType ) );
    aCanceler = NULL;
    CleanupStack::PopAndDestroy( fetch );
    User::LeaveIfError( error );

    return result;
    }
