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
* Description:  GetInkeyHandler
*
*/


#ifndef CGETINKEYHANDLER_H
#define CGETINKEYHANDLER_H

#include "CSatCommandHandler.h"

#include <etelsat.h>

class MSatUtils;

/**
 *  Command handler for GetInkey command.
 *
 *  @lib GetInkeyCmd.lib
 *  @since S60 v3.0
 */
class CGetInkeyHandler : public CSatCommandHandler
    {

public:

    static CGetInkeyHandler* NewL( MSatUtils* aUtils );

    virtual ~CGetInkeyHandler();

// from base class MSatCommand

    /**
     * From MSatCommand
     * Response from the client.
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
     * @param aStatus Request status
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
     * @return TBool indicating need of ui session.
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

    CGetInkeyHandler();

    void ConstructL();

private: // data

    /**
     * SetUpCall command data.
     */
    RSat::TGetInkeyV2 iGetInkeyData;

    /**
     * SetUpCall command package.
     */
    RSat::TGetInkeyV2Pckg iGetInkeyPckg;

    /**
     * Response from client.
     */
    RSat::TGetInkeyRspV2 iGetInkeyRsp;

    /**
     * Response package.
     */
    RSat::TGetInkeyRspV2Pckg iGetInkeyRspPckg;
    
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CGETINKEYHANDLER_H

