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
* Description:  CMnrpNaviControl class definition
*
*/


#ifndef MNRP_NAVICONTROL_H
#define MNRP_NAVICONTROL_H

#include <coecntrl.h>
#include "mnrpnavimodel.h"

class CAknSingleHeadingStyleListBox;
class CMnrpNaviView;

/** Navigation view.
 *	Shows list box with navigation information items: 
 *	destination name and coordinates, current position coordinates
 *	distance and bearing to destination.
 */	
class CMnrpNaviControl : public CCoeControl, public MNaviModelObserver
    {
    public:

        static CMnrpNaviControl* NewL( CMnrpNaviView& aView, const TRect& aRect, CMnrpEngine& aEngine );
        virtual ~CMnrpNaviControl();

        void UpdateModelL( CPosLandmark& aDestination );
        
    protected:    
    	/** Creates list box */
        void CreateListBoxL();

    public: // from MNaviModelObserver
        void HandleModelUpdateL();

    protected:  // from CCoeControl
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl( TInt aIndex ) const;
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        void SizeChanged();
        void HandleResourceChange( TInt aType );

    private:

        void ConstructL( const TRect& aRect, CMnrpEngine& aEngine );
        CMnrpNaviControl( CMnrpNaviView& aView );
    
    private:
        CMnrpNaviView&                  iView;
    
        CAknSingleHeadingStyleListBox*  iListBox;
        CMnrpNaviModel*         		iModel;
    };

#endif // MNRP_NAVICONTROL_H

