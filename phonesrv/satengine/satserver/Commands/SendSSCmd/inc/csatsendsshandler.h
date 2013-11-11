/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Process forwarding procedures.
*
*/


#ifndef CSATSENDSSHANDLER_H
#define CSATSENDSSHANDLER_H

#include <cphonegsmsshandler.h>
#include <nwdefs.h>
#include <psetcallwaiting.h>
#include <psetcontainer.h>
#include <psetcallbarring.h>
#include <psetcalldiverting.h>
#include <psetcli.h>
#include <mpsetrequestobs.h>

class CSatSendSsBarringNoUiObs;
class CSatSendSsCliNoUiObs;
class CSatSendSsCallWaitingNoUiObs;
class CSatSendSsDivertNoUiObs;

/**
*  Handles process forwarding procedures.
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsHandler : public CPhoneGsmSsHandler,
                          public MPsetRequestObserver
    {

public:

    /**
     * Destructor.
     */
    ~CSatSendSsHandler();

    /**
     * From CPhoneGsmSsHandler, Process forwarding procedures.
     *
     * If a parameter is not allowed for a procedure,
     * then it is empty. If a parameter is not explicitly given,
     * then it is empty. If a parameter is present, then it can
     * be asumed to contain appropriate values.
     *
     * @param aAction It is action.
     * @param aForwarding It contains forwarding procedure.
     * @param aNumber It is directory number.
     * @param aBasicCode It is basic service code.
     * @param aTimer It is timer.
     */
    void ProcessForwardingL(
        TAction aAction,
        TForwardingProcedure aForwarding,
        const TDesC& aNumber,
        const TDesC& aBasicCode,
        const TDesC& aTimer );

    /**
     * From CPhoneGsmSsHandler, Process barring procedures.
     *
     * See comments for forwarding, they apply also in here.
     *
     * @param aAction It is action.
     * @param aBarring It contains barring procedure.
     * @param aPassword It should be barring password, if required.
     * @param aBasicCode It is basic service code.
     */
    void ProcessBarringL(
        TAction aAction,
        TBarringProcedure aBarring,
        const TDesC& aPassword,
        const TDesC& aBasicCode );

    /**
     * From CPhoneGsmSsHandler, Process password change
     *
     * @param aPassword It is password procedure.
     * @param aOldPassword It should be old password.
     * @param aNewPassword1 It is new password.
     * @param aNewPassword2 It is new password.
     */
    void ProcessPasswordChangeL(
        TPasswordProcedure aPassword,
        const TDesC& aOldPassword,
        const TDesC& aNewPassword1,
        const TDesC& aNewPassword2 );

    /**
     * From CPhoneGsmSsHandler, Process waiting.
     *
     * @param aAction It is action.
     * @param aBasicCode It is basic service code.
     */
    void ProcessWaitingL(
        TAction aAction,
        const TDesC& aBasicCode );

    /**
     * From CPhoneGsmSsHandler, Process calling line
     * identification presentation status.
     */
    void ProcessClipStatusL();

    /**
     * From CPhoneGsmSsHandler, Process calling line
     * idnetification restriction status.
     */
    void ProcessClirStatusL();

    /**
     * From CPhoneGsmSsHandler, Process connected line
     * presentation status.
     */
    void ProcessColpStatusL();

    /**
     * From CPhoneGsmSsHandler, Process connected line
     * restriction status.
     */
    void ProcessColrStatusL();

    /**
     * From CPhoneGsmSsHandler, Processes result
     */
    void ProcessCnapStatusL();

    /**
     * From MPsetRequestObs, called when request is completed
     * so that also ui browsing is ended
     */
    void RequestComplete();

    /**
     * From MPsetRequestObs, called when request status is changed
     * so that also ui browsing is ended
     */
    void RequestStatusChanged( TInt aNewStatus );

    /**
     * cancel function cancels current request
     */
    void Cancel();

private:

    /**
     * Changes parameter to basic service code
     */
    TBasicServiceGroups ChangeToBsc( const TDesC& aBasicCode ) const;

    /**
     * Connects class to the phonesettings
     */
    void ConnectToSsEngineL();

private:    // Data

    /**
     * Pointer to PhoneSettings
     */
    CPsetContainer* iSettings;

    /**
     * Call waiting handler in Phone Settings
     */
    CPsetCallWaiting* iWaiting;

    /**
     * Call waiting observer
     */
    CSatSendSsCallWaitingNoUiObs* iSatCwObserver;

    /**
     * Call barring handler in Phone Settings
     */
    CPsetCallBarring* iBarring;

    /**
     * Call barring observer
     */
    CSatSendSsBarringNoUiObs* iSatCbObserver;

    /**
     * Cli handler in Phone Settings
     */
    CPsetCli* iCli;

    /**
     * Cli observer
     */
    CSatSendSsCliNoUiObs* iSatCliObserver;

    /**
     * Call diverting handler in Phone Settings
     */
    CPsetCallDiverting* iDivert;

    /**
     * Call diverting observer
     */
    CSatSendSsDivertNoUiObs* iSatCfObserver;

    };

#endif      // CSATSENDSSHANDLER_H
