/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Handles landmark settings events.
*
*/


#ifndef CPOSLMSERVERSETTINGSHANDLER_H
#define CPOSLMSERVERSETTINGSHANDLER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmServer;
class CPosLmSettingsHandler;

// CLASS DECLARATION

/**
 *  Handles landmark settings events.
 */
class CPosLmServerSettingsHandler : public CActive
    {
    public:    // Constructors and destructor

        /**
        * Two-phase constructor
        * @param aServer The server creating this session.
        */
        static CPosLmServerSettingsHandler* NewL(
        /* IN */        CPosLmServer& aServer
        );

        /**
        * C++ Destructor
        */
        ~CPosLmServerSettingsHandler();

    public:     // New functions

        /**
        * Starts listening for landmark settings events.
        */
        TInt StartListening();

    protected:  // Functions from base classes

        /**
        * From CActive.
        * Active Object event handler.
        * Restarts the listening for landmark settings events. Then it
        * notifies listening clients that landmark settings have changed.
        */
        void RunL();

        /**
        * From CActive.
        * Active Object cancel handler.
        */
        void DoCancel();

        /**
        * From CActive.
        * If RunL leaves, all listening clients are notified. Only the call to
        * User::LeaveIfError(StartListening()) can leave.
        * HandleLandmarkSettingsChangedL does not leave. StartListening returns
        * an error code if the settings handler in Internal Services could not
        * be started.
        *
        * @return An error code.
        */
        TInt RunError(TInt aError);

    private:

        /**
        * C++ Constructor
        */
        CPosLmServerSettingsHandler(
        /* IN */        CPosLmServer& aServer
        );

        /**
        *  By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void HandleLandmarkSettingsChangedL(
        /* IN */        const TDesC& aUri
        );

        // By default, prohibit copy constructor
        CPosLmServerSettingsHandler(
            const CPosLmServerSettingsHandler& );
        // Prohibit assigment operator
        CPosLmServerSettingsHandler& operator= (
            const CPosLmServerSettingsHandler& );

    private:    // Data

        CPosLmSettingsHandler* iSettingsHandler;
        CPosLmServer*          iLmServer;

    };

#endif // CPOSLMSERVERSETTINGSHANDLER_H

// End of File
