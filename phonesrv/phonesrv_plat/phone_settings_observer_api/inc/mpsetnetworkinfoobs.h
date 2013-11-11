/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Observes completions of network info and change requests.
*
*
*/


#ifndef     MPSETNETWORKINFOOBS_H
#define     MPSETNETWORKINFOOBS_H

//  INCLUDES
#include "mpsetnetworkselect.h" 
#include <etelmm.h>

//  DATA TYPES  
typedef CArrayFixFlat<MPsetNetworkSelect::TNetworkInfo> CNetworkInfoArray;

//  CLASS DEFINITIONS 
/**
*  MPsetNetworkInfoObserver class is virtual class for network information
*  observers.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetNetworkInfoObserver
    {
    public:            
        /**
        * Shows notes when network information received.
        *
        * @param aInfoArray Network information. Receiver owns the array.
        * @param aResult Result of action.
        */
        virtual void HandleNetworkInfoReceivedL( 
            const CNetworkInfoArray* aInfoArray, const TInt aResult ) = 0;
        
        /**
        * Shows notes when requested for current network information.
        *
        * @param aCurrentInfo Current network information.
        * @param aResult Result of action.
        */
        virtual void HandleCurrentNetworkInfoL( 
            const MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, 
            const TInt aResult ) = 0;
        
        /**
        *  DEPRECATED.
        */
        virtual void HandleNetworkChangedL( 
            const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
            const MPsetNetworkSelect::TCurrentNetworkStatus aStatus, 
            const TInt aResult ) = 0;    

        /**
        * Shows notes when network change has been approved.
        *
        * @param aCurrentInfo Current network information.
        * @param aStatus Current network status.
        * @param aResult Result of action.
        */
        virtual void HandleNetworkChangedL( 
            const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
            const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus, 
            const TInt aResult ) = 0;    

        //Service requests for MPsetNetworkInfoObserver
        enum TServiceRequest {
            EServiceRequestNone,
            EServiceRequestEnumerateNetworks,
            EServiceRequestGetNetworkInfo,
            EServiceRequestGetCurrentNetworkInfo,
            EServiceRequestGetNetworkSearchMode,
            EServiceRequestSetNetwork,
            EServiceRequestSetNetworkMode,
            EServiceRequestSetNetworkAutomatic
            };

        /**
        * Searches for networks that have coverage.
        *
        * @param aRequest Type of request that is being handled.
        */
        virtual void HandleSearchingNetworksL( TServiceRequest aRequest ) = 0;

        /**
        * Handles user request to register to specific network.
        *
        * @param aOngoing Is there an ongoing request.
        */
        virtual void HandleRequestingSelectedNetworkL( TBool aOngoing ) = 0;

        /**
        * Handles notes when call is active.
        */
        virtual void HandleCallActivatedL() = 0;

        /**
        * Handles errors.
        * 
        * @param aRequest current request.
        * @param aError error code.
        */
        virtual void HandleNetworkErrorL( const MPsetNetworkInfoObserver::TServiceRequest aRequest,
            const TInt aError ) = 0;
    };

#endif // MPSETNETWORKINFOOBS_H
// end of file
