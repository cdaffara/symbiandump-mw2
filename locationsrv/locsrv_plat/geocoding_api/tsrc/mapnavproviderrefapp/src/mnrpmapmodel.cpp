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
* Description:  CMnrpMapModel class implementation
*
*/



#include <coemain.h>
#include <barsread.h>
#include <bautils.h>

#include <aknlists.h>
#include <akniconutils.h>
#include <aknwaitdialog.h>

#include <fbs.h>
#include <imageconversion.h>
#include <icl/imagedata.h>

#include <lbs.h>
#include <lbsposition.h>
#include <lbspositioninfo.h>

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>
#include <epos_cposlandmarkcategory.h>
#include <epos_cposlmcategorymanager.h>

#include "debug.h"
#include "mnrpengine.h"
#include "mnrputils.h"
#include "mnrpappserver.h"

#include "mnrpmapviewservice.h"
#ifdef RD_MAPNAV_BITMAP_ACCESS
#include "mnrpmapimageservice.h"
#endif
#include "mnrpmapmodel.h"

const TInt KUpdateInterval = 1 * 1000 * 1000; // 1 sec
const TInt KUpdateTimeout = 10 * 1000 * 1000; // 10 sec

const TInt KMaxX = 180;
const TInt KMinX = -180;
const TInt KMapWidth = KMaxX - KMinX;

const TInt KMaxY = 90;
const TInt KMinY = -90;
const TInt KMapHeight = KMaxY - KMinY;

const TReal KDegree = 1.0;
const TReal KMinute = KDegree / 60.0;
const TReal KSecond = KMinute / 60.0;
const TReal KMilliSecond = KSecond / 1000.0;

const TReal KMaxCell = 60 * KDegree;
const TReal KMinScale = 10 * KMilliSecond; // 0.01 second / 1 px

const TInt KBigCellsInView = 2;

const TInt KCellDividers[] = { 6, 2, 5 };
const TInt KNumCellDividers = 3;

const TReal KDefaultRadius = 1; // 1 degree
const TReal KRadiusBorderFactor = 1.1; // 110%

const TReal KEarthEquator = 40075000;
//const TReal KEarthRadius = 6371010;

const TInt KLmTextOffset = 3;
const TInt KLmTextShadowOffset = 1;

const TInt KGridPenSize( 1 );

// ============================ LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPoint AbsoluteToImage( TRealPoint aAbsolute, TReal aImageToAbsoluteScale )
    {
    TInt s = ( KMapWidth / 2 + aAbsolute.iX ) / aImageToAbsoluteScale;
    TInt t = ( KMapHeight / 2 - aAbsolute.iY ) / aImageToAbsoluteScale;

    return TPoint( s , t );
    }

// ============================ MEMBER CLASSES' FUNCTIONS ===============================

// -----------------------------------------------------------------------------
//  TRealPoint implementation
// -----------------------------------------------------------------------------
//

TRealPoint::TRealPoint()
    {
    TRealX nan;
    nan.SetNaN();

    iX = nan;
    iY = nan;
    }

TRealPoint::TRealPoint( TReal aX, TReal aY )
    : iX( aX ), iY( aY )
    {
    }

TRealPoint::TRealPoint( TCoordinate aCoordinate )
    : iX( aCoordinate.Longitude() ), iY( aCoordinate.Latitude() )
    {
    }

TCoordinate TRealPoint::Coordinate()
    {
    return TCoordinate( iY, iX );
    }

TRealPoint TRealPoint::operator+(
    const TRealPoint& aRight )
    {
    return TRealPoint( this->iX + aRight.iX, this->iY + aRight.iY );
    }

TRealPoint TRealPoint::operator-(
    const TRealPoint& aRight )
    {
    return TRealPoint( this->iX - aRight.iX, this->iY - aRight.iY );
    }

TRealPoint TRealPoint::operator*( const TReal& aRight )
    {
    return TRealPoint( this->iX * aRight, this->iY * aRight );
    }

TRealPoint TRealPoint::operator/( const TReal& aRight )
    {
    return TRealPoint( this->iX / aRight, this->iY / aRight );
    }

void TRealPoint::Set( TReal aX, TReal aY )
    {
    iX = aX;
    iY = aY;
    }

