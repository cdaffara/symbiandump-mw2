/*
* Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This file contains the SAT Shell Controller class that is responsible
*     for the actual implementation of the following functionality:
*       - add SAT UI application to Desk (or similar)
*       - remove SAT UI application from Desk (or similar)
*       - bring SAT UI application to foreground if it already is up
*           and running
*       - launching the SAT UI application
*       - launching the Browser
*       - resolving default access point
*       - bring Browser to the foreground
*
*
*/


#ifndef CSATSHELLCONTROLLER_H
#define CSATSHELLCONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include <MSatShellController.h>

// CONSTS
const TInt KSatAppNameSize = 255;       //Context pane max

// CLASS DECLARATION

// DESCRIPTION
// This class is used to control adding, removing and launching of
// the SAT UI Client application. It connects to appropriate module
// and is used by the SAT Server.

class CSatShellController : public CBase,
    public MSatShellController
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSatShellController* NewL();

        /**
        * Destructor.
        */
        virtual ~CSatShellController();

    public: // New functions

        /**
        * Adds SAT UI Client to the Application Shell (or similar).
        * @param aName Name of the entry
        * @param aUid UID of the SAT UI Application
        * @return Nothing
        */
        void AddSatUiL( TDesC& aName );

        /**
        * Adds SAT UI Client to the Application Shell (or similar).
        * @param aName Name of the entry
        * @param aIconId Id for icons
        * @return Nothing
        */
        void AddSatUiL( TDesC& aName, TUint8 aIconId );

        /**
        * Removes SAT UI Client from the Application Shell (or similar).
        * @param aUid UID of the SAT UI Application
        * @return Nothing
        */
        void RemoveSatUiL();

        /**
        * Launches the SAT UI Client.
        * @param aUid UID of the SAT UI Application
        * @return Nothing
        */
        void LaunchSatUiL();

        /**
        * Brings Browser to foreground after period of time.
        * @param Nothing
        * @return Nothing
        */
        void BringBrowserToForegroundAfterPeriod();

        /**
        * Bring the SAT UI Application to foreground.
        * @param Nothing
        * @return Nothing
        */
        void BringSatUiToForeground();
        
        /**
        * Sets SAT UI Application to background if needed.
        */
        void SetSatUiToBackground() const;

        /**
        * Launch XHTML-browser
        * @param aUrl URL to be opened when launching browser
        * @param aAccessPointUid ID number of access point in CommDb
        * @return The result of the operation
        */
        TInt LaunchBrowserL( const TDesC& aUrl, TUid aAccessPointUid );

        /**
        * Resolves Browsers default accesspoint, if any available.
        * @return Accesspoint identifier.
        */
        TInt ResolveDefaultAccesspoint();

        /**
        * This callback function is used to call Browser to the foreground
        * after short while.
        * @param aObject Pointer to caller is passed to callback function.
        * @return Callback function should return TInt.
        */
        static TInt TimerCompleted( TAny* aObject );

    private:

        /**
        * C++ default constructor.
        */
        CSatShellController();

        /**
        * By default constructor is private.
        */
        void ConstructL();

        /**
        * Bring the SAT UI Application to foreground.
        * @param aAppUid The UID value of application.
        * @return Nothing
        */
        void BringApplicationToForeground( const TUid& aAppUid ) const;

        /**
        * Brings Browser to foreground.
        * @param Nothing
        * @return Nothing
        */
        void BringBrowserToForeground() const;

        /**
        * Checks if Browser is not foreground and starts that if needed.
        * @param Nothing
        * @return Nothing
        */
        void CycleSatUiBackwards();
        
        /**
        * Check if SatUi is needed to set to background.
        * @param Nothing
        * @return Nothing
        */
        void CheckSatUiStatus();

    private:    // Data

        TBufC<KSatAppNameSize>  iSimAppName;
        TUid                    iUidWmlBrowser;
        RWsSession              iWsSession;
        RApaLsSession           iApaLsSession;
        // Timer for checking Browser start.
        CPeriodic*              iTimer;
        TBool                   iSetSatUiToBackground;

    };

#endif      // CSATSHELLCONTROLLER_H

// End of File
