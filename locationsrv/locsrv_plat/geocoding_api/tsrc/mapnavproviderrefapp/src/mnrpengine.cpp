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
* Description:  CMnrpEngine class implementation
*
*/


#include <coemain.h>
#include <aknlists.h>
#include <barsread2.h>
#include <barsc2.h> 

#include <lbsposition.h>
#include <lbspositioninfo.h>

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>

#include <mnrefproviderengine.rsg>

#include "../inc/debug.h"
#include "mnrpengine.h"

_LIT( KCitiesResourceFile, "\\resource\\mnrefproviderengine.rsc");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpEngine::CMnrpEngine()
    { 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpEngine::~CMnrpEngine()
    {
    iPlaces.ResetAndDestroy();
    iFileSession.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpEngine* CMnrpEngine::NewL()
    {
    CMnrpEngine* self = new (ELeave) CMnrpEngine();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpEngine::ConstructL()
    {
    User::LeaveIfError( iFileSession.Connect() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TArray<CPosLandmark*> CMnrpEngine::KnownPlacesL()
    {
    if ( iPlaces.Count() == 0 )
        {
        TRAP_IGNORE( LoadKnownPlacesL() );
        }

    return iPlaces.Array();        
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnrpEngine::AddressToCoordLC( const CPosLandmark& aAddress )
    {
    CPosLandmark* lm = AddressToCoordL( aAddress );
    if ( lm )
        {
        CleanupStack::PushL( lm );
        return lm;
        }
    else
        {
        User::Leave( KErrNotFound );
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnrpEngine::AddressToCoordL( const CPosLandmark& aAddress )
    {
    TPtrC city, country;
    aAddress.GetPositionField( EPositionFieldCity, city );
    
    TArray<CPosLandmark*> pois = KnownPlacesL();
    
    if ( city.Length() )
        {
        for ( TInt i = 0; i < pois.Count(); i++ )
            {
            TPtrC poiCity;
            pois[i]->GetPositionField( EPositionFieldCity, poiCity );
            if ( poiCity.CompareF( city ) == 0 )
                {
                return CPosLandmark::NewL( *pois[i] );
                }
            }
        }
    
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnrpEngine::AddressToCoordLC( const TDesC& aAddress )
    {
    CPosLandmark* lm = AddressToCoordL( aAddress );
    if ( lm )
        {
        CleanupStack::PushL( lm );
        return lm;
        }
    else
        {
        User::Leave( KErrNotFound );
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnrpEngine::AddressToCoordL( const TDesC& aAddress )
    {
    TArray<CPosLandmark*> pois = KnownPlacesL();

    if ( aAddress.Length() )
        {
        for ( TInt i = 0; i < pois.Count(); i++ )
            {
            TPtrC city, country;
            pois[i]->GetPositionField( EPositionFieldCity, city );
            pois[i]->GetPositionField( EPositionFieldCountry, country );
            
            if ( aAddress.FindF( city ) >= 0 )
                {
                return CPosLandmark::NewL( *pois[i] );
                }
            }
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpEngine::DistanceBetweenLandmarks( 
    const CPosLandmark& aFrom,
    const CPosLandmark& aTo )
    {
    TRealX nan;
    nan.SetNaN();
    TReal32 distance = nan;
    
    TLocality locFrom, locTo;

    if ( aFrom.GetPosition( locFrom ) == KErrNone &&
         aTo.GetPosition( locTo ) == KErrNone )
        {
        TReal32 dist;
        if ( locFrom.Distance( locTo, dist ) == KErrNone )
            {
            distance = dist;
            }
        }
    return distance;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpEngine::FindClosestPoisL( 
    const CPosLandmark& aReference,
    RArray<CMnrpEngine::TDistanceToPoi>& aNeighbourPois,
    TInt aMaxMatches )
    {
    aNeighbourPois.Reset();
    TArray<CPosLandmark*> pois = KnownPlacesL();

    // calc distances to known places and find out closest ones
    for ( TInt i = 0; i < pois.Count(); i++ )
        {
        CPosLandmark* poi = pois[i];

        TReal32 distance = DistanceBetweenLandmarks( aReference, *poi );
        
        if ( Math::IsNaN( distance ) )
            {
            continue;
            }
        
        TDistanceToPoi newDistToPoi;
        newDistToPoi.iPoiIndex = i;
        newDistToPoi.iDistance = distance;

        for ( TInt k = 0; k < aNeighbourPois.Count(); k++ )
            {
            TDistanceToPoi distToPoi = aNeighbourPois[k];
            if ( distance < distToPoi.iDistance )
                {
                // current POI is closer to reference than current neighbour
                aNeighbourPois.InsertL( newDistToPoi, k );
                break;
                }
            }

        if ( aNeighbourPois.Count() < aMaxMatches )
            {
            aNeighbourPois.AppendL( newDistToPoi );
            }

        if ( aNeighbourPois.Count() > aMaxMatches )
            {
            aNeighbourPois.Remove( aNeighbourPois.Count() - 1 ); // remove last
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLandmark* CMnrpEngine::CreateCloseLandmarkLC( 
    const CPosLandmark& aReference,
    TReal32 aDistance )
    {
    CPosLandmark* lm = CPosLandmark::NewLC( aReference );
    TPtrC oldName;
    lm->GetLandmarkName( oldName );
    
    // create street in the form of "50 km to <POI name>"
    const TInt KDistValueLen = 5; // "40000"
    const TInt KAdditionalChars = 7; // " km to "

    _LIT( KLocationNameFormat, "%d m to %S" );
    _LIT( KLocationNameFormatKm, "%d km to %S" );

    HBufC* locName = HBufC::NewLC( oldName.Length() + KDistValueLen + KAdditionalChars );

    if ( aDistance < 1000 )
        {
        locName->Des().Format( KLocationNameFormat, TInt( aDistance ), &oldName );
        }
    else
        {
        locName->Des().Format( KLocationNameFormatKm, TInt( aDistance / 1000 ), &oldName );
        }

    lm->SetPositionFieldL( EPositionFieldLocationName, *locName );
    lm->SetPositionFieldL( EPositionFieldStreet, *locName );
   
    CleanupStack::PopAndDestroy( locName );
    return lm; // left in stack
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnrpEngine::CoordToAddressLC( const TCoordinate& aLocation )
    {
    TArray<CPosLandmark*> pois = KnownPlacesL();

    // find closest POI
    RArray<TDistanceToPoi> matches;
	CleanupClosePushL( matches );
    
    CPosLandmark* temp = CPosLandmark::NewLC();
    TLocality loc( aLocation, 0 ); // accuracy is not used by this implementation
    temp->SetPositionL( loc );
    FindClosestPoisL( *temp, matches, 1 ); // only one match needed
    CleanupStack::PopAndDestroy( temp );
    
    if ( matches.Count() < 1 )
		{
		User::Leave( KErrNotFound );
		}

    TDistanceToPoi distToPoi = matches[0];
	CleanupStack::PopAndDestroy( &matches );

    CPosLandmark* lm = CreateCloseLandmarkLC(
        *pois[distToPoi.iPoiIndex],
        distToPoi.iDistance );

    return lm; // left in stack
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpEngine::BestCoordToAddressMatchesL( 
    const TCoordinate& aLocation,
    RPointerArray<CPosLandmark>& aMatches,
    TInt aMaxMatches )
    {
    TArray<CPosLandmark*> pois = KnownPlacesL();

    // find closest POIs
    RArray<TDistanceToPoi> matches;
    CleanupClosePushL( matches );
    
    CPosLandmark* temp = CPosLandmark::NewLC();
    TLocality loc( aLocation, 0 ); // accuracy is not used by this implementation
    temp->SetPositionL( loc );
    FindClosestPoisL( *temp, matches, aMaxMatches );
    CleanupStack::PopAndDestroy( temp );
    
    for ( TInt i = 0; i < matches.Count(); i++ )
        {
        TDistanceToPoi distToPoi = matches[i];
        
        CPosLandmark* lm = CreateCloseLandmarkLC( 
            *pois[distToPoi.iPoiIndex], 
            distToPoi.iDistance );
            
        aMatches.AppendL( lm );    
        CleanupStack::Pop( lm );
        }
    CleanupStack::PopAndDestroy( &matches );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpEngine::FindResourceFileL( 
    const TDesC& aFilePathAndName,
    TFileName& aRscFile )
    {
    TParse parse;
    parse.Set( aFilePathAndName, NULL, NULL );
    
    TFindFile finder( iFileSession );
    if ( finder.FindByDir( parse.NameAndExt(), parse.Path() ) == KErrNone )
        {
        aRscFile.Copy( finder.File() );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C RFs& CMnrpEngine::Fs() 
    { 
    return iFileSession;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpEngine::LoadKnownPlacesL()
    {
    TFileName resourceFile;
    FindResourceFileL( KCitiesResourceFile(), resourceFile );

    CResourceFile* resFile = CResourceFile::NewLC( iFileSession, resourceFile, 0, 0 );
    resFile->ConfirmSignatureL( 0 );

    RResourceReader reader;

    reader.OpenLC( resFile, R_MNREFPROVIDERENGINE_PLACES );
    ReadPlacesResourceL( reader );
    CleanupStack::PopAndDestroy( &reader );
    
    CleanupStack::PopAndDestroy( resFile );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpEngine::ReadPlacesResourceL( RResourceReader& aReader )
    {
    TInt size = aReader.ReadInt16L();

    for ( TInt i = 0; i < size; i++ )
        {
        HBufC* city = aReader.ReadHBufCL();
        CleanupStack::PushL( city );
        HBufC* country = aReader.ReadHBufCL();
        CleanupStack::PushL( country );

        TReal64 lat = aReader.ReadReal64L();
        TReal64 lon = aReader.ReadReal64L();
        
        CPosLandmark* lm = CPosLandmark::NewLC();
        lm->SetLandmarkNameL( *city );
        
        lm->SetPositionFieldL( EPositionFieldCity, *city );
        lm->SetPositionFieldL( EPositionFieldCountry, *country );
        
        TLocality coord;
        coord.SetCoordinate( lat, lon );
        lm->SetPositionL( coord );
        
        iPlaces.AppendL( lm );
        CleanupStack::Pop( lm );
        CleanupStack::PopAndDestroy( country );
        CleanupStack::PopAndDestroy( city );
        }
    }
