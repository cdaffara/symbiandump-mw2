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
* Description:  SendDtmf command handler
*
*/


#ifndef CSENDDTMFHANDLER_H
#define CSENDDTMFHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class CSatDtmfSender;

/**
 *  Command handler for SendDtmf command.
 *
 *  @lib SendDtmfCmd.lib
 *  @since S60 v3.0
 */
class CSendDtmfHandler : public CSatCommandHandler
    {

public:

    static CSendDtmfHandler* NewL( MSatUtils* aUtils );

    virtual ~CSendDtmfHandler();

    /**
     * Gets the result of DTMF sending.
     *
     * @since S60 3.2
     * @param aErrCode result of completed DTMF sending.
     */
    void SendDtmfComplete( const TInt aErrCode );

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
     * From CSatCommandHandler
     * Called when UI launch fails
     *
     */
    void UiLaunchFailed();

private:

    CSendDtmfHandler();

    void ConstructL();

    /**
     * Converts extended BCD string into UCS2 string
     *
     * @param aDtmfString destination string, to contain DTMF as UCS2
     * @return ETrue if DTMF string in iSendDtmfData contains valid DTMF characters only
     *         (0-9,*,#,pause)
     */
    TBool ConvertDtmfStringToUcs2( TDes& aDtmfString );

    /**
     * converts an extended BCD character according to GSM 11.11 into a UCS2 character
     *
     * @param aExtBcd extended BCD character
     * @param aEndOfString ETrue iff aExtBcd signals end of string
     * @param aDtmfCharacter ETrue iff aExtBcd represents DTMF character (0-9,*,#,pause)
     * @param aUcs2Char DTMF character in UCS2 representation - only valid if aDtmfCharacter is true
     */
    void ConvertDtmfCharacterToUcs2(
        const TChar& aExtBcd,
        TBool& aEndOfString,
        TBool& aDtmfCharacter,
        TChar& aUcs2Char ) const;

    /**
     * Handles the result of DTMF sending and sends terminal response
     *
     */
    void HandleResult();

    /**
     * Sends terminal response. Makes sure that terminal response
     * is not send more that once / command.
     *
     */
    void SendTerminalResponse();

private: // data

    /**
     * SendDtmf command data from SIM.
     */
    RSat::TSendDtmfV1 iSendDtmfData;

    /**
     * SendDtmf command package.
     */
    RSat::TSendDtmfV1Pckg iSendDtmfPckg;

    /**
     * SendDtmf response data from client.
     */
    RSat::TSendDtmfRspV1 iSendDtmfRsp;

    /**
     * SendDtmf response package
     */
    RSat::TSendDtmfRspV1Pckg iSendDtmfRspPckg;

// UI notification data

    /**
     * SendDtmf notification send data
     */
    TSatNotificationV1 iNotificationData;

    /**
     * SendDtmf notification package
     */
    TSatNotificationV1Pckg iNotificationDataPckg;

    /**
     * SendDtmf notification Response data
     */
    TSatNotificationRspV1 iNotificationRsp;

    /**
     * SendDtmf notification Response package
     */
    TSatNotificationRspV1Pckg iNotificationRspPckg;

    /**
     * DTMF handling
     */
        CSatDtmfSender* iDtmfSender;

    /**
     * Result of completed DTMF sending.
     */
    TInt iDtmfResult;

    /**
     * Indicates is terminal response sent
     */
    TBool iTerminalRespSent;

    /**
     * Indicates do we need UI
     */
    TBool iNeedUiSession;

    /**
     * Converted DTMF string container.
     * Own.
     */
    HBufC16* iDtmfStringUcs2;

    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    /**
     * Wait scheduler for fast UI event cases.
     */
    CActiveSchedulerWait iWait;
     
    /**
     * Indicates if client response is received.
     */
    TBool iClientResponseReceived;
     
    };

#endif      // CSENDDTMFHANDLER_H

