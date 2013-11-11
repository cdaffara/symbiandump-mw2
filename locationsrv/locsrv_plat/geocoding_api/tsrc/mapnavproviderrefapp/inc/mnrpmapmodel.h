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
* Description:  CMnrpMapModel class definition
*
*/


#ifndef MNRP_MAPMODEL_H
#define MNRP_MAPMODEL_H

#include <coecntrl.h>
#include "mnrppositionrequest.h"
#include "epos_landmarks.h"

class CFbsBitmap;
class CImageDecoder;

class CPosLandmark;
class CPosLandmarkDatabase;
class CPosLmCategoryManager;

class CMnrpMapViewService;
#ifdef RD_MAPNAV_BITMAP_ACCESS
class CMnrpMapImageService;
class TMnMapImageParams;
#endif
class CMnrpEngine;

class MImageLoaderObserver;

/** Point with real coordinates */
class TRealPoint
    {
    public:
        TRealPoint();
        TRealPoint( TReal aX, TReal aY );
        TRealPoint( TCoordinate aCoordinate );

        TRealPoint operator+( const TRealPoint& aRight );
        TRealPoint operator-( const TRealPoint& aRight );
        TRealPoint operator*( const TReal& aRight );
        TRealPoint operator/( const TReal& aRight );

        void Set( TReal aX, TReal aY );
        TCoordinate Coordinate();
        TBool IsValid();

        TReal iX; // matches longitude in this model
        TReal iY; // matches latitude in this model
    };

/** Model observer class */
class MMapModelObserver
    {
    public:
        virtual void HandleModelUpdateL() =0;
    };

/** Data model for MapView.
 *  This implementation shows map as rectangle and does not support
 *  passing its borders. It displays POIs from Engine and client's
 *	landmarks, which define view area, if not specified by client.
 */
