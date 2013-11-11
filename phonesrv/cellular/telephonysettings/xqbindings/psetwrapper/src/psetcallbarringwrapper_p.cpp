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

#include <psetcontainer.h>
#include <psetcallbarring.h>
#include "psetcallbarringwrapper.h"
#include "psetcallbarringwrapper_p.h"
#include "logging.h"

TServiceGroup convertServiceGroup(PsServiceGroup serviceGroup)
{
    TServiceGroup convertedType = EServiceGroupVoice;
    switch (serviceGroup) {
        case ServiceGroupVoice:
            convertedType = EServiceGroupVoice;
            break;
        case ServiceGroupData:
            convertedType = EServiceGroupData;
            break;
        case ServiceGroupFax:
            convertedType = EServiceGroupFax;
            break;
        case ServiceGroupAllTeleservices:
            convertedType = EServiceGroupAllTeleservices;
            break;
        default:
            break;
    }
    
    return convertedType;
}

PSetCallBarringWrapper::BarringType convertBarringType(
    TBarringProgram barringType)
{
    PSetCallBarringWrapper::BarringType convertedType = 
        PSetCallBarringWrapper::BarringTypeAllBarrings;
    switch (barringType) {
        case EBarringTypeAllBarrings:
            convertedType = PSetCallBarringWrapper::BarringTypeAllBarrings;
            break;
        case EBarringTypeAllOutgoing:
            convertedType = PSetCallBarringWrapper::BarringTypeAllOutgoing;
            break;
        case EBarringTypeOutgoingInternational:
            convertedType = PSetCallBarringWrapper::BarringTypeOutgoingInternational;
            break;
        case EBarringTypeOutgoingInternationalExceptToHomeCountry:
            convertedType = PSetCallBarringWrapper::
                BarringTypeOutgoingInternationalExceptToHomeCountry;
            break;
        case EBarringTypeAllIncoming:
            convertedType = PSetCallBarringWrapper::BarringTypeAllIncoming;
            break;
        case EBarringTypeIncomingWhenRoaming:
            convertedType = PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming;
            break;
        case EBarringTypeAllServices:
            convertedType = PSetCallBarringWrapper::BarringTypeAllServices;
            break;
        case EBarringTypeAllOutgoingServices:
            convertedType = PSetCallBarringWrapper::BarringTypeAllOutgoingServices;
            break;
        case EBarringTypeAllIncomingServices:
            convertedType = PSetCallBarringWrapper::BarringTypeAllIncomingServices;
            break;
        default:
            break;
    }
    
    return convertedType;
}

TBarringProgram convertBarringType(
    PSetCallBarringWrapper::BarringType barringType)
{
    TBarringProgram convertedType = EBarringTypeAllBarrings;
    switch (barringType) {
        case PSetCallBarringWrapper::BarringTypeAllBarrings:
            convertedType = EBarringTypeAllBarrings;
            break;
        case PSetCallBarringWrapper::BarringTypeAllOutgoing:
            convertedType = EBarringTypeAllOutgoing;
            break;
        case PSetCallBarringWrapper::BarringTypeOutgoingInternational:
            convertedType = EBarringTypeOutgoingInternational;
            break;
        case PSetCallBarringWrapper::
                BarringTypeOutgoingInternationalExceptToHomeCountry:
            convertedType 
                = EBarringTypeOutgoingInternationalExceptToHomeCountry;
            break;
        case PSetCallBarringWrapper::BarringTypeAllIncoming:
            convertedType = EBarringTypeAllIncoming;
            break;
        case PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming:
            convertedType = EBarringTypeIncomingWhenRoaming;
            break;
        case PSetCallBarringWrapper::BarringTypeAllServices:
            convertedType = EBarringTypeAllServices;
            break;
        case PSetCallBarringWrapper::BarringTypeAllOutgoingServices:
            convertedType = EBarringTypeAllOutgoingServices;
            break;
        case PSetCallBarringWrapper::BarringTypeAllIncomingServices:
            convertedType = EBarringTypeAllIncomingServices;
            break;
        default:
            break;
    }
    
    return convertedType;
}


PSetCallBarringWrapper::BarringStatus convertBarringStatus(
    TBarringStatus status)
{
    PSetCallBarringWrapper::BarringStatus convertedType = 
        PSetCallBarringWrapper::BarringStatusUnknown;
    switch (status) {
        case EBarringStatusActive:
            convertedType = PSetCallBarringWrapper::BarringStatusActive;
            break;
        case EBarringStatusInactive:
            convertedType = PSetCallBarringWrapper::BarringStatusInactive;
            break;
        case EBarringStatusNotProvisioned:
            convertedType = 
                PSetCallBarringWrapper::BarringStatusNotProvisioned;
            break;
        case EBarringStatusUnavailable:
            convertedType = PSetCallBarringWrapper::BarringStatusUnavailable;
            break;
        case EBarringStatusUnknown:
        default:
            convertedType = PSetCallBarringWrapper::BarringStatusUnknown;
            break;
    }
    
    return convertedType;
}


