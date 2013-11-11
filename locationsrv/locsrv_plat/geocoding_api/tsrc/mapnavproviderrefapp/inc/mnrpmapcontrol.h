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
* Description:  CMnrpMapControl class definition
*
*/


#ifndef MNRP_MAPCONTROL_H
#define MNRP_MAPCONTROL_H

#include <coecntrl.h>
#include "mnrpmapmodel.h"

class CMnrpEngine;
class CMnrpMapView;

/** Map view. Draws map on screen using information from 
 	CMnrpMapModel.
 */
class CMnrpMapControl : public CCoeControl, public MMapModelObserver
    {
    public: 

        static CMnrpMapControl* NewL( CMnrpMapView& aView, const TRect& aRect, CMnrpEngine& aEngine );
        virtual ~CMnrpMapControl();
        
        void UpdateModelL( CMnrpMapViewService& aService );
        void SetSelectionModeL( TBool aSelectionMode );
        
        /** Returns current cursor location in selection mode */
        TCoordinate CurrentLocation();

        void ToggleFullScreen();

    public:  // from CCoeControl

        void Draw( const TRect& aRect ) const;
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        void SizeChanged();
        void HandleResourceChange( TInt aType );

    public:  // from MMapModelObserver

		/** Redraws itself when model is updated */
        void HandleModelUpdateL();

    private:

        CMnrpMapControl( CMnrpMapView& aView );
        void ConstructL( const TRect& aRect, CMnrpEngine& aEngine );
        
        void DrawCursor( CWindowGc& aGc ) const;

		/** Draws text around map
		 *	Top area - left longitude, cell size, right longitude
		 *	Left area - top latitude, bottom latitude
		 *	Bottom area - center coordinates
		 */
        void DrawGridText( 
            CWindowGc& aGc, 
            const TRect& aInnerBox,
            const TRect& aOuterBox,
            const CFont* aFont,
            CMnrpMapModel::TGrid& aGrid ) const;

        /** Helper to find out border size based on font dimensions */
        TSize BorderSize() const;

		/** Returns specifications of used font */
        TFontSpec SmallFontSpec( TInt aFontHeightInTwips ) const;
        
        void MoveCursor( TInt aXmove, TInt aYmove );
        TRect CursorMargin();

        void Zoom( TReal aRate );

        void SetFullScreen( TBool aFullScreen );

		void CreateFonts();
		void DestroyFonts();

		void CreateScreenBufferL( const TSize& aSize );
        void RenderL();

    private:
        /** Parent view */
        CMnrpMapView& iView;
        
    	/** Model */
        CMnrpMapModel* iModel;
        TSize iViewSize;
        
        TBool iSelectionMode;
        TPoint iCursor;
        TCoordinate iAbsoluteCursor;

        /** Whether fullscreen mode on or off*/
        TBool iIsFullScreen;
        
        mutable CFbsBitmap* iOffScreenBitmap;
        CFont* iTextFont;
        CFont* iItemFont;
    };

#endif // MNRP_MAPVIEW_H

