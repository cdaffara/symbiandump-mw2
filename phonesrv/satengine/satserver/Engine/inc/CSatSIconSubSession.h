/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the icon sub-session IPC.
*
*/


#ifndef CSATSICONSUBSESSION_H
#define CSATSICONSUBSESSION_H

#include <etelsat.h>

#include "CSatSSubSession.h"
#include "MSatIconObserver.h"

class MSatSIconAPI;
class CSatSSession;
class CFbsBitmap;
class TSatEventMediator;

/**
*  Handles the icon commands.
*
*  @lib SatEngine.lib
*  @since 3.0
*/
class CSatSIconSubSession : public CSatSSubSession,
                            public MSatIconObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSession Parent session.
        * @param aEventMediator Passes SetUpIdleMode response event.
        */
        static CSatSIconSubSession* NewL(
            CSatSSession& aSession,
            TSatEventMediator& aEventMediator );

        /**
        * Destructor.
        */
        virtual ~CSatSIconSubSession();

    private: // New functions

        /**
        * Gets the icon info from SIM.
        */
        void GetIconInfo();

        /**
        * Gets the icon from SIM.
        */
        void GetIconInstanceL();

        /**
        * Writes to client address space. Panics the client
        * if writing fails.
        * @param aMessage Message, which to write.
        * @param aParam Parameter number
        * @param aDes Descriptor to be written.
        */
        void Write(
            const RMessage2& aMessage,
            TInt aParam,
            const TDesC8& aDes ) const;

    public: // Functions from base classes

        /**
        * From CSatSSubSession
        * @param aFunction Function.
        * @param aMessage IPC message.
        */
        void HandleCommandL( const RMessage2& aMessage );

        /**
        * From MSatIconObserver.
        * Notification of getting icon info from SIM.
        */
        void NotifyGetIconInfo();

        /**
        * From MSatIconObserver.
        * Notification of getting icon image.
        */
        void NotifyGetIconInstanceL();

        /**
        * From MSatIconObserver.
        * Notification of getting icon color lookup table.
        */
        void NotifyGetClutL();

        /**
        * From MSatIconObserver.
        * Notification of operation failure
        * @param aErrorCode Reason of failure.
        */
        void NotifyFailure( TInt aErrorCode );

        /**
        * From MSatIconObserver.
        * Error completion for handler
        * @param aError Received error code
        * @return Result of completion
        */
        TInt CompleteError( TInt aError ) const;
        
        /**
        * Get icon fetching mode which is in synchronous or asynchronous mode.
        * @return Icon fetching mode.
        */
        TBool IconFetchingMode();
        
        /**
        * Notify CSatIconHandler to fetch icons from NAA synchronously or 
        * asynchrounously.
        * @param aMode Icon fetching is in synchronously or asynchronously
        * @return None
        */
        void NotifyIconFetchingMode( TBool aMode );

    private:

        /**
        * ConstrucL
        */
        void ConstructL();

        /**
        * C++ default constructor.
        * @param aSession Parent session.
        * @param aEventMediator Passes SetUpIdleMode response event.
        */
        CSatSIconSubSession(
            CSatSSession& aSession,
            TSatEventMediator& aEventMediator );

        /**
        * Delivers SetUpIdleModeText response to corresponding command handler.
        * @param aResponse Idle mode response.
        */
        void SendSetUpIdleModeResponse( TInt aResponse ) const;

    private:    // Data

        // Interface for accessing icons in SIM.
        MSatSIconAPI* iIconAPI;

        // Bitmap instance.
        CFbsBitmap* iBitmap;

        // Message just being executed.
        RMessage2 iMessage;

        // Icon info got from SIM.
        RSat::TIcon iIconEf;

        // Icon info from client.
        RSat::TInstanceInfoV3 iInfo;

        // Icon data.
        HBufC8* iIconData;

        // Icon color lookup table.
        HBufC8* iClut;

        // Event mediator to send SetUpIdleMode response
        TSatEventMediator& iEventMediator;

    };

#endif      // CSATSICONSUBSESSION_H
