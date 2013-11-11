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
* Description: Declaration of CPhoneSettingsTestNetworkInfoObserver class.
*
*/



#ifndef PHONESETTINGSTESTNETWORKINFOOBSERVER_H
#define PHONESETTINGSTESTNETWORKINFOOBSERVER_H

#include <e32base.h>
#include <mpsetnetworkinfoobs.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestNetworkInfoObserver) : 
    public MPsetNetworkInfoObserver,
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestNetworkInfoObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestNetworkInfoObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestNetworkInfoObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();    

    public: // Derived from MPsetNetworkInfoObserver
    
        /**
        * Shows notes when network information received.
        *
        * @param aInfoArray Network information. Receiver owns the array.
        * @param aResult Result of action.
        */
        virtual void HandleNetworkInfoReceivedL( 
            const CNetworkInfoArray* aInfoArray, const TInt aResult );
        
        /**
        * Shows notes when requested for current network information.
        *
        * @param aCurrentInfo Current network information.
        * @param aResult Result of action.
        */
        virtual void HandleCurrentNetworkInfoL( 
            const MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, 
            const TInt aResult );
        
        /**
        *  DEPRECATED.
        */
        virtual void HandleNetworkChangedL( 
            const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
            const MPsetNetworkSelect::TCurrentNetworkStatus aStatus, 
            const TInt aResult );    

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
            const TInt aResult );    

        /**
        * Searches for networks that have coverage.
        *
        * @param aRequest Type of request that is being handled.
        */
        virtual void HandleSearchingNetworksL( TServiceRequest aRequest );

        /**
        * Handles user request to register to specific network.
        *
        * @param aOngoing Is there an ongoing request.
        */
        virtual void HandleRequestingSelectedNetworkL( TBool aOngoing );

        /**
        * Handles notes when call is active.
        */
        virtual void HandleCallActivatedL();

        /**
        * Handles errors.
        * 
        * @param aRequest current request.
        * @param aError error code.
        */
        virtual void HandleNetworkErrorL( const MPsetNetworkInfoObserver::TServiceRequest aRequest,
            const TInt aError );    
    };

#endif      // PHONESETTINGSTESTNETWORKINFOOBSERVER_H

// End of File