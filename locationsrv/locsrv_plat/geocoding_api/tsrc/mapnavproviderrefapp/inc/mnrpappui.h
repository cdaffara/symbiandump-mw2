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
* Description:  CMnrpAppUi class definition
*
*/


#ifndef MNRP_APPUI_H
#define MNRP_APPUI_H

#include <aknviewappui.h>
#include <epos_landmarks.h>

class TCoordinate;
class CPosLandmark;

class CMnrpAppView;
class CMnrpNaviView;
class CMnrpMapView;

class CMnrpMapViewService;

class CAknNavigationDecorator;
class CAknNavigationControlContainer;

class CMnrpEngine;

/** Map selection callback class */
class MMapSelectionCallback
    {
    public:
        /** Called when user selects location from map */
        virtual void HandleSelectionL( const TCoordinate& aSelection ) = 0;

        /** Called when user selects landmark from dialog list */
        virtual void HandleSelectionL( TInt aLandmarkIndex ) = 0;
        virtual void HandleSelectionL( TPosLmItemId aLandmarkId, TInt aDbIndex ) = 0;
        
        /** Called when user refuses selection */
        virtual void HandleSelectionErrorL( TInt aError ) = 0;
    };

/** Simple App UI for MnRefProvider component.
 *	It manages application views.
 */
class CMnrpAppUi : public CAknViewAppUi
    {
    public:

        CMnrpAppUi( CMnrpEngine& aEngine );
        void ConstructL();
        ~CMnrpAppUi();

        void ShowMapL( CMnrpMapViewService& aService );

        void StartSelectionL( 
            CMnrpMapViewService& aService,  
            MMapSelectionCallback& aCallback );

        void StopSelection();

        void ShowNavigationL( const CPosLandmark& aDestination );

		/** Returns ETrue if Map or Navigaton view is still open */
        TBool IsAnyUserViewActive();
        
		/** Returns ETrue if application was started embedded */
        TBool IsChainedMode();

        void SetNaviPaneTitleL( const TDesC& aPaneTitle );
        void SetRightSoftkeyCommandL( TInt aCommand, const TDesC& aButtonTitle );
        
    protected: // from CAknAppUi
    
        void HandleCommandL(TInt aCommand);
        void HandleResourceChangeL( TInt aType );
        TBool ProcessCommandParametersL( CApaCommandLine &aCommandLine );

    private:

        void PostConstructL();

        void SelectFromDialogL(
            CMnrpMapViewService& aService,
            MMapSelectionCallback& aCallback );

    private:
    	/** Empty view */
        CMnrpAppView* iAppView;

		/** Navigation view */
        CMnrpNaviView* iNaviView;
		/** Map view */
        CMnrpMapView* iMapView;

        //Own:
        CAknNavigationDecorator* iNaviPaneLabel;

        //Ref:
        CAknNavigationControlContainer* iNaviPane;
        
        TInt iParentWg;

        /** Reference to engine */
        CMnrpEngine& iEngine;
    };

#endif // MNRP_APPUI_H

