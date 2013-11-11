/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles client requests.
*
*/



#ifndef CSATCLIENTSERVICEREQ_H
#define CSATCLIENTSERVICEREQ_H

//  INCLUDES
#include <e32base.h>
#include "MSatCommand.h"
#include "SatSOpcodes.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Handles the buffering of command and client request.
*  Buffering is needed because client can make request without command and
*  viceversa.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class CSatClientServiceReq : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aRequest Handled service request
        * @param aResponse Handled response request.
        * @param aCommand Command hander for request
        * @param aUtils Reference to SAT Utilities
        */
        static CSatClientServiceReq* NewL(
            TSatServerRequest aRequest,
            TSatServerRequest aResponse,
            MSatCommand* aCommand,
            MSatUtils& aUtils );

        /**
        * Destructor.
        */
        virtual ~CSatClientServiceReq();

    public: // New functions

        /**
        * Handles the command from SIM.
        * @param aCmdData Data to be sent to client.
        * @param aCmdRsp Clients response is written to this descriptor.
        * @param aRequest Request which the data is sent.
        * @return ETrue if command was recognised to be handled in this class ie aRequest is the same that
        * set in constructor.
        */
        virtual TBool HandleCommand(
            TDesC8* aCmdData,
            TDes8* aCmdRsp,
            TSatServerRequest aRequest );

        /**
        * Handles the request from client.
        * @param aRequest Service request from client.
        * @return ETrue if request was recognised to be handled in this class.
        */
        virtual TBool HandleRequest( const RMessage2& aRequest );

        /**
        * Checks is this service request handling this request
        * @param aRequest Checks own request to this request
        * @return TBool indicating is param aRequest this service
        * requests request
        */
        virtual TBool IsMyRequest( const TSatServerRequest aRequest );

        /**
        * Sets command handler for this service request handler
        * @param aCommand Command handler for this service request
        */
        virtual void SetCommandHandler( MSatCommand* aCommand );

        /**
        * Resets containing data.
        */
        virtual void Reset();

    protected:  // New functions

        /**
        * Writes the data to client address space and
        * completes the iRequest.
        */
        virtual void SendDataToClient();

    protected:

        /**
        * C++ default constructor.
        * @param aRequest Handled service request
        * @param aResponse Handled response request.
        * @param aCommand Command hander for request
        * @param aUtils Reference to SAT Utilities
        */
        CSatClientServiceReq( 
            TSatServerRequest aRequest,
            TSatServerRequest aResponse,
            MSatCommand* aCommand,
            MSatUtils& aUtils );

    private:  // Data

        // Identifies the client request, which is handled in HandleRequest
        const TSatServerRequest iHandledRequest;

        // Identifies the client response, which is handled in HandleRequest
        const TSatServerRequest iHandledResponse;

        // Command from SIM
        MSatCommand* iResponseObserver;

        // Indicates if the command from SIM is not yet sent to client
        TBool iCmdPending;

        // Service request from client
        RMessage2 iRequest;

        // Indicates that request is waiting command from SIM.
        TBool iRequestPending;

        // Data from SIM.
        TDesC8* iCmdData;

        // Response from client..
        TDes8* iCmdRsp;

        // Utilities class
        MSatUtils& iUtils;
    };

#endif      // CSATCLIENTSERVICEREQ_H

// End of File
