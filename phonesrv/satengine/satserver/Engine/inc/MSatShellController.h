/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the SAT Shell Controller Interface that is
*                responsible for the actual implementation of the following
*                functionality:
*                - add SAT UI application to Desk (or similar)
*                - remove SAT UI application from Desk (or similar)
*                - bring SAT UI application to foregroud if it already is up
*                  and running
*                - launching the SAT UI application
*                - launching the Browser application
*                - resolving default access point
*
*/


#ifndef MSATSHELLCONTROLLER_H
#define MSATSHELLCONTROLLER_H

#include <e32base.h>

/**
* This class is used to control adding, removing and launching of
* the SAT UI Client application. It connects to appropriate module
* and is used by the SAT Server.
*
* @lib SatEngine.lib
* @since Series 60 2.0
*
*/

class MSatShellController
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MSatShellController() {};

    public: // New functions

        /**
        * Adds SAT UI Client to the Application Shell (or similar).
        * @param aName Name of the entry
        */
        virtual void AddSatUiL( TDesC& aName ) = 0;

        /**
        * Adds SAT UI Client to the Application Shell (or similar).
        * @param aName Name of the entry
        * @param aIconId Id of the icon.
        */
        virtual void AddSatUiL( TDesC& aName, TUint8 aIconId ) = 0;

        /**
        * Removes SAT UI Client from the Application Shell (or similar).
        */
        virtual void RemoveSatUiL() = 0;

        /**
        * Launches the SAT UI Client.
        */
        virtual void LaunchSatUiL() = 0;
        
        /**
        * Bring Browser to foreground after period of time.
        */
        virtual void BringBrowserToForegroundAfterPeriod() = 0;

        /**
        * Bring the SAT UI Application to foreground.
        */
        virtual void BringSatUiToForeground() = 0;
        
        /**
        * Sets SAT UI Application to background if needed.
        */
        virtual void SetSatUiToBackground() const = 0;

        /**
        * Launch Browser
        * @since Series 60 2.6
        * @param aUrl URL to be opened when launching browser.
        * @param aAccessPointUid ID number of access point in CommDb.
        * @return The result of the operation.
        */
        virtual TInt LaunchBrowserL(
            const TDesC& aUrl,
            TUid aAccessPointUid ) = 0;
    };

#endif      // MSATSHELLCONTROLLER_H

// End of File
