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
* Description:  CallControl command handler
*
*/



#ifndef CCALLCONTROLHANDLER_H
#define CCALLCONTROLHANDLER_H

#include <etelsat.h>
#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class MSatUtils;
class CCallControlRequestHandler;

/**
*  Command handler for CallControl command.
*
*  @lib CallControlCmd.lib
*  @since Series 60 3.0
*/
class CCallControlHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface
        */
        static CCallControlHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CCallControlHandler();

    public: // Functions from base classes

        /**
        * From MSatCommand.
        */
        void ClientResponse();
        
    public:// New Functions
         
        /**
        * Processes the Cc Request Complete.
        *
        * @param aErrCode Result of ETelMM Api calling.
        */
        void DispatchCcRequestComplete( const TInt aErrCode );
         
        /**
        * TCcStatus
        * Call control status, when there is an asynchronous
        * operation is handling by the CCallControlRequestHandler
        * we will save some status information
        */
        enum TCcStatus
            {
            ECcIdle = 0,
            ECcDialWaiting,
            ECcSendSsWaiting,
            ECcSendUssdWaiting
            };
            
    protected:  // Functions from base classes

        /**
        * From CActive Cancels the usat request.
        */
        void DoCancel();

        /**
        * From CSatCommandHandler Requests the command notification.
        * @param aStatus Request status to active object notification
        */
        void IssueUSATRequest( TRequestStatus& aStatus );

        /**
        * From CSatCommandHandler Precheck before executing the command.
        * @return Boolean indicating is this command allowed to execute.
        */
        TBool CommandAllowed();

        /**
        * From CSatCommandHandler Need for ui session.
        * @return Boolean indicating does this command need UI session.
        */
        TBool NeedUiSession();

        /**
        * From CSatCommandHandler Called when USAT API notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler. Indicates the failure of launching ui client
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CCallControlHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        
        /**
        * If it is needed that sat makes a no FDN check call or 
        * send SS or send USSD we should perform these operation 
        */
        void CallControlClientOperation();
        
        /**
        * If SAT needs to do Call Control next action, 
        * i.e. Call Control has modified the type of request,
        * then this method should be called
        */
        TInt DialNumber();
    
        /**
        * Make a no FDN check Send SS
        */
        TInt SendSs();

        /**
        * Make a no FDN check Send USSD
        */
        TInt SendUssd();
        
        /**
         * Notify session done, start recieve more command.
         */
        void CompleteCallControlCmd( TInt aErrCode );
        
    private:    // Data

        // CallControl command data.
        RSat::TCallControlV6 iCallControlData;
        // CallControl command package.
        RSat::TCallControlV6Pckg iCallControlPckg;
        // CallControl send data
        TSatNotificationV1 iCallControlSendData;
        // CallControl SendData package
        TSatNotificationV1Pckg iCallControlSendDataPckg;
        // CallControl UI Response data
        TSatNotificationRspV1 iCallControlUiRespData;
        // CallControl UI Response package
        TSatNotificationRspV1Pckg iCallControlUiRespDataPckg;
        // Indicates is UI needed or not
        TBool iNeedUiSession;
        // Handing the asynchronous request
        CCallControlRequestHandler* iRequestHandler;
        // Current status
        TCcStatus iCcStatus;

    };

#endif      // CCALLCONTROLHANDLER_H


