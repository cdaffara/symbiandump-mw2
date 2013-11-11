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
* Description:  SendData command handler
*
*/


#ifndef CSENDDATAHANDLER_H
#define CSENDDATAHANDLER_H

#include <etelsat.h>

#include "MSatSendDataObserver.h"
#include "CSatCommandHandler.h"
#include "SatSTypes.h"
#include "MSatConnectionObserver.h"
#include "MSatBIPUtils.h"
#include "MSatEventObserver.h"

class MSatUtils;
class MSatBIPDataChannel;

/**
 *  Command handler for SendData command.
 *
 *  @lib SendDataCmd.lib
 *  @since S60 v3.0
 */
class CSendDataHandler : public CSatCommandHandler,
                         public MSatSendDataObserver,
                         public MSatConnectionObserver
    {

public:

    static CSendDataHandler* NewL( MSatUtils* aUtils );

    virtual ~CSendDataHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
     *
     */
    void ClientResponse();
    
// from base class MSatSendDataObserver

    /**
     * From MSatSendDataObserver
     * Notification of data has been sent
     *
     * @param aError Indicates the status of data sent.
     * @param aLength Number of bytes sent
     */
    void DataSentNotification( const TInt aError,
                               const TInt aLength );

    /**
     * From MSatConnectionObserver
     * Notfication from the connection
     *
     * @param aError Indicates the status of connection
     */
    void ConnectionNotification( TInt aError );

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

    CSendDataHandler();

    void ConstructL();

    /**
     * Sends data
     *
     */
    void SendData();

    /**
     * Completes Terminal response and sends it
     *
     * @param aError is error code of reason
     */
    void SendTerminalResponse(
        const TInt aError = MSatBIPUtils::ESatBIPSuccess );

    /**
     * Sends notification to UI
     *
     * @return KErrNone if notification is sent successfully
     */
    TInt SendUiNotification();

private: // data

    /**
     * SendData command data
     */
    RSat::TSendDataV2 iSendDataData;

    /**
     * SendData command package
     */
    RSat::TSendDataV2Pckg iSendDataPckg;

    /**
     * SendData response data
     */
    RSat::TSendDataRspV2 iSendDataRspData;

    /**
     * SendData response package
     */
    RSat::TSendDataRspV2Pckg iSendDataRspPckg;

    /**
     * Data to UI
     */
    TSatNotificationV1 iSendDataUiData;

    /**
     * IPC Package to ui data
     */
    TSatNotificationV1Pckg iSendDataUiPckg;

    /**
     * UI Response
     */
    TSatNotificationRspV1 iSendDataUiRsp;

    /**
     * IPC Package to response
     */
    TSatNotificationRspV1Pckg iSendDataUiRspPckg;

    /**
     * Remaining space in TX buffer
     */
    TInt iFreeBuffSize;

    /**
     * Pointer to DataChannel
     */
    MSatBIPDataChannel* iDataChannel;

    /**
     * Indicates has user cancelled the action
     */
    TBool iUserCancelled;

    /**
     * Indicates does current command need UI or not
     */
    TBool iNeedUiSession;

    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CSENDDATAHANDLER_H

