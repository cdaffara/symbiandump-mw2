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

#ifndef PSETCALLWAITINGWRAPPER_P_H_
#define PSETCALLWAITINGWRAPPER_P_H_

#include <mpsetcallwaitingobs.h>

// Forward declarations
class PSetCallWaitingWrapper;

class PSetCallWaitingWrapperPrivate: 
    public MPsetCallWaitingObserver
{
public:
    
    PSetCallWaitingWrapperPrivate( PSetCallWaitingWrapper &owner );
    
    ~PSetCallWaitingWrapperPrivate();
        
public: // From MPsetCallWaitingObserver
    
    /**
    * Handles notes when requesting Call Waiting status.
    *
    * @param aStatus Query result.
    * @param aBsc is List of active groups.
    */
    void HandleCallWaitingGetStatusL( 
        const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
        TUint8 aBsc[KPSetNumberOfBsc] );
    
    /**
    * Handles notes when Call Waiting has been changed.
    *
    * @param aSetting Changed setting.
    * @param aResult Result of the action.
    */
    void HandleCallWaitingChangedL( 
        const MPsetCallWaiting::TSetCallWaiting aSetting, 
        const TInt aResult );

    /**
    * Handles requesting notes.
    *
    * @param aStarted Is there a request going on.
    * @param aInterrupted Request needs to be immediately cancelled.
    */
    void HandleCWRequestingL( TBool aOngoing, 
        TBool aInterrupted ); 
    /**
    * Handles errors in call waiting requests.
    *
    * @param aReason Reason for the error.
    */
    void HandleCWErrorL( TInt aReason );

    /**
    * Sets pointer so that message is passed succesfully
    * from requester to observer.
    *
    * @param aEngineContact Source of events to observer.
    */
    void SetEngineContact( MPsetCallWaiting& aEngineContact );

private: // Data: 
    PSetCallWaitingWrapper &m_Owner;

};


#endif /* PSETCALLWAITINGWRAPPER_P_H_ */