TBool TRealPoint::IsValid()
    {
    return ( !Math::IsNaN( iX ) && !Math::IsNaN( iY ) );
    }

// -----------------------------------------------------------------------------
//  CDrawItem implementation
// -----------------------------------------------------------------------------
//
CMnrpMapModel::CDrawItem* CMnrpMapModel::CDrawItem::NewLC(
    const CPosLandmark& aLm )
    {
    CDrawItem* self = new (ELeave) CDrawItem;
    CleanupStack::PushL( self );

    TPtrC name;
    aLm.GetLandmarkName( name );
    self->iText = MnrpUtils::PrintableStringLC( name );
    CleanupStack::Pop( self->iText );

    TLocality loc;
    if ( KErrNone == aLm.GetPosition( loc ) )
        {
        self->SetAbsolutePosition( loc );
        }

    TPtrC iconFile;
    TInt iconIndex, maskIndex;
    if ( KErrNone == aLm.GetIcon( iconFile, iconIndex, maskIndex ) )
        {
        AknIconUtils::CreateIconL( self->iBitmap, self->iMask, iconFile, iconIndex, maskIndex );
        AknIconUtils::PreserveIconData( self->iBitmap );
        }

    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::CDrawItem* CMnrpMapModel::CDrawItem::NewLC(
    const CPosLandmark& aLm,
    CPosLmCategoryManager& aCatman )
    {
    CDrawItem* self = NewLC( aLm );

    if ( !self->iBitmap )
        {
        // try icon from category
        RArray<TPosLmItemId> cats;
        CleanupClosePushL( cats );
        aLm.GetCategoriesL( cats );

        if ( cats.Count() == 1 )
            {
            CPosLandmarkCategory* cat = aCatman.ReadCategoryLC( cats[0] );

            TPtrC iconFile;
            TInt iconIndex, maskIndex;
            if ( KErrNone == cat->GetIcon( iconFile, iconIndex, maskIndex ) )
                {
                AknIconUtils::CreateIconL( self->iBitmap, self->iMask, iconFile, iconIndex, maskIndex );
                AknIconUtils::PreserveIconData( self->iBitmap );
                }

            CleanupStack::PopAndDestroy( cat );
            }
        CleanupStack::PopAndDestroy( &cats );
        }

    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::CDrawItem::SetAbsolutePosition( const TLocality& aLocality )
    {
    iAbsolutePosition = TRealPoint( aLocality.Longitude(), aLocality.Latitude() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::CDrawItem::~CDrawItem()
    {
    if ( iBitmap )
        {
        AknIconUtils::DestroyIconData( iBitmap );
        }
    delete iBitmap;
    delete iMask;
    delete iText;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::CMnrpMapModel(
    MMapModelObserver& aObserver,
    CMnrpEngine& aEngine )
    :
    iObserver( &aObserver ), iEngine( aEngine )
    {
    iScale = 1;
    iViewOrigin.Set( 0, 0 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::CMnrpMapModel( CMnrpEngine& aEngine )
    : iEngine( aEngine )
    {
    iScale = 1;
    iViewOrigin.Set( 0, 0 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::~CMnrpMapModel()
    {
    delete iPositionRequest;
    iDrawItems.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpMapModel* CMnrpMapModel::NewL(
    MMapModelObserver& aObserver,
    CMnrpEngine& aEngine )
    {
    CMnrpMapModel* self = new (ELeave) CMnrpMapModel( aObserver, aEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpMapModel* CMnrpMapModel::NewL( CMnrpEngine& aEngine )
    {
    CMnrpMapModel* self = new (ELeave) CMnrpMapModel( aEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::ConstructL()
    {
    _LIT( KRequestorName, "MnRefProvider" );
    iPositionRequest = CMnrpPositionRequest::NewL( KRequestorName, *this );

    TPositionUpdateOptions options;
    options.SetUpdateInterval( TTimeIntervalMicroSeconds( KUpdateInterval ) );
    options.SetUpdateTimeOut( TTimeIntervalMicroSeconds( KUpdateTimeout ) );
    iPositionRequest->SetOptionsL( options );

    AddCitiesL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnrpMapModel::CheckDrawItemHasPositionL( CDrawItem& aItem, const CPosLandmark& aLandmark )
	{
    if ( !aItem.AbsolutePosition().IsValid() )
        {
        // try to find coordinate by address
        CPosLandmark* poi = iEngine.AddressToCoordL( aLandmark );
        if ( poi )
        	{
            TLocality pos;
            poi->GetPosition( pos );
            aItem.SetAbsolutePosition( pos );
        	}
    	delete poi;
        }
    
    return aItem.AbsolutePosition().IsValid();
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::UpdateModelL( CMnrpMapViewService& aService )
    {
    ASSERT( iObserver );

    iDrawItems.ResetAndDestroy();

    iShowCurrentLocation =
        aService.CurrentLocationOption() != CMnMapView::ECurrentLocationDisabled;

    if ( iShowCurrentLocation )
        {
        iPositionRequest->FetchNewPosition();
        }

    // Add client-defined marks
    
    iNumIgnoredLandmarks = 0;
    TRealPoint min, max;

    // add non-linked landmarks
    for ( TInt i = 0; i < aService.LandmarksToShow().Count(); i++)
        {
        const CPosLandmark* lm = aService.LandmarksToShow()[i];
        
        CDrawItem* item = CDrawItem::NewLC( *lm );
        if ( CheckDrawItemHasPositionL( *item, *lm ) )
        	{
        	iDrawItems.AppendL( item );
            CleanupStack::Pop( item );
            AdjustBoundingBox( *item, min, max );
        	}
        else
        	{
            CleanupStack::PopAndDestroy( item );
            iNumIgnoredLandmarks++;
        	}
        }

    // add linked landmarks
    for ( TInt db = 0; db < aService.LandmarksToShowDatabases().Count(); db++ )
        {
        const HBufC* uri = aService.LandmarksToShowDatabases()[db];

        LOG1("MnRefProvider::LinkedLandmarks: database (%S)", uri );

        CPosLandmarkDatabase* lmdb = CPosLandmarkDatabase::OpenL( *uri );
        CleanupStack::PushL( lmdb );

        if ( lmdb->IsInitializingNeeded() )
            {
            ExecuteAndDeleteLD( lmdb->InitializeL() );
            }

        CPosLmCategoryManager* catman = CPosLmCategoryManager::NewL( *lmdb );
        CleanupStack::PushL( catman );

        for ( TInt i = 0; i < aService.LinkedLandmarksToShow(db).Count(); i++)
            {
            TPosLmItemId id = aService.LinkedLandmarksToShow(db)[i];
            LOG1("MnRefProvider::LinkedLandmarks: id (%d)", id );

            CPosLandmark* lm = lmdb->ReadLandmarkLC( id );

            CDrawItem* item = CDrawItem::NewLC( *lm, *catman );
            if ( CheckDrawItemHasPositionL( *item, *lm ) )
            	{
            	iDrawItems.AppendL( item );
                CleanupStack::Pop( item );
                AdjustBoundingBox( *item, min, max );
            	}
            else
            	{
                CleanupStack::PopAndDestroy( item );
                iNumIgnoredLandmarks++;
            	}

            CleanupStack::PopAndDestroy( lm );
            }

        CleanupStack::PopAndDestroy( catman );
        CleanupStack::PopAndDestroy( lmdb );
        }

    // Add predefined places
    AddCitiesL();

    LOG4("MnRefProvider::BoundingBox: %f, %f, %f, %f", min.iX, min.iY, max.iX, max.iY );

    // set origin
    iUseCurrentLocationAsOrigin = EFalse;
    if ( aService.IsAreaCentralPointSet() )
        {
        TCoordinate center;
        aService.GetAreaCentralPoint( center );
        iViewOrigin.Set( center.Longitude(), center.Latitude() );
        }
    else if ( !Math::IsNaN( max.iX ) ) // others are also valid then
        {
        iViewOrigin.Set( ( max.iX + min.iX ) / 2, ( max.iY + min.iY ) / 2 );
        }
    else if ( iShowCurrentLocation )
        {
        // get current location and use it as center point
        // this implementation defers location acquiring
        iUseCurrentLocationAsOrigin = ETrue;
        // FetchCurrentLocation();
        iViewOrigin.Set( 0, 0 );
        }
    else
        {
        iViewOrigin.Set( 0, 0 );
        }

    LOG2("MnRefProvider: viewOrigin (%f, %f)", iViewOrigin.iX, iViewOrigin.iY);

    // set scale
    TReal desiredScale = 0;
    TReal radius = 0;

    if ( !Math::IsNaN( aService.AreaRadius() ) )
        {
         // Radius setting is easy in this implementation
         // because map is rectangular just convert meters to degrees
        radius = Abs( aService.AreaRadius() ) / KEarthEquator * 360;
        LOG1("MnRefProvider: user radius (%f)", radius );
        }
    else if ( !Math::IsNaN( max.iX ) ) // others are also valid then
        {
        // get radius from bounding box
        radius = Max( Abs( max.iX - min.iX ), Abs( max.iY - min.iY ) );
        LOG2("MnRefProvider: bounding radius (%f), with border (%f)", radius, radius * KRadiusBorderFactor );
        radius *= KRadiusBorderFactor;
        }
    else
        {
        // default radius
        }

    if ( radius <= 0 )
        {
        radius = KDefaultRadius;
        LOG1("MnRefProvider: default radius (%f)", radius );
        }

    desiredScale = radius / TReal( Max( iScreenSize.iWidth, iScreenSize.iHeight ) );

    LOG3("MnRefProvider: desired scale (%f), min (%f), max(%f)",
        desiredScale, KMinScale, MaxScale() );

    iScale = Max( Min( desiredScale, MaxScale() ), KMinScale );
    CheckBorders();

    LOG1("MnRefProvider: scale (%f)", iScale );

    iObserver->HandleModelUpdateL();
    }


#ifdef RD_MAPNAV_BITMAP_ACCESS
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::UpdateModelL( const CMnrpMapImageService& aService )
    {
    iDrawItems.ResetAndDestroy();

    iShowCurrentLocation =
        aService.ShowOptions() & CMnMapImage::EShowOptionCurrentLocation;

    if ( iShowCurrentLocation )
        {
        iPositionRequest->FetchNewPosition();
        }

    if ( aService.ShowOptions() & CMnMapImage::EShowOptionLandmarks)
        {

        // Add user-defined marks
        TRealPoint min, max;

        // add default landmarks
        CPosLandmarkDatabase* lmdb = CPosLandmarkDatabase::OpenL();
        CleanupStack::PushL( lmdb );

        if ( lmdb->IsInitializingNeeded() )
            {
            ExecuteAndDeleteLD( lmdb->InitializeL() );
            }

        CPosLmCategoryManager* catman = CPosLmCategoryManager::NewL( *lmdb );
        CleanupStack::PushL( catman );

        CPosLmItemIterator* iter = lmdb->LandmarkIteratorL();
        CleanupStack::PushL( iter );

        TInt count = iter->NumOfItemsL();
        for ( TInt i = 0; i < count; i++)
            {
            TPosLmItemId id = iter->NextL();

            CPosLandmark* lm = lmdb->ReadLandmarkLC( id );

            CDrawItem* item = CDrawItem::NewLC( *lm, *catman );
            if ( CheckDrawItemHasPositionL( *item, *lm ) )
            	{
            	iDrawItems.AppendL( item );
                CleanupStack::Pop( item );
                AdjustBoundingBox( *item, min, max );
            	}
            else
            	{
                CleanupStack::PopAndDestroy( item );
            	}

            CleanupStack::PopAndDestroy( lm );
            }
        CleanupStack::PopAndDestroy( iter );
        CleanupStack::PopAndDestroy( catman );
        CleanupStack::PopAndDestroy( lmdb );

        LOG4("MnRefProvider::BoundingBox: %f, %f, %f, %f", min.iX, min.iY, max.iX, max.iY );
        }

    if ( aService.ShowOptions() & CMnMapImage::EShowOptionPois )
        {
        // Add predefined places
        AddCitiesL();
        }

    TMnMapImageParams params = aService.MapImageParams();

    UpdateModel( params );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::UpdateModel( const TMnMapImageParams& aParams )
    {
    SetScreenSize( aParams.Size() );

    TCoordinate center;
    aParams.GetCenterPoint( center );
    iViewOrigin.Set( center.Longitude(), center.Latitude() );

    LOG2("MnRefProvider: viewOrigin (%f, %f)", iViewOrigin.iX, iViewOrigin.iY);

    // set scale
    TReal desiredScale = 0;
    TReal32 width, height;
    aParams.GetVisibleArea( width, height );
    TReal radius = Max( width, height );

    if ( radius <= 0 )
        {
        radius = KDefaultRadius;
        LOG1("MnRefProvider: default radius (%f)", radius );
        }

    desiredScale = radius / TReal( Max( iScreenSize.iWidth, iScreenSize.iHeight ) );

    LOG3("MnRefProvider: desired scale (%f), min (%f), max(%f)",
        desiredScale, KMinScale, MaxScale() );

    iScale = Max( Min( desiredScale, MaxScale() ), KMinScale );
    CheckBorders();

    LOG1("MnRefProvider: scale (%f)", iScale );
    }
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::AddCitiesL()
    {
    TArray<CPosLandmark*> places = iEngine.KnownPlacesL();
    for ( TInt i = 0; i < places.Count(); i++)
        {
        CPosLandmark* lm = places[i];
        CDrawItem* item = CDrawItem::NewLC( *lm );
        item->iOwnPoi = ETrue;
        iDrawItems.AppendL( item );
        CleanupStack::Pop( item );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::GetGrid( CMnrpMapModel::TGrid& aGrid ) const
    {
    TReal angle = Min( Width(), Height() );

    TReal minAngle = Min( iScreenSize.iWidth, iScreenSize.iHeight ) * KMinScale;

    if ( angle > minAngle )
        {
        // 1. Find nice grid scale

        TInt dividerIndex = 0;
        TReal cellSize = KMaxCell;

        while ( cellSize * KBigCellsInView > angle )
            {
            cellSize /= KCellDividers[dividerIndex];

            if ( ++dividerIndex == KNumCellDividers )
                {
                dividerIndex = 0;
                }
            }

        aGrid.iAbsoluteGridStep = cellSize;

        // 2. Find closest grid point

        TReal x = TInt( iViewOrigin.iX / cellSize ) * cellSize;
        TReal y = TInt( iViewOrigin.iY / cellSize ) * cellSize;

        aGrid.iAbsoluteReference = TRealPoint( x, y ).Coordinate();
        }
    else
        {
        aGrid.iAbsoluteReference = TCoordinate( 0, 0 );
        aGrid.iAbsoluteGridStep = minAngle / KBigCellsInView;
        }

    // convert to screen coordinates
    aGrid.iReference = AbsoluteToScreen( aGrid.iAbsoluteReference );
    aGrid.iGridStep = AbsoluteToScreen( aGrid.iAbsoluteGridStep );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::Zoom( TReal aZoomRate )
    {
    iScale *= aZoomRate;
    CheckBorders();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::GoCenter()
    {
    iViewOrigin.Set( 0, 0 );
    CheckBorders();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::MoveTo( TCoordinate aTarget )
    {
    iViewOrigin = aTarget;
    CheckBorders();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::CheckBorders()
    {
    if ( Height() > KMapHeight || Width() > KMapWidth )
        {
        iScale = MaxScale();
        }
    if ( iScale < KMinScale )
        {
        iScale = KMinScale;
        }
    if ( Left() < KMinX )
        {
        iViewOrigin.iX = KMinX + Width() / 2;
        }
    if ( Right() > KMaxX )
        {
        iViewOrigin.iX = KMaxX - Width() / 2;
        }
    if ( Top() > KMaxY )
        {
        iViewOrigin.iY = KMaxY - Height() / 2;
        }
    if ( Bottom() < KMinY )
        {
        iViewOrigin.iY = KMinY + Height() / 2;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Left() const
    {
    return iViewOrigin.iX - Width() / 2;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Right() const
    {
    return iViewOrigin.iX + Width() / 2;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Top() const
    {
    return iViewOrigin.iY + Height() / 2;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Bottom() const
    {
    return iViewOrigin.iY - Height() / 2;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Width() const
    {
    return TReal( iScreenSize.iWidth ) * iScale;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::Height() const
    {
    return TReal( iScreenSize.iHeight ) * iScale;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TCoordinate CMnrpMapModel::Center() const
    {
    return TCoordinate( iViewOrigin.iY, iViewOrigin.iX );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TArray<CMnrpMapModel::CDrawItem*> CMnrpMapModel::Items() const
    {
    // adjust all items to screen coordinates
    for ( TInt i = 0; i < iDrawItems.Count(); i++ )
        {
        iDrawItems[i]->iScreenPosition = AbsoluteToScreen( iDrawItems[i]->iAbsolutePosition );
        }
    return iDrawItems.Array();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapModel::TCurrentLocation CMnrpMapModel::CurrentLocation() const
    {
    TCurrentLocation current;

    TPosition pos;
    iPosInfo.GetPosition( pos );

    current.iAbsoluteLocation = pos;

    if ( !Math::IsNaN( pos.Latitude() ) && !Math::IsNaN( pos.Longitude() ) )
        {
        current.iLocation = AbsoluteToScreen( current.iAbsoluteLocation );

        // calculate error radius in degrees and pixels
        if ( !Math::IsNaN( pos.HorizontalAccuracy() ) )
            {
            current.iAbsoluteErrorDegrees = ( pos.HorizontalAccuracy() / KEarthEquator ) * 360;
            current.iErrorRadius = current.iAbsoluteErrorDegrees / iScale;
            }
        else
            {
            current.iErrorRadius = 0;
            current.iAbsoluteErrorDegrees = 0;
            }

        current.iIsValid = ETrue;
        }
    else
        {
        current.iIsValid = EFalse;
        }

    return current;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::SetScreenSize( TSize aScreenSize )
    {
    __ASSERT_DEBUG( aScreenSize.iWidth >= 1 && aScreenSize.iHeight >= 1,
                    Panic( KErrGeneral ) );
    iScreenSize = aScreenSize;
    CheckBorders();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal CMnrpMapModel::MaxScale() const
    {
    TReal scaleByWidth = KMapWidth / TReal( iScreenSize.iWidth );
    TReal scaleByHeight = KMapHeight / TReal( iScreenSize.iHeight );
    return Min( scaleByWidth, scaleByHeight );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPoint CMnrpMapModel::AbsoluteToScreen( TRealPoint aAbsolutePoint ) const
    {
    TRealPoint realScreen = ( aAbsolutePoint - TRealPoint( Left(), Bottom() ) ) / iScale;
    return TPoint( realScreen.iX, iScreenSize.iHeight - realScreen.iY );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TPoint CMnrpMapModel::AbsoluteToScreen( TCoordinate aAbsolutePoint ) const
    {
    TRealPoint real( aAbsolutePoint.Longitude(), aAbsolutePoint.Latitude() );
    return AbsoluteToScreen( real );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnrpMapModel::AbsoluteToScreen( TReal aDistance ) const
    {
    return aDistance / iScale;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TCoordinate CMnrpMapModel::ScreenToAbsolute( TPoint aScreenPoint ) const
    {
    TReal xdiff = aScreenPoint.iX * iScale;
    TReal ydiff = aScreenPoint.iY * iScale;
    return TCoordinate( Top() - ydiff, Left() + xdiff );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal CMnrpMapModel::ScreenToAbsolute( TInt aDistance ) const
    {
    return aDistance * iScale;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::AdjustBoundingBox(
    CMnrpMapModel::CDrawItem& aItem,
    TRealPoint& aMin,
    TRealPoint& aMax )
    {
    // X
    if ( !Math::IsNaN( aItem.iAbsolutePosition.iX ) )
        {
        if ( Math::IsNaN( aMin.iX ) || aItem.iAbsolutePosition.iX < aMin.iX )
            {
            aMin.iX = aItem.iAbsolutePosition.iX;
            }
        if ( Math::IsNaN( aMax.iX ) || aItem.iAbsolutePosition.iX > aMax.iX )
            {
            aMax.iX = aItem.iAbsolutePosition.iX;
            }
        }

    // Y
    if ( !Math::IsNaN( aItem.iAbsolutePosition.iY ) )
        {
        if ( Math::IsNaN( aMin.iY ) || aItem.iAbsolutePosition.iY < aMin.iY )
            {
            aMin.iY = aItem.iAbsolutePosition.iY;
            }
        if ( Math::IsNaN( aMax.iY ) || aItem.iAbsolutePosition.iY > aMax.iY )
            {
            aMax.iY = aItem.iAbsolutePosition.iY;
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::HandlePositionRequestCompletedL( TInt aError )
    {
    if ( !aError )
        {
        iPositionRequest->GetPosition( iPosInfo );
        if ( iObserver )
            {
            iObserver->HandleModelUpdateL();
            }
        }
    if ( iShowCurrentLocation )
        {
        iPositionRequest->FetchNewPosition();
        }
    }

// =====================================================
// DRAWING METHODS
// =====================================================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpMapModel::RenderL(
    CFbsBitmap& aBitmap,
    TRect aTargetRect,
    CFont* aTextFont,
    CFont* aItemFont ) const
	{
	// create an off-screen device and context
	CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL( &aBitmap );
	CleanupStack::PushL( bitmapDevice );

	CFbsBitGc* gc = NULL;
	User::LeaveIfError( bitmapDevice->CreateContext( gc ) );
	CleanupStack::PushL( gc );

    gc->UseFont( aTextFont );

    // get data
    TGrid grid;
    GetGrid( grid );

    TBool isBackground = EFalse;
    // background
    if ( !isBackground )
        {
        // no map background, clear map area
        gc->Clear( aTargetRect );
        }

    gc->SetClippingRect( aTargetRect );
    gc->SetOrigin( aTargetRect.iTl );

    DrawGrid( *gc, aTargetRect.Size(), grid );

    gc->DiscardFont(); // text font
    gc->UseFont( aItemFont );

    // draw items
    DrawLandmarks( *gc, *aItemFont, aTargetRect.Size(), isBackground ); // invert shadow and text color
    DrawCurrentLocation( *gc, aTargetRect.Size() );

    // cleanup

    gc->DiscardFont(); // item font

	CleanupStack::PopAndDestroy( gc );
	CleanupStack::PopAndDestroy( bitmapDevice );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMnrpMapModel::NumIgnoredLandmarks()
	{
	return iNumIgnoredLandmarks;
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::DrawGrid(
    CFbsBitGc& aGc,
    const TSize& aBoxSize,
    CMnrpMapModel::TGrid& aGrid ) const
    {
    const TRgb KGridLineColor( 128, 128, 128 );

    aGc.SetPenSize( TSize( KGridPenSize, KGridPenSize ) );
    aGc.SetPenColor( KGridLineColor );

    if ( aGrid.iGridStep > 0 )
        {
        // 1. Vertical lines
        TInt x = aGrid.iReference.iX;
        // find leftmost vertical line
        while ( x > 0 )
        	{
        	x -= aGrid.iGridStep;
        	}
        x += aGrid.iGridStep;
        // draw lines selecting different line colors for major and minor lines
        while ( x < aBoxSize.iWidth )
            {
            DrawVerticalLine( aGc, x, aBoxSize );
            x += aGrid.iGridStep;
            }

        // 2. Horizontal lines
        TInt y = aGrid.iReference.iY;
        // find topmost vertical line
        while ( y > 0 )
        	{
        	y -= aGrid.iGridStep;
        	}
        y += aGrid.iGridStep;
        // draw lines selecting different line colors for major and minor lines
        while ( y < aBoxSize.iHeight )
            {
            DrawHorizontalLine( aGc, y, aBoxSize );
            y += aGrid.iGridStep;
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::DrawVerticalLine( CFbsBitGc& aGc, TInt aX, TSize aBoxSize ) const
    {
    aGc.DrawLine( TPoint( aX, 0 ), TPoint( aX, aBoxSize.iHeight ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::DrawHorizontalLine( CFbsBitGc& aGc, TInt aY, TSize aBoxSize ) const
    {
    aGc.DrawLine( TPoint( 0, aY ), TPoint( aBoxSize.iWidth, aY ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::DrawLandmarks(
    CFbsBitGc& aGc,
    CFont& aFont,
    const TSize& aBoxSize,
    TBool aSwitchColorAndShadow ) const
    {
    const TSize KItemMarkSize( 3, 3 ); // pixels
    const TSize KItemIconSize( 20, 20 ); // pixels

    const TRgb KPoiColor( 0, 0, 192 );
    const TRgb KPoiTextColor( 0, 0, 0 );
    const TRgb KPoiTextShadowColor( 224, 224, 224 );
    const TRgb KLmColor( 255, 0, 0 );
    const TRgb KLmTextColor( 255, 0, 0 );
    const TRgb KLmTextShadowColor( 224, 224, 224 );

    TArray<CMnrpMapModel::CDrawItem*> items = Items();

    RArray<TRect> textBoxes;

    for ( TInt i = 0; i < items.Count(); i++ )
        {
        const TPoint& pos = items[i]->Position();
        if ( pos.iX >= 0 && pos.iX < aBoxSize.iWidth &&
             pos.iY >= 0 && pos.iY < aBoxSize.iHeight )
            {
            CFbsBitmap* icon = items[i]->Bitmap();
            CFbsBitmap* mask = items[i]->Mask();

            if ( icon && mask )
                {
                AknIconUtils::SetSize( icon, KItemIconSize );

                TRect iconBox( pos, KItemIconSize );
                iconBox.Move( -iconBox.Width() / 2, -iconBox.Height() / 2 );

                aGc.DrawBitmapMasked(
                    iconBox,
                    icon,
                    KItemIconSize,
                    mask,
                    EFalse );
                }
            else
                {
                if ( items[i]->IsPoi() )
                    {
                    aGc.SetPenColor( KPoiColor );
                    }
                else
                    {
                    aGc.SetPenColor( KLmColor );
                    }

                TRect markBox( pos, KItemMarkSize );
                markBox.Move( -markBox.Width() / 2, -markBox.Height() / 2 );
                aGc.SetPenSize( KItemMarkSize );
                aGc.Plot( pos );
                }

            if ( items[i]->Text().Length() )
                {
                TPoint textPos( pos );
                textPos.iX += KLmTextOffset;

                TSize textSize( aFont.TextWidthInPixels( items[i]->Text() ), aFont.HeightInPixels() );
                TRect textBox( textPos, textSize );

                // verify this textbox does not intersect any previous ones
                TBool overlaps = EFalse;
                for ( TInt b = 0; b < textBoxes.Count(); b++ )
                    {
                    if ( textBox.Intersects( textBoxes[b] ) )
                        {
                        overlaps = ETrue;
                        break;
                        }
                    }

                if ( !overlaps )
                    {
                    textBoxes.Append( textBox );

                    aGc.SetPenSize( TSize( 1, 1 ) );

                    TRgb textColor, textShadowColor;

	                if ( items[i]->IsPoi() )
	                    {
	                    textColor = KPoiTextColor;
						textShadowColor = KPoiTextShadowColor;
	                    }
	                else
	                    {
	                    textColor = KLmTextColor;
						textShadowColor = KLmTextShadowColor;
	                    }

                    // text shadow
                    TPoint shadowPos( textPos + TPoint( KLmTextShadowOffset, KLmTextShadowOffset ) );
                    aGc.SetPenColor( aSwitchColorAndShadow ? textColor : textShadowColor );
                    aGc.DrawText( items[i]->Text(), shadowPos );

                    // text itself
                    aGc.SetPenColor( aSwitchColorAndShadow ? textShadowColor : textColor );
                    aGc.DrawText( items[i]->Text(), textPos );
                    }
                }
            }
        }
    textBoxes.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapModel::DrawCurrentLocation(
    CFbsBitGc& aGc,
    const TSize& aBoxSize ) const
    {
    const TSize KPositionPenSize( 20, 20 );
    const TSize KPositionErrorPenSize( 1, 1 );
    const TRgb KPositionColor( 255, 255, 0 );

    // draw current location
    CMnrpMapModel::TCurrentLocation location = CurrentLocation();

    if ( location.iIsValid )
        {
        TPoint& pos = location.iLocation;

        if ( pos.iX >= 0 && pos.iX < aBoxSize.iWidth &&
             pos.iY >= 0 && pos.iY < aBoxSize.iHeight )
            {
            if ( location.iErrorRadius )
                {
                TRect circleBox( pos.iX, pos.iY, pos.iX, pos.iY );
                circleBox.Grow( location.iErrorRadius, location.iErrorRadius );

                aGc.SetPenSize( KPositionErrorPenSize );
                aGc.SetBrushStyle( CGraphicsContext::EVerticalHatchBrush );
                aGc.SetBrushColor( KPositionColor );
                aGc.DrawEllipse( circleBox );
                }

            aGc.SetPenColor( KPositionColor );
            aGc.SetPenSize( KPositionPenSize );
            aGc.Plot( pos );
            }
        }
    }

