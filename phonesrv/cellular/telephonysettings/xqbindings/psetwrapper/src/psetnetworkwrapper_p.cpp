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

#include <rmmcustomapi.h>
#include <psetcontainer.h>
#include <psetnetwork.h>
#include <gsmerror.h>
#include <PsetCSP.h>

#include "psetnetworkwrapper_p.h"
#include "logging.h"
#include "psetnetworkinfoconverter.h"

/*!
    \class PSetNetworkWrapperPrivate
    \brief Private implementation for network wrapper.
*/

/*!
  PSetNetworkWrapperPrivate::PSetNetworkWrapperPrivate
 */
PSetNetworkWrapperPrivate::PSetNetworkWrapperPrivate(
    PSetNetworkWrapper &owner,
    CPsetContainer &psetContainer)
    :
    m_owner(owner),
    m_psetNetwork(NULL),
    m_psetNetworkMode(NULL)
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(
        m_psetNetwork.reset(psetContainer.CreateNetworkObjectL(*this)));
    m_psetNetwork->SetNetworkModeObserver(*this);
    m_psetNetwork->SetNetSAObserver(*this);
    QT_TRAP_THROWING(
        m_psetNetworkMode.reset(psetContainer.CreateNetworkModeObjectL(*this)));
    QT_TRAP_THROWING(
        m_refreshHandler.reset(psetContainer.CreateRefreshHandlerL()));
    QT_TRAP_THROWING(m_refreshHandler->NotifyFileChangeL(
                *this,
                KCsp1Ef,
                EFileChangeNotification));
    
    QT_TRAP_THROWING(m_csp.reset(CPsetCustomerServiceProfile::NewL()));
    QT_TRAP_THROWING(m_csp->OpenCSProfileL());

    DPRINT << ": OUT ";
}

/*!
  PSetNetworkWrapperPrivate::~PSetNetworkWrapperPrivate
 */
PSetNetworkWrapperPrivate::~PSetNetworkWrapperPrivate()
{
    DPRINT << ": IN ";
    
    while (!m_networkInfoList.isEmpty()) {
        DPRINT << " delete : " << m_networkInfoList.first()->m_longName;
        delete m_networkInfoList.takeFirst();
    }
    m_networkInfoList.clear();
    
    DPRINT << ": OUT ";
}

/*!
    PSetNetworkWrapperPrivate::getAvailableNetworks.
*/
void PSetNetworkWrapperPrivate::getAvailableNetworks()
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetNetwork->GetAvailableNetworksL());
    
    DPRINT << ": OUT ";
}

/*!
    PSetNetworkWrapperPrivate::getNetworkSelectionMode
*/
PSetNetworkWrapper::ErrorCode 
    PSetNetworkWrapperPrivate::getNetworkSelectionMode(
    PSetNetworkWrapper::NetworkSelectionMode& mode) const
{
    DPRINT;
    
    MPsetNetworkSelect::TSelectMode selectMode = 
        MPsetNetworkSelect::ENetSelectModeAutomatic;
    int result = m_psetNetwork->GetNetworkSelectMode(selectMode);
    mode = NetworkInfoConverter::ConvertToQtType(selectMode);
    
    return ConvertToQtErrorCode(result);
}

/*!
    PSetNetworkWrapperPrivate::selectNetwork.
 */
void PSetNetworkWrapperPrivate::selectNetwork( 
    const PSetNetworkWrapper::NetworkInfo& info)
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetNetwork->SelectNetworkL(
        NetworkInfoConverter::ConvertToSymbianType(info)));
         
    DPRINT << ": OUT ";
}

/*!
    PSetNetworkWrapperPrivate::cancelRequest
*/
void PSetNetworkWrapperPrivate::cancelRequest()
{
    DPRINT;
    
    m_psetNetwork->CancelProcess();
}

