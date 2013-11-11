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
#include <psetcallbarringwrapper.h>
#include "psetcallbarringwrapper_p.h"
#include "logging.h"

/*!
  PSetCallBarringWrapper::PSetCallBarringWrapper
 */
PSetCallBarringWrapper::PSetCallBarringWrapper( 
    CPsetContainer &psetContainer, 
    QObject *parent) 
    : 
    QObject(parent), 
    m_privateImpl(new PSetCallBarringWrapperPrivate(*this, psetContainer))
{
    DPRINT;
}

/*!
  PSetCallBarringWrapper::~PSetCallBarringWrapper
 */
PSetCallBarringWrapper::~PSetCallBarringWrapper()
{
    DPRINT;
}

/*!
  PSetCallBarringWrapper::barringStatus
 */
void PSetCallBarringWrapper::barringStatus(
    PsServiceGroup serviceGroup,
    BarringType barringType)
{
    DPRINT;
    
    m_privateImpl->barringStatus(serviceGroup, barringType);
}

/*!
  PSetCallBarringWrapper::enableBarring
 */
void PSetCallBarringWrapper::enableBarring(
    int basicServiceGroup,
    PsServiceGroup serviceGroup,
    BarringType barringType,
    QString barringPassword )
{
    DPRINT;
    
    m_privateImpl->enableBarring(
        basicServiceGroup, serviceGroup, barringType, barringPassword);
}

/*!
  PSetCallBarringWrapper::disableBarring
 */
void PSetCallBarringWrapper::disableBarring(
    int basicServiceGroup,
    PsServiceGroup serviceGroup,
    BarringType barringType,
    QString barringPassword)
{
    DPRINT;
    
    m_privateImpl->disableBarring(
        basicServiceGroup, serviceGroup, barringType, barringPassword);
}

/*!
  PSetCallBarringWrapper::changeBarringPassword
 */
void PSetCallBarringWrapper::changeBarringPassword(
    const QString &oldPassword, 
    const QString &newPassword,
    const QString &verifiedPassword)
{
    DPRINT;
    
    m_privateImpl->changeBarringPassword(
        oldPassword, newPassword, verifiedPassword);
}

// End of File. 
