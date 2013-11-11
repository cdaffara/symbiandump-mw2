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
* Description:  File Audio Output
*
*/



#ifndef CFAOASYNCHHANDLER_H
#define CFAOASYNCHHANDLER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KIOError        	= 1;

// CLASS DECLARATION

/**
*  Interface class to be implemented by observer of the File Audio Output events generator.
*  @since 3.0
*/
class MFAOEventGeneratorObserver
    {
    public:

        /**
        * Indicates the type of error if any.
        * @since 3.0
        * @param aResult A system-wide error code.
        * @return none
        */
        virtual void IOError(TInt aResult) = 0;
    };

/**
*  CFAOEventGenerator
*
*  @lib FileAudioOutput.lib
*  @since 3.0
*/
class CFAOEventGenerator : public CActive
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CFAOEventGenerator* NewL(MFAOEventGeneratorObserver& aObserver);

        /**
        * Request to generate event
        * @aEvent The event to generate
        * @aError The error to report with the event
        * @since 3.0
        */
        void GenerateEvent(TInt aEvent, TInt aError);

        virtual ~CFAOEventGenerator();

    private:

        /**
        * C++ default constructor.
        */
        CFAOEventGenerator(MFAOEventGeneratorObserver& aObserver);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void RunL();

        void DoCancel();

        /**
        * Used to complete a request
        */
        void Ready(const TInt aStatus);

    private:

    	// The requested event
    	TInt iEvent;

    	// Error indicator
    	TInt iError;

        // Pointer to observer object
        MFAOEventGeneratorObserver* iObserver;
    };

#endif 		// CFAOASYNCHHANDLER_H