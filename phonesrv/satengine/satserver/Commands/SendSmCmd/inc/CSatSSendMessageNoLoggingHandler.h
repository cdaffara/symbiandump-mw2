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
* Description:  This is the handler for the SIM Application Toolkit
*                Send Message No
*
*/


#ifndef CSATSSENDMESSAGENOLOGGINGHANDLER_H
#define CSATSSENDMESSAGENOLOGGINGHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>

// FORWARD DECLARATIONS
class MSatApi;
class MSatSmsObserver;


// CLASS DECLARATION

/**
*  This is the handler for the Send Message No Logging.
*  This active object is registered with ETel SAT Api to receive notifications
*  about the completion of Send SMS message
*
*  @lib SendSmCmd.lib
*  @since Series 60 3.0
*/
class CSatSSendMessageNoLoggingHandler : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSatApi Reference to Sat api.
        * @param aObserver Observer of the sms end event.
        */
        static CSatSSendMessageNoLoggingHandler* NewL(
            MSatApi& aSatApi,
            MSatSmsObserver& aObserver );

        /**
        * Destructor.
        */
        virtual ~CSatSSendMessageNoLoggingHandler();

    public: // New functions

        /**
        * Starts listening for the Sat command.
        * @param aSendSmData Data of the proactive SendSm command.
        */
        void Start( const RSat::TSendSmV1& aSendSmData );

    protected: // Functions from base classes

        /**
        * From CActive, handles the request completion.
        */
        void RunL();

        /**
        * From CActive, implements the cancel protocol.
        */
        void DoCancel();

    private: // Constructors

        /**
        * C++ default constructor.
        * @param aSatApi Reference to Sat api.
        * @param aObserver Observer of the sms end event.
        */
        CSatSSendMessageNoLoggingHandler(
            MSatApi& aSatApi,
            MSatSmsObserver& aObserver );

    private: // Data

        // The data structure for the command data.
        RSat::TSatSmsV1 iSmsData;

        // The data package for IPC.
        RSat::TSatSmsV1Pckg iSmsPckg;

        // Sat api.
        MSatApi& iSatApi;

        // Observer of the sms sending
        MSatSmsObserver& iObserver;

        // Reference to SMS message.
        TUint16 iMessageRef;
    };

#endif      // CSATSSENDMESSAGENOLOGGINGHANDLER_H

// End of File