/*!
    PSetNetworkWrapperPrivate::getNetworkAccessMode
*/
void PSetNetworkWrapperPrivate::getNetworkAccessMode() const
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(
        m_psetNetwork->GetCurrentNetworkModeSelectionL());
        
    DPRINT << ": OUT ";
}

/*!
    PSetNetworkWrapperPrivate::setNetworkAccessMode
*/
void PSetNetworkWrapperPrivate::setNetworkAccessMode(
    PSetNetworkWrapper::NetworkAccessMode mode)
{
    DPRINT << ": IN ";
    
    RMmCustomAPI::TNetworkModeCaps modeCaps = 
        RMmCustomAPI::KCapsNetworkModeDual;
    switch (mode) {
        case PSetNetworkWrapper::AccessModeDual:
            modeCaps = RMmCustomAPI::KCapsNetworkModeDual;
            break;
        case PSetNetworkWrapper::AccessModeUmts:
            modeCaps = RMmCustomAPI::KCapsNetworkModeUmts;
            break;
        case PSetNetworkWrapper::AccessModeGsm:
            modeCaps = RMmCustomAPI::KCapsNetworkModeGsm;
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
    }
    
    QT_TRAP_THROWING(m_psetNetwork->SetNetworkModeSelectionL(
        reinterpret_cast<TUint32&>(modeCaps)));
        
    DPRINT << ": OUT ";
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkInfoReceivedL( 
    const CNetworkInfoArray* aInfoArray, 
    const TInt aResult)
{
    DPRINT << ": IN ";
    
    Q_UNUSED(aResult);
    __ASSERT_DEBUG(NULL != aInfoArray, User::Leave(KErrArgument));
    
    // delete qlist
    while (!m_networkInfoList.isEmpty()) {
        DPRINT << " delete : " << m_networkInfoList.first()->m_longName;
        delete m_networkInfoList.takeFirst();
    }
    m_networkInfoList.clear();
    
    const int itemsCount = aInfoArray->Count();

    //then insert found networks
    for(int i = 0; i < itemsCount; i++)
        {
        PSetNetworkWrapper::NetworkInfo *info = 
            new (ELeave) PSetNetworkWrapper::NetworkInfo;
        
        info->m_id.m_countryCode = QString::fromUtf16(
            aInfoArray->At(i).iId.iCountryCode.Ptr(), 
            aInfoArray->At(i).iId.iCountryCode.Length());
        info->m_id.m_networkCode =  QString::fromUtf16(
            aInfoArray->At(i).iId.iNetworkCode.Ptr(), 
            aInfoArray->At(i).iId.iNetworkCode.Length());
        info->m_status = 
            static_cast<PSetNetworkWrapper::NetworkStatus>(
                aInfoArray->At(i).iStatus);
        info->m_mode = 
            static_cast<PSetNetworkWrapper::NetworkSelectionMode>(
                aInfoArray->At(i).iMode);
        info->m_access = 
            static_cast<PSetNetworkWrapper::NetworkAccessType>(
                aInfoArray->At(i).iAccess);
        info->m_shortName = 
            QString::fromUtf16(
                aInfoArray->At(i).iShortName.Ptr(), 
                aInfoArray->At(i).iShortName.Length());
        info->m_longName = 
            QString::fromUtf16(
                aInfoArray->At(i).iLongName.Ptr(), 
                aInfoArray->At(i).iLongName.Length());
        
        m_networkInfoList.append(info);
        }
    
    QT_TRYCATCH_LEAVING(
        emit m_owner.availableNetworksGot(m_networkInfoList);
    )
    
    DPRINT << ": OUT ";
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleCurrentNetworkInfoL( 
    const MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, 
    const TInt aResult)
{
    DPRINT;
    
    Q_UNUSED(aCurrentInfo);
    Q_UNUSED(aResult);
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkChangedL( 
    const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
    const MPsetNetworkSelect::TCurrentNetworkStatus aStatus, 
    const TInt aResult)
{
    DPRINT;
    
    Q_UNUSED(aCurrentInfo);
    Q_UNUSED(aStatus);
    Q_UNUSED(aResult);
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkChangedL( 
    const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
    const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus, 
    const TInt aResult)
{
    DPRINT << ": IN ";
    
    Q_UNUSED(aResult);
    
    PSetNetworkWrapper::NetworkInfo info;      
    info.m_id.m_countryCode =  QString::fromUtf16(
        aCurrentInfo.iId.iCountryCode.Ptr(), 
        aCurrentInfo.iId.iCountryCode.Length());
    info.m_id.m_networkCode =  QString::fromUtf16(
        aCurrentInfo.iId.iNetworkCode.Ptr(), 
        aCurrentInfo.iId.iNetworkCode.Length());
    info.m_status = 
        static_cast<PSetNetworkWrapper::NetworkStatus>(
            aCurrentInfo.iStatus);
    info.m_mode = 
        static_cast<PSetNetworkWrapper::NetworkSelectionMode>(
            aCurrentInfo.iMode);
    info.m_access = 
        static_cast<PSetNetworkWrapper::NetworkAccessType>(
            aCurrentInfo.iAccess);
    info.m_shortName = 
        QString::fromUtf16(
            aCurrentInfo.iShortName.Ptr(), 
            aCurrentInfo.iShortName.Length());
    info.m_longName = 
        QString::fromUtf16(
            aCurrentInfo.iLongName.Ptr(), 
            aCurrentInfo.iLongName.Length());
    
    PSetNetworkWrapper::RegistrationStatus status;
    switch (aStatus){
        case RMobilePhone::ERegisteredOnHomeNetwork:
            status = PSetNetworkWrapper::RegisteredOnHomeNetwork;
            break;
        case RMobilePhone::ERegisteredRoaming:
            status = PSetNetworkWrapper::RegisteredRoaming;
            break;
        }
   
    QT_TRYCATCH_LEAVING(
        emit m_owner.networkChanged(info, status);
    )
    
    DPRINT << ": OUT ";
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleSearchingNetworksL( 
    MPsetNetworkInfoObserver::TServiceRequest aRequest)
{
    DPRINT << ": IN ";
    
    PSetNetworkWrapper::RequestType type =
        static_cast<PSetNetworkWrapper::RequestType>(aRequest);
    QT_TRYCATCH_LEAVING(
        emit m_owner.searchingNetworks(type);
    )
    
    DPRINT << ": OUT ";
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleRequestingSelectedNetworkL( 
    TBool aOngoing)
{
    DPRINT << ": IN ";
    
    QT_TRYCATCH_LEAVING(
        emit m_owner.requestingSelectedNetwork(
            static_cast<bool>(aOngoing));
    )
    
    DPRINT << ": OUT ";
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleCallActivatedL()
{
    DPRINT;
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkErrorL( 
    const MPsetNetworkInfoObserver::TServiceRequest aRequest,
    const TInt aError)
{
    DPRINT << ": IN ";
    
    PSetNetworkWrapper::ErrorCode error;
    switch(aError) {
        case KErrGsmNetCauseCallActive:
            error = PSetNetworkWrapper::ErrCauseCallActive;
            break;
        case KErrGsm0707NoNetworkService:
            error = PSetNetworkWrapper::ErrNoNetworkService;
            break;
        case KErrGsmOfflineOpNotAllowed:
            error = PSetNetworkWrapper::ErrOfflineOpNotAllowed;
            break;
        default:
            error = PSetNetworkWrapper::ErrNoNetworkAccess;
            break;
    }

    QT_TRYCATCH_LEAVING(
        emit m_owner.networkReqestFailed(
            error, static_cast<PSetNetworkWrapper::RequestType>(aRequest));
    )
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkSystemModeEventsL( 
    const MPsetNetworkModeObserver::TServiceRequest aRequest,
    const TUint32 aNetworkModeCaps)
{
    DPRINT;
    
    switch (aRequest) {
        case MPsetNetworkModeObserver::EServiceRequestNone:
            // TODO: what to do with this kind of event
            break;
        case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
            // TODO: implement this
            break;
        case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode: {
            PSetNetworkWrapper::NetworkAccessMode mode =
                PSetNetworkWrapper::AccessModeDual;
            if (aNetworkModeCaps & RMmCustomAPI::KCapsNetworkModeDual) {
                mode = PSetNetworkWrapper::AccessModeDual;
            } else if 
                (aNetworkModeCaps & RMmCustomAPI::KCapsNetworkModeUmts) {
                    mode = PSetNetworkWrapper::AccessModeUmts;
            } else if 
                (aNetworkModeCaps & RMmCustomAPI::KCapsNetworkModeGsm) {
                    mode = PSetNetworkWrapper::AccessModeGsm;
            } else {
                Q_ASSERT(false);
            }
            QT_TRYCATCH_LEAVING(
                emit m_owner.networkAccessModeGot(static_cast<int>(mode));
            )

        }
            break;
        default:
            Q_ASSERT_X(false, "switch default", "unknown enumerator");
    }
}

/*!
     From MPsetNetworkInfoObserver.
     \see MPsetNetworkInfoObserver.
*/
void PSetNetworkWrapperPrivate::HandleNetworkErrorL(
    const MPsetNetworkModeObserver::TServiceRequest aRequest,
    const TInt aError)
{
    DPRINT;
    
    Q_UNUSED(aRequest);
    Q_UNUSED(aError);
}

/*!
     PSetNetworkWrapperPrivate::ConvertToQtErrorCode
*/
PSetNetworkWrapper::ErrorCode PSetNetworkWrapperPrivate::ConvertToQtErrorCode(
    TInt symbianErrorCode) const
{
    // TODO: do proper conversion for error codes
    if (KErrNone == symbianErrorCode) {
        return PSetNetworkWrapper::ErrNoError;
    } else {
        return PSetNetworkWrapper::ErrGeneral;
    }    
}

/*!
     PSetNetworkWrapperPrivate::isManualNetworkSelectionSupported
*/
bool PSetNetworkWrapperPrivate::isManualNetworkSelectionSupported() const
{
    DPRINT << ": IN ";
    
    TBool settingSupported(EFalse);
    TInt retVal = m_csp->IsNetworkSelectionSupported(settingSupported);
 
    if ( retVal != KErrNone )
         {
        //if a CSP error occurs, by default service is available
        settingSupported = ETrue;
        }

    DPRINT << ": OUT ";
    return settingSupported;
}

/*!
     PSetNetworkWrapperPrivate::AllowRefresh
*/
TBool PSetNetworkWrapperPrivate::AllowRefresh(
                const TSatRefreshType aType,
                const TSatElementaryFiles aFiles )
{
    DPRINT << "aType: " << aType;
    DPRINT << "aFiles: " << aFiles;
    
    return ETrue;
}

/*!
     PSetNetworkWrapperPrivate::Refresh
*/
void PSetNetworkWrapperPrivate::Refresh(
                const TSatRefreshType aType,
                const TSatElementaryFiles aFiles )
{
    DPRINT << "aType: " << aType;
    DPRINT << "aFiles: " << aFiles;
    
    if ((aType != EFileChangeNotification) ||
        ((aType == EFileChangeNotification) &&
        (aFiles == KCsp1Ef))) {
        TBool networkSelectionSupported;
        if (KErrNone == m_csp->IsNetworkSelectionSupported(networkSelectionSupported)) {
            DPRINT << "networkSelectionSupported " << networkSelectionSupported;
            emit m_owner.chageVisbilityOfManualNetworkSelection(networkSelectionSupported);
        }
    }
    
    DPRINT << ": OUT";
}