/*!
  PSetCallBarringWrapperPrivate::PSetCallBarringWrapperPrivate
 */
PSetCallBarringWrapperPrivate::PSetCallBarringWrapperPrivate( 
        PSetCallBarringWrapper &owner, CPsetContainer &psetContainer) 
    : 
    m_owner(owner),
    m_callBarring(NULL),
    m_currentRequest(RequestNone),
    m_barringError(PSetCallBarringWrapper::BarringErrorNone),
    m_barringStatus(PSetCallBarringWrapper::BarringStatusUnknown),
    m_barringType(PSetCallBarringWrapper::BarringTypeAllBarrings),
    m_plural(false)
{
    DPRINT;
    
    QT_TRAP_THROWING(
        m_callBarring = psetContainer.CreateCBObjectL(*this)
    )
    
    m_callBarring->SetRequestObserver(this);
}


/*!
  PSetCallBarringWrapperPrivate::~PSetCallBarringWrapperPrivate
 */
PSetCallBarringWrapperPrivate::~PSetCallBarringWrapperPrivate()
{
    DPRINT;
    
    delete m_callBarring;
}


/*!
  PSetCallBarringWrapperPrivate::barringStatus
 */
void PSetCallBarringWrapperPrivate::barringStatus(
    PsServiceGroup serviceGroup,
    PSetCallBarringWrapper::BarringType barringType)
{
    DPRINT;
    
    TServiceGroup convertedServiceGroup = convertServiceGroup(serviceGroup);
    TBarringProgram convertedBarringType = convertBarringType(barringType);

    m_currentRequest = RequestBarringStatus;
    QT_TRAP_THROWING(m_callBarring->GetBarringStatusL(
        convertedServiceGroup, convertedBarringType)
    )
}


/*!
  PSetCallBarringWrapperPrivate::enableBarring
 */
void PSetCallBarringWrapperPrivate::enableBarring(
        int basicServiceGroup,
        PsServiceGroup serviceGroup,
        PSetCallBarringWrapper::BarringType barringType,
        QString barringPassword)
{
    DPRINT;
    
    TCallBarringSetting setting;
    setting.iType = convertBarringType(barringType);
    setting.iSetting = EActivateBarring;
    setting.iServiceGroup = convertServiceGroup(serviceGroup);
    setting.iPassword.Copy(barringPassword.utf16());
    
    m_currentRequest = RequestEnableBarring;
    QT_TRAP_THROWING(m_callBarring->SetBarringL(
        setting, static_cast<TBasicServiceGroups>(basicServiceGroup));
    )
}


/*!
  PSetCallBarringWrapperPrivate::disableBarring
 */
void PSetCallBarringWrapperPrivate::disableBarring(
    int basicServiceGroup,
    PsServiceGroup serviceGroup,
    PSetCallBarringWrapper::BarringType barringType,
    QString barringPassword)
{
    DPRINT;
    
    TCallBarringSetting setting;
    setting.iType = convertBarringType(barringType);
    setting.iSetting = ECancelBarring;
    setting.iServiceGroup = convertServiceGroup(serviceGroup);
    setting.iPassword.Copy(barringPassword.utf16());
    
    m_currentRequest = RequestDisableBarring;
    QT_TRAP_THROWING(m_callBarring->SetBarringL(
        setting, static_cast<TBasicServiceGroups>(basicServiceGroup));
    )
}


/*!
  PSetCallBarringWrapperPrivate::changeBarringPassword
 */
