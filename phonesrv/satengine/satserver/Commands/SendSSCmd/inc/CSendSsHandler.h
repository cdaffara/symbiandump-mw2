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
* Description:  Proactive SendSs command handler
*
*/


#ifndef CSENDSSHANDLER_H
#define CSENDSSHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class MSatUtils;
class CSatSendSsRequestCompleteHandler;
class CSatSendSsAdditionalInfoHandler;
class CPhoneGsmHandlerBase;
class CPhoneGsmParserBase;
class CPhoneGsmParserResult;
class CPhoneGsmOptionContainerBase;

/**
*  Command handler for SendSS command.
*
*  @lib SendSSCmd.lib
*  @since S60 v3.0
*/
class CSendSSHandler : public CSatCommandHandler
    {
    
public:

    static CSendSSHandler* NewL( MSatUtils* aUtils );

    virtual ~CSendSSHandler();
    
    /**
     * Processes the SS Request Complete.
     *
     * @param aErrCode Result of Ss sending
     */
    void DispatchSsRequestComplete( const TInt aErrCode );

    /**
     * Processes the SS aditional info.
     *
     * @param Addtional info.
     */
    void DispatchSsAdditionalInfo( const TDesC& aAdditionalInfo );

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

    CSendSSHandler();

    void ConstructL();

    /**
     * Handles the Ss string sending
     *
     */
    void SendSsStringL();

    /**
     * Handles the result of Ss sending
     *
     */
    void HandleSendSsResult();

    /**
     * Checks is the command transparent or not
     *
     * @return Boolean indicating is current command transparent to user
     */
    TBool TransparentSsSending();

    /**
     * Sends terminal response, if not yet sent
     *
     */
    void SendTerminalResponse();

    /**
     * Delay callback function. Called when predefined time
     * has passed. Used to wait SendSS AdditionalInfoNotification
     *
     * @param aPtr Pointer to creator
     * @return whether to call function again after an interval
     */
    static TInt SSRequestCallback( TAny* aPtr );

private: // data

    /**
     * SendSSCall command data.
     */
    RSat::TSendSsV1 iSendSsData;

    /**
     * SendSSCall command package.
     */
    RSat::TSendSsV1Pckg iSendSsPckg;

    /**
     * Response from client
     */
    RSat::TSendSsRspV2 iSendSsRsp;

    /**
     * Response package.
     */
    RSat::TSendSsRspV2Pckg iSendSsRspPckg;

    /**
     * UI Queries and notifications:
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
     * Ss specific
     */
    TInt iSsResult;

    /**
     * Indicates is the result arrived
     */
    TBool iRequestCompleteArrived;

    /**
     * Indicates is additional info arrived
     */
    TBool iAdditionalInfoArrived;

    /**
     * Additional info of SS message
     */
    TBuf<RSat::KAlphaIdMaxSize> iAdditionalInfo;

    /**
     * Ss additional info handler
     * Own.
     */
    CSatSendSsAdditionalInfoHandler* iAdditionalInfoHandler;

    /**
     * Ss request complete handler
     * Own.
     */
    CSatSendSsRequestCompleteHandler* iRequestCompleteHandler;

    /**
     * SS Parser used to process  parser result.
     * Own.
     */
    CPhoneGsmHandlerBase* iPhoneGsmHandlerBase;

    /**
     * Ss string parsers
     * Own.
     */
    CPhoneGsmParserBase* iParser;

    /**
     * Ss result
     * Own.
     */
    CPhoneGsmParserResult* iResult;

    /**
     * SS options used to update options.
     * Own.
     */
    CPhoneGsmOptionContainerBase* iPhoneGsmOptionContainerBase;

    /**
     * Indicates is terminal response sent
     */
    TBool iTerminalRespSent;

    /**
     * Indicates is UI needed
     */
    TBool iNeedUiSession;

    /**
     * Indicates is Confirm SAT Operations on
     */
    TBool iQueryOn;

    /**
     * delay timer
     * Own.
     */
    CPeriodic* iTimer;

    /**
     * Indicates wether user accepted or rejected
     */
   TBool iUserAccepted;

    /**
     * For delayed Ss sendinf
     */
    CActiveSchedulerWait iSendWait;
        
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;
    };

#endif      // CSENDSSHANDLER_H

