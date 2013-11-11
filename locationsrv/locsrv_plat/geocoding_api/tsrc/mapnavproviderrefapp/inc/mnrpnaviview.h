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
* Description:  CMnrpNaviView class definition
*
*/


#ifndef MNRP_NAVIVIEW_H
#define MNRP_NAVIVIEW_H

#include <aknview.h>

class CMnrpEngine;
class CMnrpNaviModel;
class CMnrpNaviControl;
class CPosLandmark;

/** Map view. Draws map on screen using information from 
 	CMnrpMapModel.
 */
class CMnrpNaviView : public CAknView
    {
    public: 

        static CMnrpNaviView* NewL( TBool aChained, CMnrpEngine& aEngine );
        virtual ~CMnrpNaviView();
        
        /** Shows map view in browser mode 
         *  @param aService Object to get map view options from
         */
        void SetDestinationL( const CPosLandmark& aDestination );
        
    public: // from CAknView

        TUid Id() const;

        void HandleCommandL( TInt aCommand );

        void DoActivateL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        void DoDeactivate();

    private:

        CMnrpNaviView( CMnrpEngine& aEngine );
        void ConstructL( TBool aChained );
        
    private:
        /** Application engine */
        CMnrpEngine& iEngine;
    	
    	/** UI control */
        CMnrpNaviControl* iControl;    
        
        /** Destination */
        CPosLandmark* iDestination;
    };

#endif // MNRP_NAVIVIEW_H

