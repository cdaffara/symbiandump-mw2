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
* Description:  Implements the MPsetDivertObserver observer.
*
*/


#ifndef CSATSENDSSDIVERTNOUIOBS_H
#define CSATSENDSSDIVERTNOUIOBS_H

#include <mpsetdivertobs.h>

/**
*  Implements MPsetDivertObserver and gets notifications of
*  the diverting process
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsDivertNoUiObs : public CBase,
                                public MPsetDivertObserver
    {
public:

    /**
     * C++ default constructor.
     */
    CSatSendSsDivertNoUiObs();

    /**
     * Destructor.
     */
    ~CSatSendSsDivertNoUiObs();

    /**
     * From MPsetDivertObserver, handles diverting changed.
     * @param aSetting identifies the cf setting.
     * @param aPlural indicates that operation affected several bscs.
     */
    void HandleDivertingChangedL(
        const TCallDivertSetting& aSetting,
        TBool aPlural );

    /**
     * From MPsetDivertObserver, handles diverting status.
     * @param aBsc array of Bscs
     * @param aSetting identifies the cf setting.
     * @param aPlural indicates that operation affects several bscs.
     */
    void HandleDivertingStatusL(
        CMobilePhoneCFList& aSetting,
        TBool aPlural );

    /**
     * From MPsetDivertObserver, handles diverting error.
     * @param aReason identifies the reason for error.
     */
    void HandleDivertingErrorL( TInt aReason );

    /**
     * HandleCFRequestingL-function handles requesting notes.
     * @param aOngoing defines whether or not there is a request going on.
     * @param aInterrupted if ETrue, means that notemaster should be
     *                     immediately deleted.
     */
    void HandleCFRequestingL( TBool aOngoing, TBool aInterrupted );

    /**
     * SetEngineContact-function sets engine contact for observer
     * @param aDivertEngine is the contact.
     */
    void SetEngineContact( MPsetCallDiverting* aDivertEngine );
    };

#endif //CSATSENDSSDIVERTNOUIOBS_H
