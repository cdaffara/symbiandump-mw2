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
* Description:  CloseChannel command handler
*
*/



#ifndef CCLOSECHANNELHANDLER_H
#define CCLOSECHANNELHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for CloseChannel command.
*
*  @lib CloseChannelCmd.lib
*  @since Series 60 v3.0
*/
class CCloseChannelHandler : public CSatCommandHandler
    {

public:

    static CCloseChannelHandler* NewL( MSatUtils* aUtils );

    virtual ~CCloseChannelHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
     *
     */
    void ClientResponse();

protected:
    
// from base class CActive

    /**
     * From CActive.
     * Cancels the usat request.
     *
     */
    void DoCancel();
    
// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler.
     * Requests the command notification.
     *
     * @param aStatus Request status to active object notification
     */
    void IssueUSATRequest( TRequestStatus& aStatus );

    /**
     * From CSatCommandHandler.
     * Precheck before executing the command.
     *
     * @return TBool indicating is this command allowed to execute.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler.
     * Need for ui session.
     *
     * @return TBool indicating does this command need UI session.
     */
    TBool NeedUiSession();

    /**
     * From CSatCommandHandler.
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

    CCloseChannelHandler();

    /**
     * Sends notification to UI.
     *
     * @return KErrNone if notification is sent successfully
     */
    TInt SendUiNotification();

private: // data

    /**
     * CloseChannel command data
     */
    RSat::TCloseChannelV2 iCloseChannelData;

    /**
     * CloseChannel command package
     */
    RSat::TCloseChannelV2Pckg iCloseChannelPckg;

    /**
     * CloseChannel response data
     */
    RSat::TCloseChannelRspV2 iCloseChannelRspData;

    /**
     * CloseChannel response package
     */
    RSat::TCloseChannelRspV2Pckg iCloseChannelRspPckg;

    /**
     * Data to UI
     */
    TSatNotificationV1 iCloseChannelUiData;

    /**
     * IPC Package to ui data
     */
    TSatNotificationV1Pckg iCloseChannelUiPckg;

    /**
     * UI Response
     */
    TSatNotificationRspV1 iCloseChannelUiRsp;

    /**
     * IPC Package to response
     */
    TSatNotificationRspV1Pckg iCloseChannelUiRspPckg;

    /**
     * Flag to signal client response that handle command is not done
     */
    TBool iWaitHandleCloseCommand;

    /**
     * Flag to signal that icon response is needed before terminal
     * response is done
     */
    TBool iWaitClientIconResponse;
        
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CCLOSECHANNELHANDLER_H

