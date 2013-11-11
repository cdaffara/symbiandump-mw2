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
* Description: Declaration of CPhoneSettingsTestNetworkModeObserver class.
*
*/



#ifndef PHONESETTINGSTESTNETWORKMODEOBSERVER_H
#define PHONESETTINGSTESTNETWORKMODEOBSERVER_H

#include <e32base.h>
#include <mpsetnetworkmodeobs.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestNetworkModeObserver) : 
    public MPsetNetworkModeObserver, 
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestNetworkModeObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestNetworkModeObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestNetworkModeObserver(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();    

    public: // Derived from MPsetNetworkModeObserver

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
            const TUint32 aNetworkModeCaps );

        /**
        * Handles errors.
        *
        * @param aRequest current request.
        * @param aError error code.
        */
        virtual void HandleNetworkErrorL(
            const MPsetNetworkModeObserver::TServiceRequest aRequest,
            const TInt aError );
    };

#endif      // PHONESETTINGSTESTNETWORKMODEOBSERVER_H

// End of File