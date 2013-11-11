/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SendUssd command handler
*
*/


#ifndef CSENDUSSDHANDLER_H
#define CSENDUSSDHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class MSatUtils;
class CPhCltUssdSatClient;

/**
*  Command handler for SendUssd command.
*
*  @lib SendUssd.lib
*  @since S60 v3.0
*/
class CSendUssdHandler : public CSatCommandHandler
    {
    
public:

    static CSendUssdHandler* NewL( MSatUtils* aUtils );

    virtual ~CSendUssdHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
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
     * @param aStatus Request status
     */
    void IssueUSATRequest( TRequestStatus& aStatus );

    /**
     * From CSatCommandHandler
     * Precheck before executing the command.
     *
     * @return TBool indicating command is currently allowed.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler
     * Need for ui session.
     *
     * @return TBool indicating need of ui session.
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
     * Indicates the failure of launching ui client
     *
     */
    void UiLaunchFailed();

private:

    CSendUssdHandler();

    /**
     * Handles the Ussd string sending
     *
     */
    void SendUssdString();

    /**
     * Handles the Ussd string sending
     *
     * @param aSendMessage The USSD string to be sent
     * @param aSendDcs Data coding scheme of the USSD string
     * @param aReceiveMessage Response message from NW
     * @param aSendCompletedFirst Indicates is this function completed
     *        before response
     * @param aReceivedDcs Data coding scheme of the received USSD string
     */
    void SendUssdStringL(
        const TDesC& aSendMessage,
        const TUint8 aSendDcs,
        TDes& aReceiveMessage,
        TBool& aSendCompletedFirst,
        TUint8& aReceivedDcs );

    /**
     * Show the ussd response note.
     *
     * @param aText The USSD response string to be shown.
     */
     void ShowUssdResponseNoteL( const TDesC& aText );

    /**
     * Handles the result of Ussd sending
     *
     * @param aError Result of Ussd sending
     */
    void HandleSendUssdResult( TInt aError );

    /**
     * Converts reveived Dcs to correct format
     *
     * @param aReceivedDcs Received Dcs to convert
     */
    void ConvertReceivedDcs( TUint8& aReceivedDcs ) const;

    /**
     * Sends terminal response. Makes sure that terminal response
     * is not send more that once / command.
     *
     */
    void SendTerminalResponse();

    /**
     * Checks is the command transparent or not
     *
     */
    TBool TransparentUssdSending();
    
    /**
     * Check whether the ussd sending result is displayed
     * @return 1 indicating the result is displayed
     */
    TInt SatDisplayUssdResultL();

private: // data

    /**
     * SendUssd command data.
     */
    RSat::TSendUssdV1 iSendUssdData;

    /**
     * SendUssd command package.
     */
    RSat::TSendUssdV1Pckg iSendUssdPckg;

    /**
     * Response from client
     */
    RSat::TSendUssdRspV1 iSendUssdRsp;

    /**
     * Response package.
     */
    RSat::TSendUssdRspV1Pckg iSendUssdRspPckg;

    /**
     * UI query and notification data:
     */
    TBool iNeedUiSession;

    /**
     * Indicates the query status
     */
    TBool iQueryOn;

    /**
     * Indicates is AlphaId present
     */
    TBool iAlphaIdPresent;

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
     * Notification send data
     */
    TSatNotificationV1 iNotificationData;

    /**
     * Notification package
     */
    TSatNotificationV1Pckg iNotificationDataPckg;

    /**
     * Notification Response data
     */
    TSatNotificationRspV1 iNotificationRsp;

    /**
     * Notification Response package
     */
    TSatNotificationRspV1Pckg iNotificationRspPckg;

    /**
     * Indicates is notification sent
     */
    TBool iNotificationSent;

    /**
     * Indicates is terminal response sent
     */
    TBool iTerminalRespSent;

    /**
     * Ussd sender
     */
    CPhCltUssdSatClient* iUssdClient;

    /**
     * Blocks the USSD send until user has respond
     */
    CActiveSchedulerWait iWait;

    /**
     * Indicates wether user accepted or rejected
     */
    TBool iUserAccepted;
    
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    /**
     * Indicates whether the result of sending USSD is displayed
     */
    TBool iIsSatDisplayUssdResult;
    };

#endif      // CSENDUSSDHANDLER_H

