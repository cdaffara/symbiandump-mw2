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

#include <rsssettings.h>
#include <psetcontainer.h>
#include <psetcallwaiting.h>
#include "psetcallwaitingwrapper.h"
#include "psetcallwaitingwrapper_p.h"
#include "logging.h"

/*!
  ConvertToQtType
 */
PSetCallWaitingWrapper::PsCallWaitingStatus ConvertToQtType(
    const MPsetCallWaiting::TGetCallWaitingStatus& status)
{
    PSetCallWaitingWrapper::PsCallWaitingStatus convertedType =
        PSetCallWaitingWrapper::StatusUnknown;
    
    switch (status) {
        case MPsetCallWaiting::EStatusActive:
            convertedType = PSetCallWaitingWrapper::StatusActive;
            break;
        case MPsetCallWaiting::EStatusDisabled:
            convertedType = PSetCallWaitingWrapper::StatusDisabled;
            break;
        case MPsetCallWaiting::EStatusNotAvailable:
            convertedType = PSetCallWaitingWrapper::StatusNotAvailable;
            break;
        case MPsetCallWaiting::EStatusNotProvisioned:
            convertedType = PSetCallWaitingWrapper::StatusNotProvisioned;
            break;
        case MPsetCallWaiting::EStatusUnknown:
            convertedType = PSetCallWaitingWrapper::StatusUnknown;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}

/*!
  ConvertToQtType
 */
PSetCallWaitingWrapper::PsCallWaitingCommand ConvertToQtType(
    const MPsetCallWaiting::TSetCallWaiting& callWaitingCommand)
{
    PSetCallWaitingWrapper::PsCallWaitingCommand convertedType =
        PSetCallWaitingWrapper::ActivateCallWaiting;
    
    switch (callWaitingCommand) {
        case MPsetCallWaiting::EActivateCallWaiting:
            convertedType = PSetCallWaitingWrapper::ActivateCallWaiting;
            break;
        case MPsetCallWaiting::EDeactivateCallWaiting:
            convertedType = PSetCallWaitingWrapper::DeactivateCallWaiting;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
            break;
    }
    
    return convertedType;
}
    
/*!
  PSetCallWaitingWrapperPrivate::PSetCallWaitingWrapperPrivate
 */
PSetCallWaitingWrapperPrivate::PSetCallWaitingWrapperPrivate( 
        PSetCallWaitingWrapper &owner ) : m_Owner(owner)
{
    DPRINT;
}

/*!
  PSetCallWaitingWrapperPrivate::~PSetCallWaitingWrapperPrivate
 */
PSetCallWaitingWrapperPrivate::~PSetCallWaitingWrapperPrivate()
{
    DPRINT;
}

// --------  Observer interface  --------- // 
/*!
  PSetCallWaitingWrapperPrivate::HandleCallWaitingGetStatusL
 */
void PSetCallWaitingWrapperPrivate::HandleCallWaitingGetStatusL( 
        const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
        TUint8 aBsc[KPSetNumberOfBsc] )
{
    DPRINT << "aStatus:" << aStatus;
    
    QT_TRYCATCH_LEAVING(
        QList<unsigned char> basicServiceGroupIds;
        int index = 0;
        unsigned char groupIdCandidate = aBsc[index];
        while (groupIdCandidate != KPSetUnusedValue) {
            basicServiceGroupIds << groupIdCandidate;
            groupIdCandidate = aBsc[++index];
        }
        
        emit m_Owner.handleCallWaitingGetStatus(
            ConvertToQtType(aStatus), basicServiceGroupIds );
    )
}

/*!
  PSetCallWaitingWrapperPrivate::HandleCallWaitingChangedL
 */
void PSetCallWaitingWrapperPrivate::HandleCallWaitingChangedL( 
        const MPsetCallWaiting::TSetCallWaiting aSetting, 
        const int aResult )
{
    DPRINT << "aSetting: " << aSetting
            << "aResult" << aResult;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCallWaitingChanged(
            ConvertToQtType(aSetting), 
            static_cast<int>(aResult) );
    )
}

/*!
  PSetCallWaitingWrapperPrivate::HandleCWRequestingL
 */
void PSetCallWaitingWrapperPrivate::HandleCWRequestingL( TBool aOngoing, 
        TBool aInterrupted )   
{
    DPRINT << "aOngoing: " << aOngoing
            << " aInterrupted: " << aInterrupted;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCallWaitingRequesting( 
            static_cast<bool>(aOngoing), 
            static_cast<bool>(aInterrupted) );
    )
}

/*!
  PSetCallWaitingWrapperPrivate::HandleCWErrorL
 */
void PSetCallWaitingWrapperPrivate::HandleCWErrorL( TInt aError )
{
    DPRINT << "aError: " << aError;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCallWaitingError( aError );
    )
}

/*!
  PSetCallWaitingWrapperPrivate::SetEngineContact
 */
void PSetCallWaitingWrapperPrivate::SetEngineContact(
        MPsetCallWaiting& aEngineContact )
{   
    DPRINT << "aEngineContact: " << &aEngineContact;
}

// End of File. 
