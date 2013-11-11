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
* Description:  Call Waiting observer
*
*/


#ifndef CSATSENDSSCALLWAITINGNOUIOBS_H
#define CSATSENDSSCALLWAITINGNOUIOBS_H

#include <mpsetcallwaitingobs.h>

/**
*  Implements MPsetCallWaitingObserver and gets notifications
*  of the call waiting process.
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsCallWaitingNoUiObs : public CBase,
                                     public MPsetCallWaitingObserver
    {
public:

    /**
     * C++ default constructor.
     */
    CSatSendSsCallWaitingNoUiObs();

    /**
     * Destructor.
     */
    ~CSatSendSsCallWaitingNoUiObs();

    /**
     * HandleCallWaitingGetStatusL-function
     * @param aStatus is the query result.
     */
    void HandleCallWaitingGetStatusL(
        MPsetCallWaiting::TGetCallWaitingStatus aStatus,
        TUint8 aBsc[KPSetNumberOfBsc] );

    /**
     * HandleCallWaitingChangedL-function
     * @param aSetting is the setting that was changed.
     * @param aResult is the result of the action.
     */
    void HandleCallWaitingChangedL(
        MPsetCallWaiting::TSetCallWaiting aSetting,
        TInt aResult );

    /**
     * HandleRequestingL-function handles requesting notes.
     * @param aOngoing defines whether or not there is a request going on.
     * @param aInterrupted if ETrue deletes immediately CPsuiNoteMaster-object.
     */
    void HandleCWRequestingL( TBool aOngoing, TBool aInterrupted );

    /**
     * This function handles errors in call waiting requests.
     * @param aReason is the reason for error.
     */
    void HandleCWErrorL( TInt aReason );

    /**
     * SetEngineContact-function
     * @param aEngineContact is a connection to the engine.
     */
    void SetEngineContact( MPsetCallWaiting& aEngineContact );
    };

#endif //CSATSENDSSCALLWAITINGNOUIOBS_H
