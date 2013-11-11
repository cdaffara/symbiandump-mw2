/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ClearScreen handler is responsible for clearing screen when
*                DisplayText has sent immediate response and it is still active
*                when any other command starts to execute.
*
*/



#ifndef CCLEARSCREENHANDLER_H
#define CCLEARSCREENHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "MSatEventObserver.h"

// CONSTANTS

// FORWARD DECLARATIONS
class MSatUtils;

// CLASS DECLARATION

/**
*  CClearScreenHandler for ClearScreen event
*
*  @lib DisplayTextCmd.lib
*  @since Series 60 3.0
*/
class CClearScreenHandler : public CBase,
                            public MSatEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils SAT Utils interface
        */
        static CClearScreenHandler* NewL( MSatUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CClearScreenHandler();

    public: // New functions

        /**
        * Updates immediate response state to ETrue or EFalse
        * @param aState new state
        */
        void UpdateImmediateState( TBool aState );

    public: // From MSatEventObserver

        /**
        * Notification of event.
        * @param aEvent Identifies the event.
        */
        void Event( TInt aEvent );

    private:

        /**
        * C++ default constructor.
        * @param aUtils SAT Utils interface
        */
        CClearScreenHandler( MSatUtils& aUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Sends the request for clear screen.
        */
        void ClearScreen();

    private:    // Data

        // Holds immediate response's status.
        TBool iImmediateResponse;

        // Utils interface
        MSatUtils& iUtils;

    };

#endif      // CCLEARSCREENHANDLER_H

// End of File
