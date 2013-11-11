/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  OpenChannel command handler
*
*/


#ifndef COPENCHANNELHANDLER_H
#define COPENCHANNELHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "MSatBIPDataChannel.h"
#include "MSatBIPUtils.h"
#include "SatSTypes.h"
#include "MSatConnectionObserver.h"

class MSatUtils;
class MSatBIPDataChannel;

/**
 *  Command handler for OpenChannel command.
 *
 *  @lib OpenChannelCmd.lib
 *  @since S60 v3.0
 */
class COpenChannelHandler : public CSatCommandHandler,
                            public MSatConnectionObserver
    {

public:

    static COpenChannelHandler* NewL( MSatUtils* aUtils );

    virtual ~COpenChannelHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
     *
     */
    void ClientResponse();
    
// from base class MSatConnectionObserver

    /**
     * From MSatConnectionObserver.
     * Notification from the connection.
     *
     * @param aError Indicates the status of connection
     */
    void ConnectionNotification( TInt aError );
    
// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler.
     * Event notification.
     *
     * @param aEvent Identifies the event.
     */
    void Event( TInt aEvent );

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
     * @return Boolean indicating is this command allowed to execute.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler.
     * Need for ui session.
     *
     * @return Boolean indicating does this command need UI session.
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

    COpenChannelHandler();

    void ConstructL();

    /**
     * Sets new connection settings for channel to open.
     *
     * @param aBearerType Bearer type for new connection.
     */
    void SetConnectionSettingsL(
        MSatBIPUtils::TSatBearerType aBearerType );

    /**
     * Sets override settings into connection information.
     *
     * @param aIapId Unique ID in the IAP table.
     */
    void SetOverrideSettingsL( const TUint32 aIapId );

    /**
     * Sets up data channel. Does not activate context.
     *
     * @return Error code indicating connection settings setup
     */
    TInt SetUpChannel();

    /**
     * Activates context.
     *
     * @return Error code indicating the activation.
     */
    TInt ActivateChannel();

    /**
     * Creates and sends the terminal response on successful cases.
     *
     */
    void SendSuccessTerminalRsp();

    /**
     * Sends confirmation query to UI.
     *
     */
    void SendConfirmToUi();

    /**
     * Closes the channel in error cases.
     *
     */
    void CloseChannel();

    /**
     * Checks parameters from SIM and makes modifications, if needed.
     *
     * @return TBool indicating is command allowed after parameters checked.
     */
    TBool CheckParameters();

    /**
     * Launches UI if it not active.
     *
     * @return Boolean value indicating did the function launch the UI or not.
     */
    TBool LaunchUiSession();

private: // data

    /**
     * OpenChannel command data from SIM.
     */
    RSat::TOpenGprsChannelV4 iOpenChannelData;

    /**
     * OpenChannel command package.
     */
    RSat::TOpenGprsChannelV4Pckg iOpenChannelPckg;

    /**
     * OpenChannel response data from client.
     */
    RSat::TOpenChannelRspV2 iOpenChannelRsp;

    /**
     * OpenChannel response package.
     */
    RSat::TOpenChannelRspV2Pckg iOpenChannelRspPckg;

    /**
     * Query command data.
     */
    TSatQueryV1 iQueryData;

    /**
     * Query package.
     */
    TSatQueryV1Pckg iQueryPckg;

    /**
     * Query response.
     */
    TSatQueryRspV1 iQueryRsp;

    /**
     * Query rsp package.
     */
    TSatQueryRspV1Pckg iQueryRspPckg;

    /**
     * Commands PCmd. depends on requested bearer type.
     */
    RSat::TPCmd iPCmd;

    /**
     * Data channel.
     * Not own.
     */
    MSatBIPDataChannel* iDataChannel;

    /**
     * Indicates did the channel setup need modifications.
     */
    TBool iSetUpWithModifications;

    /**
     * Indicates does this command need UI session.
     */
    TBool iNeedUiSession;

    /**
     * Bearer type.
     */
    MSatBIPUtils::TSatBearerType iBearerType;

    /**
     * Connection information.
     */
    MSatBIPDataChannel::TSatBipConnectionInfo iConnectionInfo;

    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // COPENCHANNELHANDLER_H

