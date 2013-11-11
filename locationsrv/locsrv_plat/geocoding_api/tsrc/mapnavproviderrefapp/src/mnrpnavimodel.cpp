/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpNaviModel class implementation
*
*/


#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>

#include <lbs.h>
#include <lbsposition.h>
#include <lbspositioninfo.h>

#include <epos_cposlandmark.h>

#include "mnrputils.h"
#include "mnrpengine.h"
#include "mnrpnavimodel.h"

const TInt KUpdateInterval = 1 * 1000 * 1000; // 1 sec
const TInt KUpdateTimeout = 10 * 1000 * 1000; // 10 sec

_LIT( KNotDefined, "<n/a>" );
_LIT( KDestination, "destination" );
_LIT( KPosition, "position" );
_LIT( KDistance, "distance" );
_LIT( KBearing, "bearing" );
_LIT( KDestinationLat, "dest.lat." );
_LIT( KDestinationLon, "dest.lon." );
_LIT( KPositionLat, "pos.lat." );
_LIT( KPositionLon, "pos.lon." );
_LIT( KSpeed, "speed" );
_LIT( KHeading, "heading" );
_LIT( KMagHeading, "m.heading" );
_LIT( KCourse, "course" );
_LIT( KMagCourse, "m.course" );

// ========================== MEMBER CLASS' FUNCTIONS ==========================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviModel::CListItem* CMnrpNaviModel::CListItem::NewL( 
    const TDesC& aItemHeader, 
    const TDesC& aItemValue )
    {
    CListItem* self = new (ELeave) CListItem;
    CleanupStack::PushL( self );
    self->iHeader = aItemHeader.AllocL();
    self->iValue = aItemValue.AllocL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviModel::CListItem::~CListItem()
    {
    delete iHeader;
    delete iValue;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviModel::CMnrpNaviModel( MNaviModelObserver& aObserver, CMnrpEngine& aEngine )
    : iEngine( aEngine ), iObserver( aObserver )
    { 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviModel::~CMnrpNaviModel()
    {
    delete iPositionRequest;
    delete iDestination;
    iListItems.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviModel* CMnrpNaviModel::NewL( MNaviModelObserver& aObserver, CMnrpEngine& aEngine  )
    {
    CMnrpNaviModel* self = new (ELeave) CMnrpNaviModel( aObserver, aEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviModel::ConstructL()
    {
    _LIT( KRequestorName, "MnRefProvider" );
    iPositionRequest = CMnrpPositionRequest::NewL( KRequestorName, *this );
    
    TPositionUpdateOptions options;
    options.SetUpdateInterval( TTimeIntervalMicroSeconds( KUpdateInterval ) );
    options.SetUpdateTimeOut( TTimeIntervalMicroSeconds( KUpdateTimeout ) );
    iPositionRequest->SetOptionsL( options );
    
    iPositionRequest->FetchNewPosition();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviModel::SetDestinationL( const CPosLandmark& aNewDestination )
    {
    delete iDestination;
    iDestination = NULL;
    iDestination = CPosLandmark::NewL( aNewDestination );
    
    iObserver.HandleModelUpdateL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TArray<CMnrpNaviModel::CListItem*> CMnrpNaviModel::ListItemsL()
    {
    iListItems.ResetAndDestroy();
    
    TBuf<64> buf;
    TPosition position;
    TLocality destination;

    iPosInfo.GetPosition( position );

    if ( !iDestination || iDestination->GetPosition( destination ) )
        {
        iListItems.AppendL( CListItem::NewL( KDestination, KNotDefined ) );
        }
    else
        {
        TPtrC name;
        iDestination->GetLandmarkName( name );
        HBufC* printableName = MnrpUtils::PrintableStringLC( name );
        iListItems.AppendL( CListItem::NewL( KDestination, *printableName ) );
        CleanupStack::PopAndDestroy( printableName );

        TBool validPosition = ( !Math::IsNaN( position.Latitude() ) && 
                                !Math::IsNaN( position.Longitude() ) );

        if ( validPosition )
            {
            TReal32 distance, bearing;
            if ( position.Distance( destination, distance ) == KErrNone )
                {
                MnrpUtils::DistanceToString( distance, buf );
                iListItems.AppendL( CListItem::NewL( KDistance, buf ) );
                }
            else
                {
                iListItems.AppendL( CListItem::NewL( KDistance, KNotDefined ) );
                }
                
            if ( position.BearingTo( destination, bearing ) == KErrNone )
                {
                MnrpUtils::AngleToString( bearing, buf );
                iListItems.AppendL( CListItem::NewL( KBearing, buf ) );
                }
            else
                {
                iListItems.AppendL( CListItem::NewL( KBearing, KNotDefined ) );
                }

            MnrpUtils::SpeedToString( iPositionRequest->Speed(), buf );
            iListItems.AppendL( CListItem::NewL( KSpeed, buf ) );

            MnrpUtils::AngleToString( iPositionRequest->Course(), buf );
            iListItems.AppendL( CListItem::NewL( KCourse, buf ) );

            MnrpUtils::AngleToString( iPositionRequest->Heading(), buf );
            iListItems.AppendL( CListItem::NewL( KHeading, buf ) );

            MnrpUtils::CoordinateToString( position.Latitude(), buf, ETrue, ETrue );
            iListItems.AppendL( CListItem::NewL( KPositionLat, buf ) );

            MnrpUtils::CoordinateToString( position.Longitude(), buf, ETrue, ETrue );
            iListItems.AppendL( CListItem::NewL( KPositionLon, buf ) );
            }
        else
            {
            iListItems.AppendL( CListItem::NewL( KPosition, KNotDefined ) );
            }

        MnrpUtils::CoordinateToString( destination.Latitude(), buf, ETrue, ETrue );
        iListItems.AppendL( CListItem::NewL( KDestinationLat, buf ) );

        MnrpUtils::CoordinateToString( destination.Longitude(), buf, ETrue, ETrue );
        iListItems.AppendL( CListItem::NewL( KDestinationLon, buf ) );

        if ( validPosition )
            {
            MnrpUtils::AngleToString( iPositionRequest->MagneticCourse(), buf );
            iListItems.AppendL( CListItem::NewL( KMagCourse, buf ) );

            MnrpUtils::AngleToString( iPositionRequest->MagneticHeading(), buf );
            iListItems.AppendL( CListItem::NewL( KMagHeading, buf ) );
            }
        }
   
    return iListItems.Array();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviModel::HandlePositionRequestCompletedL( TInt aError )
    {
    if ( !aError )
        {
        iPositionRequest->GetPosition( iPosInfo );
        TPosition position;
        iPosInfo.GetPosition( position );

        iObserver.HandleModelUpdateL();
        }
    iPositionRequest->FetchNewPosition();
    }
