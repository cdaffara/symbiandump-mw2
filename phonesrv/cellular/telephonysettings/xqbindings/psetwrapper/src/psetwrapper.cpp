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
#include "psetwrapper.h"
#include "psetcliwrapper.h"
#include "psetcallwaitingwrapper.h"
#include "psetcalldivertingwrapper.h"
#include "psetnetworkwrapper.h"
#include "psetcallbarringwrapper.h"
#include "logging.h"

/*!
  PSetWrapper::PSetWrapper
 */
PSetWrapper::PSetWrapper(QObject *parent) : 
    QObject(parent),
        m_psetContainer(NULL),
        m_wrapperCli(NULL),
        m_callWaitingWrapper(NULL),
        m_callDivertingWrapper(NULL),
        m_networkWrapper(NULL),
        m_callBarringWrapper(NULL)
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetContainer = CPsetContainer::NewL()); 

    DPRINT << ": OUT ";
}

/*!
  PSetWrapper::~PSetWrapper
 */
PSetWrapper::~PSetWrapper()
{
    DPRINT << ": IN ";
    
    delete m_wrapperCli;
    delete m_callWaitingWrapper;
    delete m_callDivertingWrapper;
    delete m_networkWrapper;
    delete m_callBarringWrapper;
    delete m_psetContainer;
    
    DPRINT << ": OUT ";
}

/*!
  PSetWrapper::cliWrapper
 */
PSetCliWrapper& PSetWrapper::cliWrapper()
{
    DPRINT;
    
    if (!m_wrapperCli) {
        m_wrapperCli = new PSetCliWrapper(*m_psetContainer, this);
        Q_CHECK_PTR(m_wrapperCli);
    }
    
    return *m_wrapperCli;
}

/*!
  PSetWrapper::callWaitingWrapper
 */
PSetCallWaitingWrapper& PSetWrapper::callWaitingWrapper()
{
    DPRINT;
    
    if (!m_callWaitingWrapper) {
        m_callWaitingWrapper = 
            new PSetCallWaitingWrapper(*m_psetContainer, this);
        Q_CHECK_PTR(m_callWaitingWrapper);
    }
    
    return *m_callWaitingWrapper;
}

/*!
  PSetWrapper::callDivertingWrapper
 */
PSetCallDivertingWrapper& PSetWrapper::callDivertingWrapper()
{
    DPRINT;
    
    if (!m_callDivertingWrapper) {
        m_callDivertingWrapper = 
            new PSetCallDivertingWrapper(*m_psetContainer, this);
        Q_CHECK_PTR(m_callDivertingWrapper);
    }
    
    return *m_callDivertingWrapper;
}

/*!
  PSetWrapper::networkWrapper
 */
PSetNetworkWrapper& PSetWrapper::networkWrapper()
{
    DPRINT;
    
    if (!m_networkWrapper) {
        m_networkWrapper = new PSetNetworkWrapper(*m_psetContainer, this);
        Q_CHECK_PTR(m_networkWrapper);
    }
    
    return *m_networkWrapper;    
}

/*!
  PSetWrapper::callBarringWrapper
 */
PSetCallBarringWrapper& PSetWrapper::callBarringWrapper()
{
    DPRINT;
    
    if (!m_callBarringWrapper) {
        m_callBarringWrapper = 
            new PSetCallBarringWrapper(*m_psetContainer, this);
    }
    
    return *m_callBarringWrapper;    
}

// End of File. 
