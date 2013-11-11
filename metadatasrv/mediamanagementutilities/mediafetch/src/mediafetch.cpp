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
* Description:  Media Fetch implementation plug-in loader
*
*/


#include <ecom/ecom.h>

#include "mediafetch.h"
#include "mediafetch.hrh"
#include <mediafetchimplementation.h>

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CleanupResetAndDestroy
// -----------------------------------------------------------------------------
//
void CleanupResetAndDestroy( TAny* aObj )
    {
    if( aObj )
        {
        static_cast<RImplInfoPtrArray*>( aObj )->ResetAndDestroy();
        }
    }


// ======== MEMBER FUNCTIONS ========

inline CMediaFetch::CMediaFetch()
    {
    }

inline void CMediaFetch::ConstructL( TInt aMediaType )
    {
    RImplInfoPtrArray implementations;
    CleanupStack::PushL( TCleanupItem( CleanupResetAndDestroy,
                                       &implementations) );

    // Get a list of all Media Fetch plug-in implementations
    REComSession::ListImplementationsL(
                            TUid::Uid(  MEDIAFETCH_ECOM_INTERFACE ),
                            implementations );

    TUid uid( TUid::Null() );
    TInt version( -1 );

    const TInt count( implementations.Count() );

    // Loop through all found implementations
    for( TInt i( 0 ); i < count; ++i )
        {
        CImplementationInformation* implInfo = implementations[i];

        // Get media types supported by the implementation
        // The value is assumed to be in hexadecimal as defined in the
        // Media Fetch Implementation API
        TUint val( 0 );
        TLex8 lex( implInfo->DataType() );
        User::LeaveIfError( lex.Val( val, EHex ) );

        // Check that the implementation supports the wanted media type(s)
        if( ( aMediaType & val ) == aMediaType )
            {
            // Pick implementation with highest version number
            if( implInfo->Version() > version )
                {
                uid = implInfo->ImplementationUid();
                version = implInfo->Version();
                }
            }
        }

	if( uid == TUid::Null() )
		{
		User::Leave( KErrNotSupported );
		}


    // Instatiate the fetcher plug-in implementation
    iFetcher = CMediaFetchImplementation::NewL( uid );

    CleanupStack::PopAndDestroy(); // implementations
    }

CMediaFetch* CMediaFetch::NewL( TInt aMediaType )
    {
    CMediaFetch* self = CMediaFetch::NewLC( aMediaType );
    CleanupStack::Pop( self );
    return self;
    }

CMediaFetch* CMediaFetch::NewLC( TInt aMediaType )
    {
    CMediaFetch* self = new( ELeave ) CMediaFetch;
    CleanupStack::PushL( self );
    self->ConstructL( aMediaType );
    return self;
    }

CMediaFetch::~CMediaFetch()
    {
    delete iFetcher;
    REComSession::FinalClose();
    }

void CMediaFetch::SetMultiSelectionL( TBool aMultiSelect )
    {
    iFetcher->SetMultiSelectionL( aMultiSelect );
    }

void CMediaFetch::SetMimeTypesL( const MDesCArray& aMimeTypes )
    {
    iFetcher->SetMimeTypesL( aMimeTypes );
    }

void CMediaFetch::SetSelectionSoftkeyL( const TDesC& aSelectionSoftkey )
    {
    iFetcher->SetSelectionSoftkeyL( aSelectionSoftkey );
    }

void CMediaFetch::SetHeadingL( const TDesC& aHeading )
    {
    iFetcher->SetHeadingL( aHeading );
    }

void CMediaFetch::SetVerifierL( MMGFetchVerifier& aVerifier )
    {
    iFetcher->SetVerifierL( aVerifier );
    }

void CMediaFetch::GetCancelerL( MMGFetchCanceler*& aCanceler )
    {
    iFetcher->GetCancelerL( aCanceler );
    }

TBool CMediaFetch::LaunchL( CDesCArray& aSelectedFiles,
                            TMediaFileType aMediaType )
    {
    return iFetcher->LaunchL( aSelectedFiles, aMediaType );
    }
