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
* Description:  CMnrpGeocodingService class implementation
*
*/


#include <e32base.h>

#include <aknnotewrappers.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mngeocodingservicebase.h>

#include "debug.h"

#include "mnrpappserverback.h"
#include "mnrpappuiback.h"
#include "mnrpdocumentback.h"
#include "mnrpengine.h"

#include <mnrefproviderback.rsg>
#include "mnrpgeocodingservice.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
//  Cleanup item for RPointerArray<CPosLandmark>
// ---------------------------------------------------------------------------
//
void CleanupLandmarksArray( TAny* aArray )
    {
    ( static_cast<RPointerArray<CPosLandmark>*>( aArray ) )->ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
//  Cleanup item for moving application fore/background
// ---------------------------------------------------------------------------
//
void CleanupForeground( TAny* aAppUi )
    {
    ( static_cast<CMnrpAppUiBack*>( aAppUi ) )->MoveBackground();
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpGeocodingService::CMnrpGeocodingService( CMnrpEngine& aEngine )
 :  iEngine( aEngine )
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpGeocodingService* CMnrpGeocodingService::NewL( CMnrpEngine& aEngine )
    {
    CMnrpGeocodingService* self = new (ELeave) CMnrpGeocodingService( aEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpGeocodingService::~CMnrpGeocodingService()
    {
    delete iProcessor;
    delete iLandmark;
    delete iPlainAddress;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::ConstructL()
    {
    BaseConstructL();
    
    iProcessor = CIdle::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::LeaveIfBusyL()
    {
    if ( iProcessor->IsActive() )
        {
        User::Leave( KErrInUse );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::StartProcessingL()
    {
    iProcessor->Start( TCallBack( CMnrpGeocodingService::GeocodingProcessingCallBack, this ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnrpGeocodingService::GeocodingProcessingCallBack( TAny* aPtr )
    {
    CMnrpGeocodingService* self =    
        static_cast<CMnrpGeocodingService*> ( aPtr );
        
    self->HandleGeocodingCompleted();
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::HandleGeocodingCompleted()
    {
    TInt err = KErrNone;
    TRAP( err, FinishProcessingL() );
    if ( err )
        {
        CompleteRequest( err );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::FinishProcessingL()
    {
    LOG1("MnRefProvider: FinishProcessingL in, cursvc %d", iCurrentService);

    TLocality loc;
    CPosLandmark* lm = NULL;
    
    switch ( iCurrentService )
        {
        case ECoordToAddr:
                
            if ( iDialogAllowed )
                {
                lm = LetUserSelectBestMatchL( *iLandmark );
                CleanupStack::PushL( lm );
                }
            else
                {
                // search and return just best match - closest POI
                if ( iLandmark->GetPosition( loc ) != KErrNone )
                    {
                    User::Leave( KErrArgument );
                    }
                lm = iEngine.CoordToAddressLC( loc );
                }
            break;

        case EAddrToCoord:
            lm = iEngine.AddressToCoordLC( *iLandmark );
            break;

        case EPlainToCoord:
            lm = iEngine.AddressToCoordLC( *iPlainAddress );
            break;
            
        default:            
            User::Leave( KErrGeneral );
        }

    iCurrentService = ENone;    
    
    CompleteGeocodingRequestL( *lm );
    CleanupStack::PopAndDestroy( lm );

    LOG("MnRefProvider: FinishProcessingL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CPosLandmark* CMnrpGeocodingService::LetUserSelectBestMatchL( const CPosLandmark& aReference )
    {
    // 1. Find best matches
    
    RPointerArray<CPosLandmark> matches;
    CleanupStack::PushL( TCleanupItem( CleanupLandmarksArray, &matches ) );
    
    TLocality location;
    if ( aReference.GetPosition( location ) != KErrNone )
        {
        User::Leave( KErrArgument );
        }
    
    const TInt KMaxmMatches = 3;
    iEngine.BestCoordToAddressMatchesL( location, matches, KMaxmMatches );
    
    if ( matches.Count() < 1 )
        {
        User::Leave( KErrNotFound ); // cannot find any match
        }
    
    // 2. Start selection dialog
    
    TInt result = 1, selected = 0;
    if ( matches.Count() > 1 )
        {
        // prepare dialog
        const TInt KGranularity = 8;
        CDesCArraySeg *textArray = new (ELeave) CDesCArraySeg( KGranularity );
        CleanupStack::PushL( textArray );
        
        for ( TInt i = 0; i < matches.Count(); i++ )
            {
            HBufC* buf = HBufC::NewLC( KPosLmMaxTextFieldLength );
            TPtrC ptr( buf->Des() );
            
            matches[i]->GetPositionField( EPositionFieldLocationName, ptr );
            textArray->AppendL( ptr );

            CleanupStack::PopAndDestroy( buf );
            }
        
        // bring application foreground
        CMnrpAppUiBack* ui = (CMnrpAppUiBack*) CEikonEnv::Static()->AppUi();
        CleanupStack::PushL( TCleanupItem( CleanupForeground, ui ) );
        ui->MoveForeground();

        CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog( &selected );
        dialog->PrepareLC( R_MNREFGEOCODER_LIST_QUERY );

        dialog->SetItemTextArray( textArray );
        dialog->SetOwnershipType( ELbmOwnsItemArray );

        // pop up dialog
        result = dialog->RunLD();

        CleanupStack::PopAndDestroy(); // move background
        CleanupStack::Pop( textArray );
        }

    // 3. Return result
    CPosLandmark* lm = NULL;
    if ( result )
        {
        lm = matches[selected];
        matches.Remove( selected );
        }
    else
        {
        User::Leave( KErrCancel ); 
        // user cancelled geocoding selection dialog
        // it automatically cancels geocoding request
        }

    CleanupStack::PopAndDestroy(); // matches
    return lm;
    }

// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::HandleFindAddressByCoordinateL(
    const CPosLandmark& aLandmark)
    {
    LeaveIfBusyL();

    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL(aLandmark);
    
    iCurrentService = ECoordToAddr;
    iDialogAllowed = !( Options() & CMnGeocoder::EOptionDialogDisabled );
    StartProcessingL();
    }
        
// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::HandleFindCoordinateByAddressL(
    const CPosLandmark& aLandmark)
    {
    LeaveIfBusyL();

    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL( aLandmark );
    
    iCurrentService = EAddrToCoord;
    iDialogAllowed = !( Options() & CMnGeocoder::EOptionDialogDisabled );
    StartProcessingL();
    }

// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::HandleFindCoordinateByAddressL(
    const TDesC& aAddress)
    {
    LeaveIfBusyL();

    delete iPlainAddress;
    iPlainAddress = NULL;
    iPlainAddress = aAddress.AllocL();
    
    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL();
    
    iCurrentService = EPlainToCoord;
    iDialogAllowed = !( Options() & CMnGeocoder::EOptionDialogDisabled );
    StartProcessingL();
    }

// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpGeocodingService::DoCancel()
    {
    iProcessor->Cancel();
    iCurrentService = ENone;
    }
