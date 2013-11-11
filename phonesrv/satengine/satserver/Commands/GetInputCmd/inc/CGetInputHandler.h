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
* Description:  GetInput command handler
*
*/


#ifndef CGETINPUTHANDLER_H
#define CGETINPUTHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"

class MSatUtils;

/**
 *  Command handler for GetInput command.
 *
 *  @lib GetInputCmd.lib
 *  @since S60 v3.0
 */
class CGetInputHandler : public CSatCommandHandler
    {

public:

    static CGetInputHandler* NewL( MSatUtils* aUtils );

    virtual ~CGetInputHandler();

// from base class MSatCommand

    /**
     * From MSatCommand.
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
     * @return TBool indicating command is currently allowed.
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

    CGetInputHandler();

    void ConstructL();

private: // data

    /**
     * GetInput command data.
     */
    RSat::TGetInputV1 iGetInputData;

    /**
     * GetInput command package.
     */
    RSat::TGetInputV1Pckg iGetInputPckg;

    /**
     * Response from client.
     */
    RSat::TGetInputRspV1 iGetInputRsp;

    /**
     * Response package.
     */
    RSat::TGetInputRspV1Pckg iGetInputRspPckg;
                
    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CGETINPUTHANDLER_H