void PSetCallBarringWrapperPrivate::changeBarringPassword(
    const QString &oldPassword,
    const QString &newPassword,
    const QString &verifiedPassword)
{
    RMobilePhone::TMobilePhonePasswordChangeV2 passwordChange;
    passwordChange.iOldPassword.Copy(oldPassword.utf16());
    passwordChange.iNewPassword.Copy(newPassword.utf16());
    passwordChange.iVerifiedPassword.Copy(verifiedPassword.utf16());
    
    m_currentRequest = RequestChangePassword;
    QT_TRAP_THROWING(
        m_callBarring->ChangePasswordL(passwordChange);
    )
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::HandleBarringModeChangedL
 */
void PSetCallBarringWrapperPrivate::HandleBarringModeChangedL( 
    TBarringProgram aType, 
    TBarringStatus aStatus, 
    TBool aPlural )
{
    DPRINT;
    Q_ASSERT(RequestEnableBarring == m_currentRequest || 
             RequestDisableBarring == m_currentRequest);
    
    m_barringType = convertBarringType(aType);
    m_barringError = PSetCallBarringWrapper::BarringErrorNone;
    m_barringStatus = convertBarringStatus(aStatus);
    m_plural = static_cast<bool>(aPlural);
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::HandleBarringModeStatusL
 */
void PSetCallBarringWrapperPrivate::HandleBarringModeStatusL( 
    TUint8 aBsc[KPSetNumberOfBsc], 
    TBarringStatus aStatus )
{
    DPRINT;
    Q_ASSERT(RequestBarringStatus == m_currentRequest);
    
    m_barringError = PSetCallBarringWrapper::BarringErrorNone;
    QT_TRYCATCH_LEAVING(
        int index = 0;
        unsigned char groupIdCandidate = aBsc[index];
        while ((groupIdCandidate 
                != static_cast<unsigned char>(KPSetUnusedValue))) {
            m_basicServiceGroupIds << groupIdCandidate;
            groupIdCandidate = aBsc[++index];
        }
    )
    m_barringStatus = convertBarringStatus(aStatus);
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::HandleBarringErrorL
 */
void PSetCallBarringWrapperPrivate::HandleBarringErrorL( 
    TInt aReason )
{
    DPRINT;
    Q_UNUSED(aReason)
    
    m_barringError = aReason;
    m_basicServiceGroupIds.clear();
    m_barringStatus = PSetCallBarringWrapper::BarringStatusUnknown;
    m_plural = false;
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::HandleCBRequestingL
 */
void PSetCallBarringWrapperPrivate::HandleCBRequestingL( 
    TBool aTrue, 
    TBool aInterrupted )
{
    DPRINT;
    
    Q_UNUSED(aTrue)
    Q_UNUSED(aInterrupted)
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::SetEngineContact
 */
void PSetCallBarringWrapperPrivate::SetEngineContact( 
    MPsetCallBarring* aBarringEngine )
{
    DPRINT;
    
    Q_UNUSED(aBarringEngine)
}


/*!
  From MPsetBarringObserver.
  PSetCallBarringWrapperPrivate::CbPasswordChangedL
 */
void PSetCallBarringWrapperPrivate::CbPasswordChangedL( 
    TBool aSuccess )
{
    DPRINT;
    
    if (aSuccess) {
        m_barringError = PSetCallBarringWrapper::BarringErrorNone;
    } else {
        m_barringError = KErrGsmSSNegativePasswordCheck;
    }
}


/*!
  From MPsetRequestObserver.
  PSetCallBarringWrapperPrivate::RequestComplete
 */
void PSetCallBarringWrapperPrivate::RequestComplete()
{
    DPRINT;
    Q_ASSERT(RequestNone != m_currentRequest);
    BarringRequest completedRequest = m_currentRequest;
    m_currentRequest = RequestNone;
    
    switch (completedRequest)
    {
        case RequestBarringStatus:
        {
            int errorCode = 0;
            QT_TRYCATCH_ERROR( errorCode,
                emit m_owner.barringStatusRequestCompleted(
                    m_barringError,
                    m_basicServiceGroupIds, 
                    m_barringStatus);
            )
            DPRINT << "RequestBarringStatus ERROR:" << errorCode;
            break;
        }
        
        case RequestEnableBarring:
        {
            int errorCode = 0;
            QT_TRYCATCH_ERROR( errorCode,
                emit m_owner.enableBarringRequestCompleted(
                    m_barringError,
                    m_barringType,
                    m_barringStatus, 
                    m_plural);
            )
            DPRINT << "RequestEnableBarring ERROR:" << errorCode;
            break;
        }
        
        case RequestDisableBarring:
        {
            int errorCode = 0;
            QT_TRYCATCH_ERROR( errorCode,
                emit m_owner.disableBarringRequestCompleted(
                    m_barringError,
                    m_barringType,
                    m_barringStatus, 
                    m_plural);
            )
            DPRINT << "RequestDisableBarring ERROR:" << errorCode;
            break;
        }
        
        case RequestChangePassword:
        {
            int errorCode = 0;
            QT_TRYCATCH_ERROR( errorCode,
                emit m_owner.barringPasswordChangeRequestCompleted(
                    m_barringError);
            )
            DPRINT << "RequestChangePassword ERROR:" << errorCode;
            break;
        }
        
        default:
            break;
    }
}


/*!
  From MPsetRequestObserver.
  PSetCallBarringWrapperPrivate::RequestStatusChanged
 */
void PSetCallBarringWrapperPrivate::RequestStatusChanged( 
    TInt aNewStatus)
{
    DPRINT;
    Q_UNUSED(aNewStatus)
    Q_ASSERT(RequestNone != m_currentRequest);
}

// End of File. 
