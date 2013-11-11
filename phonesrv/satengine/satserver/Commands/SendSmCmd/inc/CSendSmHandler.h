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
* Description:  SendSm command handler
*
*/


#ifndef CSENDSMHANDLER_H
#define CSENDSMHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"
#include "MSatSmsObserver.h"

class MSatUtils;
class CSatSSendMessageNoLoggingHandler;

/**
*  Command handler for SendSm command.
*
*  @lib SendSmCmd.lib
*  @since S60 v3.0
*/
class CSendSmHandler : public CSatCommandHandler,
                       public MSatSmsObserver
    {
    
public:

    static CSendSmHandler* NewL( MSatUtils* aUtils );

    virtual ~CSendSmHandler();

// from base class MSatCommand

    /**
     * From MSatCommand. 
     * Query response.
     *
     */
    void ClientResponse();
    
// from base class MSatSmsObserver

    /**
     * From MSatEventObserver.
     * Event notification
     *
     * @param aEvent An event that is occured.
     */
    void Event( TInt aEvent );

    /**
     * From MSatSmsObserver.
     * Notification that sms was sent.
     *
     * @param aErrorCode Error code indicating a possible error.
     */
    void SmsSent( TInt aErrorCode );


protected:

    /**
     * Sets the sca number to the iSendSmData.
     *
     * @param aScaNumber SCA number.
     * @return Boolean indicating SCA number successfully set.
     */
    TBool SetScaNumber( const RSat::TSatTelNumber& aScaNumber );
    
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
     * From CSatCommandHandler
     * Called when UI launch fails
     *
     */
    void UiLaunchFailed();

private:

    CSendSmHandler();

    void ConstructL();

    /**
     * Checks is the command transparent or not
     *
     */
    TBool TransparentSmsSending() const;

    /**
     * Checks is the SCA available
     *
     */
    TBool IsSCAAvailable();

private: // data

    /**
     * SendSm command data.
     */
    RSat::TSendSmV1 iSendSmData;

    /**
     * SendSm command package.
     */
    RSat::TSendSmV1Pckg iSendSmPckg;

    /**
     * SendSm response data
     */
    RSat::TSendSmRspV1 iSendSmRsp;

    /**
     * SendSm response package.
     */
    RSat::TSendSmRspV1Pckg iSendSmRspPckg;

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
     * SendSm notification send data
     */
    TSatNotificationV1 iNotificationData;

    /**
     * SendSm notification package
     */
    TSatNotificationV1Pckg iNotificationDataPckg;

    /**
     * SendSm notification Response data
     */
    TSatNotificationRspV1 iNotificationRsp;

    /**
     * SendSm notification Response package
     */
    TSatNotificationRspV1Pckg iNotificationRspPckg;

    /**
     * Is MoSmControl active.
     */
    TBool iMoSmControlActive;

    /**
     * Message sender.
     */
    CSatSSendMessageNoLoggingHandler* iMsgSender;

    /**
     * Indicates does this command need UI session ot not
     */
    TBool iNeedUiSession;

    /**
     * Indicates if KPartialComprehension is needed instead of KSuccess
     */
    TBool iPartialComprehension;

    /**
     * Indicates is SCA number available at all
     */
    TBool iSCANumberAvailable;

    /**
     * Indicates is notification already sent
     */
    TBool iNotificationSent;
        
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CSENDSMHANDLER_H