class CMnrpMapModel : public CBase, public MMnrpPositionObserver
    {
    public:
        IMPORT_C static CMnrpMapModel* NewL(
            MMapModelObserver& aObserver,
            CMnrpEngine& aEngine );

        IMPORT_C static CMnrpMapModel* NewL(
            CMnrpEngine& aEngine );

        IMPORT_C ~CMnrpMapModel();

		/** Service calls it when new ShowOnMap request comes
		 *	from client */
        IMPORT_C void UpdateModelL( CMnrpMapViewService& aService );

#ifdef RD_MAPNAV_BITMAP_ACCESS
		/** Service calls it when new RenderImage request comes
		 *	from client */
        IMPORT_C void UpdateModelL( const CMnrpMapImageService& aService );

		/** Plugin calls it when new CoordinateConversion request comes
		 *	from client */
        IMPORT_C void UpdateModel( const TMnMapImageParams& aParams );
#endif
		/** Defines dimensions of view used for map display.
		 *	this information is used to convert absolute coordinates
		 *	to screen coordinates. Absolute coordinates are plain
		 *	latitude and longitude degrees.
		 */
        IMPORT_C void SetScreenSize( TSize aScreenSize );

        IMPORT_C void RenderL(
            CFbsBitmap& aBitmap,
            TRect aTargetRect,
            CFont* aTextFont,
            CFont* aItemFont ) const;
        
        IMPORT_C TInt NumIgnoredLandmarks();

		/** Draw item.
		 *	It is a POI or client's landmark, holds coordinates
		 *	and associated text and icon.
		 */
        class CDrawItem : public CBase
            {
            public:
            	/** Constructs from landmark instance */
                static CDrawItem* NewLC( const CPosLandmark& );
            	/** Constructs from linked landmark. If landmark
            	 *	does not have icon, it tries to get icon of its
            	 *	category, if only single category is assigned.
            	 */
                static CDrawItem* NewLC(
                    const CPosLandmark& aLm,
                    CPosLmCategoryManager& aCatman );

                ~CDrawItem();

                const TDesC& Text() { return *iText; };
                TPoint& Position() { return iScreenPosition; };
                TRealPoint AbsolutePosition() { return iAbsolutePosition; };
                CFbsBitmap* Bitmap() { return iBitmap; };
                CFbsBitmap* Mask() { return iMask; };
                TBool IsPoi() { return iOwnPoi; };
                
                void SetAbsolutePosition( const TLocality& aLocality );

            private:
                HBufC*      iText;
               	/** Absolute coordinates - lat,lon */
                TRealPoint  iAbsolutePosition;
                /** Coordinates on screen (pixels) */
                TPoint      iScreenPosition;
                CFbsBitmap* iBitmap;
                CFbsBitmap* iMask;
                /** Whether it is client's landmark or internal POI */
                TBool       iOwnPoi;

            friend class CMnrpMapModel;
            };
		/** Returns all items, that can be shown on screen. Screen
		 *	coordinates are updated according to current scale and viewport */
        TArray<CDrawItem*> Items() const;

		/** Describes grid to show */
        class TGrid
            {
            public:
            	/** Reference grid point (screen) */
                TPoint iReference;
                /** Distance between grid lines (screen)*/
                TInt iGridStep;

            	/** Coordinate of reference grid point (absolute) */
                TCoordinate iAbsoluteReference;
                /** Distance between grid lines (absolute) */
                TReal iAbsoluteGridStep;
            };

        /** Returns grid for current viewport and scale */
        void GetGrid( TGrid& aGrid ) const;

        /** Information about current location */
        class TCurrentLocation
            {
            public:
            	/** Screen coordinates */
                TPoint  iLocation;
            	/** Error radius in screen units - pixels */
                TInt    iErrorRadius;

                /** Absolute coordinates */
                TCoordinate iAbsoluteLocation;
            	/** Error radius in degrees */
                TReal       iAbsoluteErrorDegrees;

                TBool       iIsValid;
            };

        /** Returns current location */
        IMPORT_C TCurrentLocation CurrentLocation() const;

		// Zooming
        IMPORT_C void Zoom( TReal aZoomRate );

		// View movement
        IMPORT_C void MoveTo( TCoordinate aTarget );

        // Reset-kind of action, moves to world center
        IMPORT_C void GoCenter();

		/** Converts absolute units to screen */
        IMPORT_C TPoint AbsoluteToScreen( TCoordinate aAbsolutePoint ) const;
        IMPORT_C TInt AbsoluteToScreen( TReal aDistance ) const;

		/** Converts screen coords to absolute */
        IMPORT_C TCoordinate ScreenToAbsolute( TPoint aScreenPoint ) const;
        IMPORT_C TReal ScreenToAbsolute( TInt aDistance ) const;

		// Viewport in absolute units - degrees
        IMPORT_C TReal Left() const;
        IMPORT_C TReal Top() const;
        IMPORT_C TReal Right() const;
        IMPORT_C TReal Bottom() const;
        IMPORT_C TReal Width() const;
        IMPORT_C TReal Height() const;
        IMPORT_C TCoordinate Center() const;

    protected:
		/** Converts absolute coords to screen */
        TPoint AbsoluteToScreen( TRealPoint aAbsolutePoint ) const;

    protected:  // from MMnrpPositionObserver

		/** Forces map view update when new position comes */
        void HandlePositionRequestCompletedL( TInt aError );

    private:

        void ConstructL();
        CMnrpMapModel( MMapModelObserver& aObserver, CMnrpEngine& aEngine );
        CMnrpMapModel( CMnrpEngine& aEngine );

        void AddCitiesL();

		/** Includes given draw item into bounding box */
        void AdjustBoundingBox( CDrawItem& aItem, TRealPoint& aMin, TRealPoint& aMax );
        
        /** Verifies if draw item has position. If not, tries geocoding.
         *  Updates the item if position is found from geocoding.
         *  @return ETrue if position is found (possibly from geocoding), EFalse otherwise */
        TBool CheckDrawItemHasPositionL( CDrawItem& aItem, const CPosLandmark& aLandmark );

		/** Adjusts viewport if movement or zooming goes beyond the borders */
        void CheckBorders();

        TReal MaxScale() const;
        TRect CursorMargin();

		// Grid drawing
        void DrawGrid(
            CFbsBitGc& aGc,
            const TSize& aBoxSize,
            CMnrpMapModel::TGrid& aGrid ) const;

        void DrawVerticalLine( CFbsBitGc& aGc, TInt aX, TSize aBoxSize ) const;
        void DrawHorizontalLine( CFbsBitGc& aGc, TInt aY, TSize aBoxSize ) const;

		/** Draws drawable items */
        void DrawLandmarks(
            CFbsBitGc& aGc,
            CFont& aFont,
            const TSize& aBoxSize,
            TBool aSwitchColorAndShadow ) const;

		/** Draws current location */
        void DrawCurrentLocation(
            CFbsBitGc& aGc,
            const TSize& aBoxSize ) const;

    private:
        CMnrpPositionRequest*           iPositionRequest;
        TPositionInfo                   iPosInfo;

        TSize                           iScreenSize;
        TRealPoint                      iViewOrigin;

        TReal                           iScale; // 1 == 1 degree per pixel

        MMapModelObserver*              iObserver;

        mutable RPointerArray<CDrawItem> iDrawItems;

        CMnrpEngine&            		iEngine;

        TBool                           iUseCurrentLocationAsOrigin;
        TBool                           iShowCurrentLocation;

        CFont*                          iTextFont;
        CFont*                          iItemFont;
        
        TInt 							iNumIgnoredLandmarks; 
    };

#endif // MNRP_MAPMODEL_H

