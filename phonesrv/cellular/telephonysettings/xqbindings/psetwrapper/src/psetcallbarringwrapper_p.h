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

#ifndef PSETCALLBARRINGWRAPPER_P_H
#define PSETCALLBARRINGWRAPPER_P_H

#include <mpsetbarringobs.h>
#include <mpsetrequestobs.h>
#include <nwdefs.h>
#include <psetcallbarringwrapper.h>

// Forward declarations
class PSetCallBarringWrapper;
class CPsetContainer;
class CPsetCallBarring;

class PSetCallBarringWrapperPrivate 
    : public MPsetBarringObserver, public MPsetRequestObserver
{
public:
    
    enum BarringRequest
    {
        RequestNone,
        RequestBarringStatus,
        RequestEnableBarring,
        RequestDisableBarring,
        RequestChangePassword
    };
    
public:
    
    PSetCallBarringWrapperPrivate(
        PSetCallBarringWrapper &owner,
        CPsetContainer &psetContainer);
    
    ~PSetCallBarringWrapperPrivate();
    
    void barringStatus(
        PsServiceGroup serviceGroup, 
        PSetCallBarringWrapper::BarringType barringType);
    
    void enableBarring(
        int basicServiceGroup,
        PsServiceGroup serviceGroup,
        PSetCallBarringWrapper::BarringType barringType,
        QString barringPassword);
    
    void disableBarring(
        int basicServiceGroup,
        PsServiceGroup serviceGroup,
        PSetCallBarringWrapper::BarringType barringType,
        QString barringPassword);
    
    void changeBarringPassword(
        const QString &oldPassword, 
        const QString &newPassword,
        const QString &verifiedPassword);
    
public: // From MPsetBarringObserver
    
    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void HandleBarringModeChangedL( 
        TBarringProgram aType, 
        TBarringStatus aStatus, 
        TBool aPlural );        
    
    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void HandleBarringModeStatusL( 
        TUint8 aBsc[KPSetNumberOfBsc], 
        TBarringStatus aStatus );
    
    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void HandleBarringErrorL( 
        TInt aReason );

    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void HandleCBRequestingL( 
        TBool aTrue, 
        TBool aInterrupted ); 
    
    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void SetEngineContact( 
        MPsetCallBarring* aBarringEngine );

    /**
     * From MPsetBarringObserver.
     * @see MPsetBarringObserver.
     */
    void CbPasswordChangedL( 
        TBool aSuccess );

public: // From MPsetRequestObserver
    
    /**
     * From MPsetRequestObserver.
     * @see MPsetRequestObserver.
     */
    void RequestComplete();

    /**
     * From MPsetRequestObserver.
     * @see MPsetRequestObserver.
     */
    void RequestStatusChanged( 
        TInt aNewStatus);
    
private: 
    
    /** Owner. */
    PSetCallBarringWrapper &m_owner;
    
    /** Call barring supplementary service handler. */
    CPsetCallBarring *m_callBarring;
    
    /** Current barring request. */
    BarringRequest m_currentRequest;
    
    /** Error info from last completed request. */
    int m_barringError;
    
    /** Basic service group identifiers from last completed request. */
    QList<unsigned char> m_basicServiceGroupIds;
    
    /** Barring status from last completed request. */
    PSetCallBarringWrapper::BarringStatus m_barringStatus;
    
    /** Barring type from last completed request. */
    PSetCallBarringWrapper::BarringType m_barringType;
    
    /** Plurality from last completed request. */
    bool m_plural;
};

#endif // PSETCALLBARRINGWRAPPER_P
