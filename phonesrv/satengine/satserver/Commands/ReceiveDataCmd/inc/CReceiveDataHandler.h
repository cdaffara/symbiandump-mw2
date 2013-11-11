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
* Description:  ReceiveData command handler
*
*/


#ifndef CRECEIVEDATAHANDLER_H
#define CRECEIVEDATAHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"
#include "MSatBIPUtils.h"

class MSatUtils;

/**
*  Command handler for ReceiveData command.
*
*  @lib ReceiveDataCmd.lib
*  @since S60 v3.0
*/
class CReceiveDataHandler : public CSatCommandHandler
    {

public:

    static CReceiveDataHandler* NewL( MSatUtils* aUtils );

    virtual ~CReceiveDataHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client.
     *
     */
    void ClientResponse();

// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler
     * Overrides the default implementation.
     *
     * @param aEvent Identifies the event.
     */
    void Event( TInt aEvent );

protected:

// from base class CActive

    /**
     * From CActive
     * Cancels the usat request.
     *
     */
    void DoCancel();

// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler
     * Requests the command notification.
     *
     * @param aStatus Request status to active object notification
     */
    void IssueUSATRequest( TRequestStatus& aStatus );

    /**
     * From CSatCommandHandler
     * Precheck before executing the command.
     *
     * @return Boolean indicating is this command allowed to execute.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler
     * Need for ui session.
     *
     * @return Boolean indicating does this command need UI session.
     */
    TBool NeedUiSession();

    /**
     * From CSatCommandHandler
     * Called when USAT API notifies that command.
     *
     */
    void HandleCommand();

    /**
     * From CSatCommandHandler.
     * Indicates the failure of launching ui client.
     *
     */
    void UiLaunchFailed();

private:

    CReceiveDataHandler();

    void ConstructL();

    /**
     * Sends notification to UI.
     *
     * @return KErrNone if notification is sent successfully
     */
    TInt SendUiNotification();

    /**
     * Completes Terminal response and sends it.
     *
     * @param aError is error code of reason
     */
    void SendTerminalResponse(
        const TInt aError = MSatBIPUtils::ESatBIPSuccess );

private: // data

    /**
     * ReceiveData command data.
     */
    RSat::TReceiveDataV2 iReceiveDataData;

    /**
     * ReceiveData command package.
     */
    RSat::TReceiveDataV2Pckg iReceiveDataPckg;

    /**
     * ReceiveData response data.
     */
    RSat::TReceiveDataRspV2 iReceiveDataRspData;

    /**
     * ReceiveData response package.
     */
    RSat::TReceiveDataRspV2Pckg iReceiveDataRspPckg;

    /**
     * The data structure for notification.
     */
    TSatNotificationV1 iReceiveDataUiData;

    /**
     * Nitificatio package.
     */
    TSatNotificationV1Pckg iReceiveDataUiPckg;

    /**
     * Notification response.
     */
    TSatNotificationRspV1 iReceiveDataUiRsp;

    /**
     * Notification response package.
     */
    TSatNotificationRspV1Pckg iReceiveDataUiRspPckg;

    /**
     * Reiceive data buffer.
     */
    TBuf8<KSatBIPMaxResponseSize> iChData;

    /**
     * Remaining amount of data in RX-buffer.
     */
    TInt iChDataLength;

    /**
     * Indicates has user cancelled the action.
     */
    TBool iUserCancelled;

    /**
     * Indicates does current command need UI or not.
     */
    TBool iNeedUiSession;

    /**
     * Flag to signal that command has icon data.
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CRECEIVEDATAHANDLER_H
