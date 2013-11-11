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
* Description:  SetUpCall command handler
*
*/


#ifndef CSETUPCALLHANDLER_H
#define CSETUPCALLHANDLER_H

#include <etelsat.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

class CSetupCallRequestHandler;

/**
*  Command handler for SetUpCall command.
*
*  @lib SetUpCallCmd.lib
*  @since S60 v3.0
*/
class CSetUpCallHandler : public CSatCommandHandler
    {

public:

    static CSetUpCallHandler* NewL( MSatUtils* aUtils );

    virtual ~CSetUpCallHandler();

// from base class MSatEventObserver

    /**
     * From MSatEventObserver.?
     * Event notification.
     *
     * @param aEvent Identifies the event.
     */
    void Event( TInt aEvent );

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
     *
     */
    void ClientResponse();

    /**
    * Handle the setup call Request Complete.
    *
    * @param aErrCode Result of ETelMM Api calling.
    */
    void SetupCallRequestComplete( const TInt aErrCode );

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
     * Indication that UI launching failed.
     *
     */
    void UiLaunchFailed();

private:

    CSetUpCallHandler();

    void ConstructL();

    /**
     * Performs the request to dial
     *
     * @since S60 3.2
     */
    void DoSetupCall( CSetupCallRequestHandler& aHandler );

    /**
     * Return terminal response filled according to dial result.
     *
     * @since S60 3.2
     * @param aStatus Dial status from phone.
     */
    void CompleteSetupCallWithStatus(
        const TInt32 aStatus );

    /**
     * Return terminal response filled according to result.
     *
     * @since S60 3.2
     * @param aGeneralResult Completion status.
     */
    void CompleteSetupCall(
        const RSat::TPCmdResult aGeneralResult );

    /**
     * Return terminal response filled according to result.
     *
     * @since S60 3.2
     * @param aGeneralResult Completion status.
     * @param aAdditionalInfo Completion status additional info.
     */
    void CompleteSetupCall(
        const RSat::TPCmdResult aGeneralResult,
        const TInt16 aAdditionalInfo );

    /**
     * Checks the dialling number string for extended BCD
     * values. Changes them to correct characters
     * see ETSI 11.11 10.5.1
     *
     * @param aNumber dialling number string
     */
    void CheckNumber( TDes& aNumber ) const;
        
    /**
     * Create emergency call
     */
    void CreateEmergencyCall( CSetupCallRequestHandler& aHandler );
    
    /**
     * Check the Param of the setup call 
     */
    TBool CheckSetupCallParam();
    
private: // data

    /**
     * SetUpCall command data from SIM.
     */
    RSat::TSetUpCallV1 iSetUpCallData;

    /**
     * SetUpCall command package.
     */
    RSat::TSetUpCallV1Pckg iSetUpCallPckg;

    /**
     * SetUpCall response data from client.
     */
    RSat::TSetUpCallRspV2 iSetUpCallRsp;

    /**
     * SetUpCall response package
     */
    RSat::TSetUpCallRspV2Pckg iSetUpCallRspPckg;

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
     * Handing the asynchronous request
     */
    CSetupCallRequestHandler* iRequestHandler;
      
    /**
     * Current call is an emergency call.
     */
    TBool iEmergencyCall;

    /**
     * Indicates if call control is active
     */
    TBool iCallControlActive;

    /**
     * wait scheduler
     */
    CActiveSchedulerWait iWait;

    /**
     * Boolean indicating is command pending for CallControl completion
     */
    TBool iPCmdPending;

    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CSETUPCALLHANDLER_H
