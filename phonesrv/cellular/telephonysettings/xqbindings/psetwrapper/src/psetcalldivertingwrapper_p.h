/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  
 *
 */

#ifndef DIVERTINGOBSERVER_H_
#define DIVERTINGOBSERVER_H_

#include <mpsetdivertobs.h>
#include <mphcltemergencycallobserver.h>
#include <mpsetrequestobs.h>
#include <voicemailboxdefs.h>
#include "psetcalldivertingwrapper.h"
#include "psetwrappertypes.h"

class PSetCallDivertingWrapperPrivate : 
        public MPsetDivertObserver,
        public MPhCltEmergencyCallObserver,
        public MPsetRequestObserver
{
public:
    PSetCallDivertingWrapperPrivate(PSetCallDivertingWrapper &owner);

    virtual ~PSetCallDivertingWrapperPrivate();

public:
    // Functions from base classes (observer interface):     

    /**
     * Handles showing notes, when diverts change.
     *
     * @param aSetting Setting which was changed.
     * @param aPlural Plural notes to be used.
     */
    void HandleDivertingChangedL(const TCallDivertSetting& aSetting,
            TBool aPlural);

    /**
     * Handles showing of status inquiry notes.
     *
     * @param aBsc List of basic services.
     * @param aSetting Divert setting which was queried.
     * @param aPlural Plural notes to be used.
     */

    void HandleDivertingStatusL(CMobilePhoneCFList& aSetting, TBool aPlural);

    /**
     * Handles showing of error notes.
     *
     * @param aReason Reason for diverting error.
     */
    void HandleDivertingErrorL(TInt aReason);

    /**
     * Handles requesting notes.
     *
     * @param aOngoing Is there a request going on.
     * @param aInterrupted Request needs to immediately cancelled.
     */
    void HandleCFRequestingL(TBool aOngoing, TBool aInterrupted);

    /**
     * Sets pointer so that message is passed succesfully
     * from requester to observer.
     *
     * @param aDivertEngine Source for the events to observer.
     */
    void SetEngineContact(MPsetCallDiverting* aDivertEngine);

public:
    // From MPhCltEmergencyCallObserver   

    /**
     * It is called whenever client's dial request is completed.
     *
     * @param aStatus error code of the success of the operation.
     */
    void HandleEmergencyDialL(const TInt aStatus);

public:
    // From MPsetRequestObserver
    /**
    * Informs the receiver that request has been completed.
    */
    void RequestComplete();

    /**
    * Informs the receiver that request state has changed.
    * @param new status, or when finished KErrNone or error code.
    */
    void RequestStatusChanged( TInt aNewStatus );
    
public:
    TCallDivertingCondition convert(PsCallDivertingCondition type);
    TDivertingStatus convert(PsCallDivertingStatus type);
    TServiceGroup convert(PsServiceGroup type);
    TDivertingSetting convert(PsCallDivertingSetting type);
    TVmbxType convert(PsService aService);
    
private:
    PsCallDivertingCondition convert(TCallDivertingCondition type);
    PsCallDivertingCondition convert(RMobilePhone::TMobilePhoneCFCondition type);
    PsCallDivertingStatus convert(TDivertingStatus type);
    PsCallDivertingStatus convert(RMobilePhone::TMobilePhoneCFStatus type);
    PsServiceGroup convert(TServiceGroup type);
    PsServiceGroup convert(RMobilePhone::TMobileService service);
    
    
private:
    PSetCallDivertingWrapper &m_Owner;

    QList<PSCallDivertingStatus*> m_DivertingStatusList;

    PSCallDivertingCommand m_callDivertingSetting;
};

#endif /* DIVERTINGOBSERVER_H_ */
