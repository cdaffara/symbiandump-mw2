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
* Description:  Implements MPsetBarringObserver.
*
*/


#ifndef CSATSENDSSBARRINGNOUIOBS_H
#define CSATSENDSSBARRINGNOUIOBS_H

#include <mpsetbarringobs.h>

/**
*  Implements MPsetBarringObserver and gets notifications of the barring process
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsBarringNoUiObs : public CBase,
                                 public MPsetBarringObserver
    {
public:

    /**
     * C++ default constructor.
     */
    CSatSendSsBarringNoUiObs();

    /**
     * Destructor.
     */
    ~CSatSendSsBarringNoUiObs();

    /**
     * HandleBarringModeChangedL-function handles mode changed notes.
     * @param aType defines what kind of barring program is involved.
     * @param aStatus defines the status of the barring program.
     * @param aPlural Plural notes should be used.
     */
    void HandleBarringModeChangedL( TBarringProgram aType,
        TBarringStatus aStatus, TBool aPlural );
    
    /**
     * HandleBarringModeStatusL-function handles mode status notes.
     * @param aBsc defines what kind of basic service groups are involved.
     * @param aStatus defines the status of the barring program.
     */
    void HandleBarringModeStatusL( 
        TUint8 aBsc[KPSetNumberOfBsc], TBarringStatus aStatus );
    
    /**
     * HandleBarringErrorL-function handles errors in call barring requests.
     * @param aReason is the reason for error.
     */
    void HandleBarringErrorL( TInt aReason );

    /**
     * HandleCBRequestingL-function handles requesting notes.
     * @param aOngoing defines whether or not there is a request going on.
     * @param aInterrupted if ETrue, means that notemaster should be 
     *                     immediately deleted.
     */
    void HandleCBRequestingL( TBool aOngoing, TBool aInterrupted );

    /**
     * CbPasswordChangedL-function handles notes relating to cb password 
     * change requests.
     * @param aSuccess defines whether or not the change request was successfull.
     */
    void CbPasswordChangedL( TBool aSuccess );

    /**
     * SetEngineContact-function is used to
     * @param aEngineContact is a connection to the engine. 
     */
    void SetEngineContact( MPsetCallBarring* aBarringEngine );
    };

#endif // CSATSENDSSBARRINGNOUIOBS_H
