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
* Description:  CMnrpMapView class definition
*
*/


#ifndef MNRP_MAPVIEW_H
#define MNRP_MAPVIEW_H

#include <aknview.h>

class CMnrpEngine;
class CMnrpMapModel;
class CMnrpMapControl;

class CMnrpMapViewService;
class MMapSelectionCallback;

/** Map view. Draws map on screen using information from 
 	CMnrpMapModel.
 */
class CMnrpMapView : public CAknView
    {
    public: 

        static CMnrpMapView* NewL( TBool aChained, CMnrpEngine& aEngine );
        virtual ~CMnrpMapView();
        
        /** Shows map view in browser mode 
         *  @param aService Object to get map view options from
         */
        void SetMapModeL( CMnrpMapViewService& aService );

        /** Shows map view in selection mode 
         *  @param aService Object to get map view options from
         *  @param aCallback Object to inform about selection
         */
        void SetSelectionModeL( 
            CMnrpMapViewService& aService,  
            MMapSelectionCallback& aCallback );

        void HandleUserSelectionL( TInt aError );

        void StopSelection();
		
    public: // from CAknView

        TUid Id() const;

        void HandleCommandL( TInt aCommand );

        void DoActivateL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        void DoDeactivate();
        
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane *aMenuPane );

    private:

        CMnrpMapView( CMnrpEngine& aEngine );
        void ConstructL( TBool aChained );
        
    private:
        /** Application engine */
        CMnrpEngine& iEngine;
    	
    	/** UI control */
        CMnrpMapControl* iControl;    

        /** Map view data */
        CMnrpMapViewService* iMapViewService;

        /** Selection callback listener */        
        MMapSelectionCallback* iSelectionCallback;
        
        TBool iChained;
    };

#endif // MNRP_MAPVIEW_H

