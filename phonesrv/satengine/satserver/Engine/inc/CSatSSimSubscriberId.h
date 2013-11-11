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
* Description:  This is the handler for the SIM Application Toolkit
*                to dedect the SIM card subscriber id.
*
*/


#ifndef CSATSSIMSUBSCRIBERID_H
#define CSATSSIMSUBSCRIBERID_H

#include <etelsat.h>
#include "CSatSServer.h"
#include "MSatSSimSubscriberId.h"

class TSatEventMediator;
class CSatSActiveWrapper;
class MSatMultiModeApi;

/**
*  This is the handler for the SIM card status.
*  This active objects is registered to listen SIM card status,
*  especially SIM card removed status.
*/

class CSatSSimSubscriberId : public CActive,
                             public MSatSSimSubscriberId
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aPhone a reference to MSatMultiModeApi
        * @param aEventMediator Event handler class
        * @return a pointer to the newly created active object.
        */
        static CSatSSimSubscriberId* NewL(
            MSatMultiModeApi& aPhone,
            TSatEventMediator& aEventMediator );

        /**
        * Destructor.
        */
        virtual ~CSatSSimSubscriberId();

    public: // New functions

        /**
        * Starts the handler.
        */
        void Start();

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
        * @param aPhone a reference to MSatMultiModeApi.
        * @param aEventMediator Event handler class.
        */
        CSatSSimSubscriberId(
            MSatMultiModeApi& aPhone,
            TSatEventMediator& aEventMediator );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // New functions

        /**
        * Starts listening for the SIM card subscriber id.
        */
        void DoStart();

        /**
        * Checks whether special SIM is being used.
        * @return TBool indicating special SIM.
        */
        TBool IsCmccSIM() const;

    private: // Data

        // MSatMultiModeApi instance.
        MSatMultiModeApi& iPhone;

        // Event mediator.
        TSatEventMediator& iEventMediator;

        // Indicates whether subscriber id below is successfully read.
        TBool iSubscriberIdValid;

        // Subscriber ID data.
        RMobilePhone::TMobilePhoneSubscriberId iSubscriberId;

        // Provides asynchronous timer for thread suspension.
        CSatSActiveWrapper* iWrapper;

        // Counter to limít subscriber id read retries.
        TInt iRetryCounter;

    };

#endif      // CSATSSIMSUBSCRIBERID_H
