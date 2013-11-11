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
* Description:  CMnrpNavigationService class definition
*
*/


#ifndef MNRP_NAVIGATIONSERVICE_H
#define MNRP_NAVIGATIONSERVICE_H

#include "mnnavigationservicebase.h"

class CPosLandmark;

/** Implementation of Navigation service (KMnAppNavigationService). 
 */
class CMnrpNavigationService : public CMnNavigationServiceBase
    {
    public :
        static CMnrpNavigationService* NewL();
        
    protected :
        CMnrpNavigationService();
        virtual ~CMnrpNavigationService();
        void ConstructL();

    public: // from CMnNavigationServiceBase
    
        /** Handles client's request for navigation. */
        void HandleNavigateToL( const CPosLandmark& aDestination );
    
    protected: // from CMnServiceBase

        /** Empty implementation, as the only request is sychcronous */
        void DoCancel();
    };

#endif // MNRP_NAVIGATIONSERVICE_H


