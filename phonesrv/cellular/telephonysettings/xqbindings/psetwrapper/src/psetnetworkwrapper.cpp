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

#include <QtGlobal>
#include "psetnetworkwrapper.h"
#include "psetnetworkwrapper_p.h"
#include "logging.h"

/*!
    \class PSetNetworkWrapper
    \brief TODO: description
*/

/*!
    \enum PSetNetworkWrapper::ErrorCode
    
    This enumeration defines error codes used by PSetNetworkWrapper.
*/
/*!
    \var PSetNetworkWrapper::ErrNoError
         Indicates that no error has happened.
*/

// TODO: better commenting for NetworkAccessMode enumeration
/*!
    \enum NetworkAccessMode
    This enumeration defines possible network access modes.
*/
/*!
    \var PSetNetworkWrapper::AccessModeDual
         Dual mode.
*/
/*!
    \var PSetNetworkWrapper::AccessModeUmts
         3g/UMTS network mode.
*/
/*!
    \var PSetNetworkWrapper::AccessModeGsm
         2g/GSM network mode.
*/

/*! 
     \struct PSetNetworkWrapper::NetworkId
     NetworkId contains network and country identifiers. 
*/
// TODO: is that valid way to document struct members
/*!
    \var PSetNetworkWrapper::NetworkId::m_countryCode
         Country code. Maximum length is 4.
*/
/*!
    \var PSetNetworkWrapper::NetworkId::m_networkCode
         Network code. Maximum length is 8.
*/

/*!
    \enum PSetNetworkWrapper::NetworkSelectionMode
    Network selection mode.
*/
/*!
    \var PSetNetworkWrapper::SelectionModeManual
         Specific network is selected manually.
*/
/*!
    \var PSetNetworkWrapper::SelectionModeAutomatic
         Network is selected automatically without user interaction.
*/

/*!
    \enum PSetNetworkWrapper::NetworkStatus
    Status of a network.
*/
// TODO: enumerator comments
/*!
    \var PSetNetworkWrapper::StatusUnknown
*/
/*!
    \var PSetNetworkWrapper::StatusAvailable
*/
/*!
    \var PSetNetworkWrapper::StatusCurrent
*/
/*!
    \var PSetNetworkWrapper::StatusForbidden
*/

/*!
    \enum PSetNetworkWrapper::NetworkAccessType
    Network access type.
*/
// TODO: enumerator comments
/*!
    \var PSetNetworkWrapper::AccessTypeGsm
*/
/*!
    \var PSetNetworkWrapper::AccessTypeWcdma
*/

/*! 
     \struct PSetNetworkWrapper::NetworkInfo
     Encapsulates information about network.
*/
// TODO: struct member commenting

/*! void PSetNetworkWrapper::networkAccessModeGot(
        PSetNetworkWrapper::NetworkAccessMode mode)
    
    This signal is emitted when the current network access mode query has been
    completed. 
*/

/*!
  PSetNetworkWrapper::PSetNetworkWrapper
 */
PSetNetworkWrapper::PSetNetworkWrapper(
    CPsetContainer &psetContainer, QObject *parent) 
    :
    QObject(parent), 
    m_privateImpl(new PSetNetworkWrapperPrivate(*this, psetContainer))
{
    DPRINT << ": IN";
    
    DPRINT << ": OUT";
}

/*!
  PSetNetworkWrapper::~PSetNetworkWrapper
 */
PSetNetworkWrapper::~PSetNetworkWrapper()
{
    DPRINT << ": IN";
    
    delete m_privateImpl;
    
    DPRINT << ": OUT";
}

/*!
    Puts network retrieve results into array.
*/
void PSetNetworkWrapper::getAvailableNetworks()
{
    DPRINT;
    
    m_privateImpl->getAvailableNetworks();
}

/*!
    Returns current network selection mode.
    
    \param  aMode   Network selection mode.
    \return Error code.
*/
PSetNetworkWrapper::ErrorCode PSetNetworkWrapper::getNetworkSelectionMode(
    PSetNetworkWrapper::NetworkSelectionMode& mode) const
{
    DPRINT;
    
    return m_privateImpl->getNetworkSelectionMode(mode);
}

/*!
    Selects network, also for selecting automatic/manual mode.
    \param   aInfo   Network information.
*/
void PSetNetworkWrapper::selectNetwork( 
    const PSetNetworkWrapper::NetworkInfo& info)
{
    DPRINT;
    
    m_privateImpl->selectNetwork(info);
}

/*!
    Cancels ongoing request if any.
*/
void PSetNetworkWrapper::cancelRequest()
{
    DPRINT;
    
    m_privateImpl->cancelRequest();
}

/*!
    Starts to query network access mode. Result is informed with a
    networkAccessModeGot() signal.
*/
void PSetNetworkWrapper::getNetworkAccessMode() const
{
    DPRINT;
    
    m_privateImpl->getNetworkAccessMode();
}

/*!
  Sets network access mode.
  \param  mode    Network access mode. 
  \return ErrorCode.
*/
void PSetNetworkWrapper::setNetworkAccessMode(
    PSetNetworkWrapper::NetworkAccessMode mode)
{
    DPRINT;
    
    m_privateImpl->setNetworkAccessMode(mode);
}

/*!
  Is manual network selection supported.
*/
bool PSetNetworkWrapper::isManualNetworkSelectionSupported() const
{
    DPRINT;
    
    return m_privateImpl->isManualNetworkSelectionSupported();
}

// End of File. 
