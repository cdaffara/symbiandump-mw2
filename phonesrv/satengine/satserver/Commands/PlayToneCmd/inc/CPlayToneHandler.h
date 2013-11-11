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
* Description:  Play Tone command handler
*
*/


#ifndef CPLAYTONEHANDLER_H
#define CPLAYTONEHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"

class MSatUtils;

/**
 *  Command handler for SelectItem command.
 *
 *  @lib PlayToneCmd.lib
 *  @since S60 v3.0
 */
class CPlayToneHandler : public CSatCommandHandler
    {
    
public:

    static CPlayToneHandler* NewL( MSatUtils* aUtils );

    virtual ~CPlayToneHandler();

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
     * Cancels the usat request
     *
     */
    void DoCancel();

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
     * Indicates the failure of launching ui client.
     *
     */
    void UiLaunchFailed();

private:

    CPlayToneHandler();

    void ConstructL();

private: // data

    /**
     * The data structure for the command data.
     */
    RSat::TPlayToneV2 iPlayToneData;

    /**
     * The data package for IPC.
     */
    RSat::TPlayToneV2Pckg iPlayTonePckg;

    /**
     * the response.
     */
    RSat::TPlayToneRspV1 iPlayToneRsp;

    /**
     * the response package.
     */
    RSat::TPlayToneRspV1Pckg iPlayToneRspPckg;

    /**
     * Flag to signal that command has icon data.
     *To be removed when icons are allowed in this command.
     */
    TBool iIconCommand;

    };

#endif      // CPLAYTONEHANDLER_H

