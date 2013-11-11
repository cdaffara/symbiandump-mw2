/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Responsible for handling notes related to CLIR, CLIP, 
                 COLR and COLP.
*
*/


#ifndef CSATSENDSSCLINOUIOBS_H
#define CSATSENDSSCLINOUIOBS_H

#include <mpsetcliobserver.h>

/**
*  Implements MPsetCliObserver and is responsible for handling notes related 
*  to CLIR, CLIP, COLR and COLP.
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsCliNoUiObs : public CBase, public MPsetCliObserver
    {
public:

    /**
     * C++ default constructor.
     */
    CSatSendSsCliNoUiObs();

    /**
     * Destructor.
     */
    ~CSatSendSsCliNoUiObs();

    /**
     * CliInformation-method takes the correct type of cli
     * and passes it on to iNote
     * @param aType defines the type and status of cli info.
     */
    void CliInformationL( TPsuiCli aType );

    /**
     * HandleCliRequestingL-function handles requesting notes.
     * @param aOngoing defines whether or not there is a request going on.
     * @param aInterrupted if ETrue, means that notemaster should be
     *                     immediately deleted.
     */
    void HandleCliRequestingL( TBool aOngoing, TBool aInterrupted );

    /**
     * SetEngineContact-function...
     * @param aEngine reference to the PS engine
     */
    void SetEngineContact( MPsetCli* aEngine );

    /*
     * HandleCliStatusL-function shows the status of Cli if requested.
     *   @param  aBsc list of basic service groups
     *   @param  aMode status of CLI
     */
    void HandleCliStatusL( TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode );

    /*
     * HandleCnapStatusL-function shows the status of Cnap if requested.
     *   @param  aStatus status of Cnap
     */
    void HandleCnapStatusL( TInt aStatus );

    /**
     * Handles errors.
     * @param aError error code.
     */
    void HandleCliErrorL( TInt aError );
    };

#endif // CSATSENDSSCLINOUIOBS_H
