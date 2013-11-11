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
* Description:  CMnrpMapViewService class definition
*
*/


#ifndef MNRP_MAPVIEWSERVICE_H
#define MNRP_MAPVIEWSERVICE_H

#include <mnmapviewservicebase.h>

#include "mnrpappui.h"

class CPosLandmark;

/** Implementation of MapView service (KMnAppMapViewService). 
 */
class CMnrpMapViewService : public CMnMapViewServiceBase, public MMapSelectionCallback
    {
    public :
        static CMnrpMapViewService* NewL();
        
    protected :
        CMnrpMapViewService();
        virtual ~CMnrpMapViewService();
        void ConstructL();

        CMnrpAppUi* AppUi();

    protected: // from CMnMapViewServiceBase
    
        /** Handles client's request for showing map */
        void HandleShowMapL();

        /** Handles client's request for showing map from current location */
        void HandleShowCurrentLocationL();

        /** Handles client's request for selecting a location from map */
        void HandleSelectFromMapL();

    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

        void CompleteSelectionL( const TCoordinate& aSelection );

    protected: // from MMapSelectionCallback
            
        void HandleSelectionL( const TCoordinate& aSelection );

        void HandleSelectionL( TInt aLandmarkIndex );
        void HandleSelectionL( TPosLmItemId aLandmarkId, TInt aDbIndex );

        void HandleSelectionErrorL( TInt aError );

    private:
        void StopSelection();

    private:

        /** Flag shows that selection is ongoing */
        TBool                   iSelecting;
    };

#endif // MNRP_MAPVIEWSERVICE_H


