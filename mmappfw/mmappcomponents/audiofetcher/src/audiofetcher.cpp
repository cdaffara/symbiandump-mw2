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
* Description: provide synchronized access to file system and metadata source
*
*/

// INCLUDE FILES
#include <aknViewAppUi.h>				// CAknViewAppUi 
#include <avkon.rsg>
#include <data_caging_path_literals.hrh>// KDC_APP_RESOURCE_DIR			
#include <eikapp.h>						// CEikApplication 
#include <e32cmn.h> 
#include <coemain.h>
#include <e32base.h>
#include <bautils.h>
#include <AknUtils.h>

#include "audiofetcher.hrh"            // For implementation id
#include "audiofetcher.h"
#include "audiofetcherlog.h"
#include "audiofetcherdialog.h"
#include <audiofetcherdialog.rsg>

_LIT(KAudioFetcherDialogResource, "AudioFetcherdialog.rsc" ); // AudioFetcherDialog Resource File
    
// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CAudioFetcher* CAudioFetcher::NewL()
    {
    WLOG("CAudioFetcher::NewL");
    CAudioFetcher* self = new( ELeave ) CAudioFetcher();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CAudioFetcher::ConstructL()
    {
    WLOG("CAudioFetcher::ConstructL");
    // No implementation required
    }
    
// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CAudioFetcher::CAudioFetcher()
    {
    WLOG("CAudioFetcher::CAudioFetcher");
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CAudioFetcher::~CAudioFetcher()
    {
    WLOG("CAudioFetcher::~CAudioFetcher >");


    if ( iResourceOffset )
        {
        CCoeEnv::Static()->DeleteResourceFile(iResourceOffset);
        }

    delete iSelectionSoftkey;
    delete iTitle;
    
    WLOG("CAudioFetcher::~CAudioFetcher <");
    }
    
// ---------------------------------------------------------------------------
// SetMultiSelectionL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::SetMultiSelectionL( TBool aMultiSelect )
    {
    WLOG("CAudioFetcher::SetMultiSelectionL");
    iIsMultiSelection = aMultiSelect;
    }

// ---------------------------------------------------------------------------
// SetMimeTypesL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::SetMimeTypesL( const MDesCArray& /*aMimeTypes*/ )
    {
    WLOG("CAudioFetcher::SetMimeTypesL (not supported)");
    // No implementation required (not supported)
    }

// ---------------------------------------------------------------------------
// SetSelectionSoftkeyL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::SetSelectionSoftkeyL( const TDesC& aSelectionSoftkey )
    {
    WLOG("CAudioFetcher::SetSelectionSoftkeyL");
	
	HBufC* tmp = aSelectionSoftkey.AllocL();
	delete iSelectionSoftkey;
	iSelectionSoftkey = tmp;
    }

// ---------------------------------------------------------------------------
// SetHeadingL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::SetHeadingL( const TDesC& aTitle )
    {
    WLOG("CAudioFetcher::SetHeadingL");
	
	HBufC* tmp = aTitle.AllocL();
	delete iTitle;
	iTitle = tmp;
    }

// ---------------------------------------------------------------------------
// SetVerifierL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::SetVerifierL( MMGFetchVerifier& aVerifier )
    {
    WLOG("CAudioFetcher::SetVerifierL");

	iVerifier = &aVerifier;
    }

// ---------------------------------------------------------------------------
// GetCancelerL
// ---------------------------------------------------------------------------
//
void CAudioFetcher::GetCancelerL( MMGFetchCanceler*& aCanceler )
    {
    WLOG("CAudioFetcher::GetCancelerL");
	
	aCanceler = this;
    }


// ---------------------------------------------------------------------------
// LaunchL
// ---------------------------------------------------------------------------
//
TBool CAudioFetcher::LaunchL( CDesCArray& aSelectedFiles,
                               TMediaFileType aMediaType )
   	{
   	WLOG("CAudioFetcher::LaunchL >");
		
	TInt buttonId;
    TFileName resFile;
    TParse parse;
    parse.Set( KAudioFetcherDialogResource, &KDC_APP_RESOURCE_DIR, NULL );

    resFile.Append(parse.FullName()); 
    GetResourceFilenameL( resFile );
    iResourceOffset = CCoeEnv::Static()->AddResourceFileL( resFile );

	// Configure heading and filter type
	switch ( aMediaType )
	    {
	    case EAudioFile:
	    case EMusicFile:
	        {
            iTitle = TitleForAudioFetcherL();
            break;
	        }
	    case EAnyMediaFile:
	        {
            iTitle = TitleForAnyMediaFileFetcherL();
	        break;
	        }
	    default:
	        {
	        WLOG("CAudioFetcher::LaunchL  - Error: Unsupported media type, Leaving");
            User::Leave( KErrNotSupported );
	        }
	    }
	
	// create the dialog first
	CAudioFetcherDialog* dialog =
	    CAudioFetcherDialog::NewL( aSelectedFiles, iVerifier, *iTitle, iIsMultiSelection, aMediaType );
	
	// Returns zero when Fetcher is cancelled by User.
	buttonId = dialog->ExecuteLD( R_MODAL_SINGLE_DIALOG );	
	
	dialog = NULL;
	WLOG("CAudioFetcher::LaunchL <");
    // Return false if the fetcher was canceled by user
	return ( 0 != buttonId && aSelectedFiles.Count() != 0 );
  	}

// ---------------------------------------------------------------------------
// Return nearest resource file
// ---------------------------------------------------------------------------
//
void CAudioFetcher::GetResourceFilenameL( TFileName& aResFile )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    // don't use AknUtils CompleteWithAppPath  
    aResFile.Insert( 0, TDriveUnit( EDriveC).Name() );
    // try to locate the localised resource
    BaflUtils::NearestLanguageFile( fs, aResFile );
    // if the localised resource is found, the file name is changed to 
    // the localised name (z:\apps\resources\xxx.r001)
    if( !BaflUtils::FileExists( fs, aResFile ) )
        {
        // not found on c drive, try z
        aResFile.Replace( 0, KMaxDriveName, TDriveUnit( EDriveZ ).Name() );
        // try to locate the localised resource again
        BaflUtils::NearestLanguageFile( fs, aResFile );
        // if file was not found this time, there is no localised 
        // resource with the name
        if (!BaflUtils::FileExists( fs, aResFile ) )
            {
            User::Leave( KErrNotFound );
            }
        }
    
    CleanupStack::PopAndDestroy( &fs );
    }

// ---------------------------------------------------------------------------
// Return Dialog title
// ---------------------------------------------------------------------------
//
HBufC* CAudioFetcher::TitleForAudioFetcherL() const
    {
    WLOG("CAudioFetcher::TitleForAudioFetcherL()");
    return TitleL( R_AUDIOFETCHER_DIALOG_TITLE_AUDIO );
    }

// ---------------------------------------------------------------------------
// Return Dialog title
// ---------------------------------------------------------------------------
//
HBufC* CAudioFetcher::TitleForAnyMediaFileFetcherL() const
    {
    WLOG("CAudioFetcher::TitleForAudioFetcherL()");
    return TitleL( R_AUDIOFETCHER_DIALOG_TITLE_ANY_FILE );
    }

// ---------------------------------------------------------------------------
// Return Dialog title
// ---------------------------------------------------------------------------
//
HBufC* CAudioFetcher::TitleL( TInt aSingleSelectionTitleResourceId ) const
    {
    WLOG("CAudioFetcher::TitleL()");

    if( !iTitle )
        {
        TInt resourceId = aSingleSelectionTitleResourceId;
        return CCoeEnv::Static()->AllocReadResourceL( resourceId );
        }
    return iTitle;
    }

// -----------------------------------------------------------------------------
// From MMGFetchCanceler
// -----------------------------------------------------------------------------
//	
void CAudioFetcher::CancelFetcherL()
    {
    WLOG("CAudioFetcher::CancelFetcherL");
    
    }

// End of File


