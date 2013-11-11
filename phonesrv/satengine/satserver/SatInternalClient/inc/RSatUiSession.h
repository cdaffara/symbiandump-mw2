/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The UI Client API of the SAT Server.
*
*/



#ifndef RSATUISESSION_H
#define RSATUISESSION_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// The name of the SatClient module for panic purposes
_LIT( KSatCliName, "SatClient" );

// DATA TYPES

// The Sat Client Panic codes
enum TSatClientPanic
    {
    ESatInvalidResponse,
    ESatSetUpMenuFailure,
    ESelectedItemOutOfRange
    };

// FORWARD DECLARATIONS

class MSatUiObserver;
class MSatUiAdapter;
class CSatCCommandProcessor;
class CSatCDisplayTextHandler;
class RSatSession;

// CLASS DECLARATION

/**
 *  UI sub-session to SatServer.
 *  Use this to register your event observer object.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.0
 */

class RSatUiSession : public RSubSessionBase
    {
    public: // Constructors and destructor

        /**
         * C++ default constructor.
         */
        IMPORT_C RSatUiSession();

    public: // New functions

        /**
         * Registers the listener object for SAT events and connects to
         * the Sat Server.
         * @param aSat
         * @param aObserver The observer.
         */
        IMPORT_C void RegisterL(
            const RSatSession& aSatSession,
            MSatUiObserver* aObserver );

        /**
         * Returns a pointer to the object that implements the UI
         * adapter interface.
         * @return The adapter object pointer.
         */
        IMPORT_C MSatUiAdapter* Adapter() const;

        /**
         * Unregisters the listener object and disconnects from the server.
         */
        void Unregister();

        /**
         * The client API version
         * @return The version of the current API.
         */
        IMPORT_C TVersion Version() const;

        /**
         * Panics the client application
         * @param aReason The reason code.
         */
        IMPORT_C void Panic( TSatClientPanic aReason ) const;

        /**
         * Shuts down the server. Active in development use only.
         */
        IMPORT_C void KillServer() const;

        /**
        * Create and send service request message to SAT server.
        * @param aFunction Service request id.
        * @param aArgs Service request arguments.
        * @param aStatus Service request status.
        */
        IMPORT_C void CreateRequest(
            TInt aFunction,
            const TIpcArgs& aArgs,
            TRequestStatus& aStatus ) const;

        /**
         * Create and send service request message to SAT server.
         * @param aFunction Service request id.
         * @param aArgs Service request arguments.
         */
        IMPORT_C void CreateRequest(
            TInt aFunction,
            const TIpcArgs& aArgs ) const;

        /**
        * Returns a pointer to the object that implements client observer
        * object that receives SAT commands
        * @return The client observer object pointer.
        */
        IMPORT_C MSatUiObserver* SatUiObserver();

        /**
         * Returns a pointer to the object that implements Processor agent for
         * SAT commands.
         * @return The command processor object pointer.
         */
        CSatCCommandProcessor* CommandProcessor();

    public: // Functions from base classes

        /**
         * From RHandleBase
         *
         * Close the handle.
         */
        IMPORT_C void Close();

    private:

        // Prohibited copy constructor.
        RSatUiSession( const RSatUiSession& );

        // Prohibited assigment operator.
        RSatUiSession& operator= ( const RSatUiSession& );

        /**
        * Checks if the server is started.
        * @return Interger value, is server started or not.
        */
        TBool IsServerStarted() const;

    private: // Data

        /**
         * Processor agent for SAT commands
         */
        CSatCCommandProcessor* iCommandProcessor;

        /**
         * The client observer object that receives SAT commands.
         */        
        MSatUiObserver* iSatUiObserver;

    };

#endif // RSATUISESSION_H

// End of File
