/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*      Observes completions of network mode and change requests.
*
*
*/


#ifndef     MPSETNETWORKMODEOBS_H
#define     MPSETNETWORKMODEOBS_H

//  CLASS DEFINITIONS 
/**
*  MPsetNetworkModeObserver class is virtual class for network mode observers.
*  @lib phonesettings.lib
*  @since 2.6
*/
class   MPsetNetworkModeObserver
    {
    public:
                
        //Service requests for MPsetNetworkModeObserver
        enum TServiceRequest {            
            EServiceRequestNone,
            EServiceRequestSetSelectedNetworkMode,
            EServiceRequestGetCurrentNetworkMode
            //EServiceRequestGetSupportedNetworkModes
            };        

        /**
        * Handles Network system mode list fetching from CustomAPI.
        * Handles Network system mode setting to CustomAPI.
        * Handles current network system mode fetching from CustomAPI.
        * 
        * @param aRequest one of the TServiceRequest values
        * @param aNetworkModeCaps current/list of supported network mode(s).
        * @since 2.6
        */
        virtual void HandleNetworkSystemModeEventsL( 
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TUint32 aNetworkModeCaps ) = 0;

        /**
        * Handles errors.
        *
        * @param aRequest current request.
        * @param aError error code.
        */
        virtual void HandleNetworkErrorL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TInt aError ) = 0;
        
    };

#endif // MPSETNETWORKMODEOBS_H

// End of File
