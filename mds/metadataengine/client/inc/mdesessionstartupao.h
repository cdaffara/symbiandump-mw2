/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MdE session startup active object*
*/


#ifndef __MDESESSIONSTARTUPAO_H__
#define __MDESESSIONSTARTUPAO_H__

#include <e32base.h>

// Forward declaration
class CMdESessionImpl;
class RMdEEngineSession;

/**
* CMdESessionStartupAO
* An active object that handles asynchronous MdE session starting.
*/
NONSHARABLE_CLASS(CMdESessionStartupAO) : public CActive
    {
    public: // Constructors and destructors

        /**
        * Two-phased constructor.
        * @param aSessionImpl session impl
        * @param aSession the server handler
        * @return the created instance
        */
        static CMdESessionStartupAO* NewL(
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * Two-phased constructor.
        * @param aSessionImpl session impl
        * @param aSession the server handler
        * @return the created instance
        */
        static CMdESessionStartupAO* NewLC(
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * Destructor.
        */
        virtual ~CMdESessionStartupAO();

    protected: // Functions from base classes

        /**
        * From CActive
        * Callback function.
        * Invoked to handle responses from the server.
        */
        void RunL();

        /**
        * From CActive
        * Handles errors that occur during notifying the observer.
        */
        TInt RunError(TInt aError);

        /**
        * From CActive
        * Cancels any outstanding operation.
        */
        void DoCancel();

    private: // Constructors and destructors

        /**
        * constructor
        * @param aSessionImpl session impl
        * @param aSession the server handler
        */
        CMdESessionStartupAO( 
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

    private: // Data

		enum TMdESessionStartupAOStates
			{
			EStartupOpenServer,
			EStartupConnect,
			EStartupLoadSchema,
			EStartupNotifyClient
			};

        TMdESessionStartupAOStates iState;

        /** the session impl. */
        CMdESessionImpl& iSessionImpl;

        /** the session */
        RMdEEngineSession& iSession;
        
        RTimer iTimer;
    };

#endif //__MDESESSIONSTARTUPAO_H__
