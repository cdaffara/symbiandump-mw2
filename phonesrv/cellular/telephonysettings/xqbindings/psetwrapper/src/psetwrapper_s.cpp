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


#include "psetwrapper.h"
#include "psetcliwrapper.h"
#include "psetcallwaitingwrapper.h"
#include "psetcalldivertingwrapper.h"
#include "psetnetworkwrapper.h"
#include "psetcallbarringwrapper.h"
#include "logging.h"


PSetWrapper::PSetWrapper(QObject *parent):
        QObject(parent),
        m_wrapperCli(NULL),
        m_callWaitingWrapper(NULL),
        m_callDivertingWrapper(NULL),
        m_networkWrapper(NULL)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetWrapper::~PSetWrapper()
{
    DPRINT << "DUMMY WRAPPER";

    delete m_wrapperCli;
    delete m_callWaitingWrapper;
    delete m_callDivertingWrapper;
    delete m_networkWrapper;

}

PSetCliWrapper& PSetWrapper::cliWrapper()
{
    DPRINT << "DUMMY WRAPPER";
    
    if( !m_wrapperCli ){
        m_wrapperCli = new PSetCliWrapper(*m_psetContainer, this);
    }

    return *m_wrapperCli;
}


PSetCallWaitingWrapper& PSetWrapper::callWaitingWrapper()
{
    DPRINT << "DUMMY WRAPPER";
        
    if( !m_callWaitingWrapper ){
        m_callWaitingWrapper = new PSetCallWaitingWrapper(*m_psetContainer, this);
    }

    return *m_callWaitingWrapper;
}


PSetCallDivertingWrapper& PSetWrapper::callDivertingWrapper()
{
    DPRINT << "DUMMY WRAPPER";
    
    if( !m_callWaitingWrapper ){
        m_callDivertingWrapper = new PSetCallDivertingWrapper(*m_psetContainer, this);
    }

    return *m_callDivertingWrapper;
}

PSetNetworkWrapper& PSetWrapper::networkWrapper()
{
    DPRINT << "DUMMY WRAPPER";
    
    if (!m_networkWrapper) {
        m_networkWrapper = new PSetNetworkWrapper(*m_psetContainer, this);
    }

    return *m_networkWrapper;
}

class PSetCliWrapperPrivate {
    PSetCliWrapperPrivate(){}
};

PSetCliWrapper::PSetCliWrapper( CPsetContainer &psetContainer,
        QObject *parent):
    QObject(parent), m_privateImpl(NULL)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetCliWrapper::~PSetCliWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}

// --------  Wrapper interface  --------- //
void PSetCliWrapper::getColpMode()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCliWrapper::getClipMode()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCliWrapper::getClirMode()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCliWrapper::getColrMode()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCliWrapper::getCnap()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCliWrapper::cancelAll()
{
    DPRINT << "DUMMY WRAPPER";
}

class PSetCallWaitingWrapperPrivate {
    PSetCallWaitingWrapperPrivate(){}
};

PSetCallWaitingWrapper::PSetCallWaitingWrapper(
        CPsetContainer &psetContainer,
        QObject *parent) : QObject(parent), m_privateImpl(NULL)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetCallWaitingWrapper::~PSetCallWaitingWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}

// --------  Wrapper interface  --------- //
void PSetCallWaitingWrapper::setCallWaiting( PsCallWaitingCommand aSetting,
                                            int aBasicServiceGroup )
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCallWaitingWrapper::getCallWaitingStatus()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCallWaitingWrapper::cancelProcess()
{
    DPRINT << "DUMMY WRAPPER";
}

PSetCallDivertingWrapper::PSetCallDivertingWrapper(
        CPsetContainer &psetContainer, QObject *parent) :
    QObject(parent), m_DefaultNumberListCDes(NULL)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetCallDivertingWrapper::~PSetCallDivertingWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}

// --------  Wrapper interface  --------- //

int PSetCallDivertingWrapper::setCallDiverting(
        PSCallDivertingCommand& aSetting, int aBasicServiceGroup)
{
    DPRINT << "DUMMY WRAPPER";
    return 0;
}

void PSetCallDivertingWrapper::getCallDivertingStatus(
        const PsServiceGroup aServiceGroup, const PsCallDivertingCondition aCondition,
        int aBsc)
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCallDivertingWrapper::cancelProcess()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetCallDivertingWrapper::getDefaultNumbers(QStringList &aDefNumbers)
{
    DPRINT << "DUMMY WRAPPER";
    aDefNumbers = m_DefaultnumberListQSList;
}

void PSetCallDivertingWrapper::setNewDefaultNumber(QString aNumber)
{
    DPRINT << "DUMMY WRAPPER";
    m_DefaultnumberListQSList.append(aNumber);
}

void PSetCallDivertingWrapper::getVoiceMailBoxNumber(QString &/*aVmbxNumber*/)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetNetworkWrapper::PSetNetworkWrapper(
    CPsetContainer &psetContainer, QObject *parent)
    : QObject(parent)
{
    DPRINT << "DUMMY WRAPPER";
}

PSetNetworkWrapper::~PSetNetworkWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetNetworkWrapper::getNetworkAccessMode() const
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetNetworkWrapper::setNetworkAccessMode(
    PSetNetworkWrapper::NetworkAccessMode mode)
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetNetworkWrapper::getAvailableNetworks()
{
    DPRINT << "DUMMY WRAPPER";
}

PSetNetworkWrapper::ErrorCode PSetNetworkWrapper::getNetworkSelectionMode(
    PSetNetworkWrapper::NetworkSelectionMode& mode) const
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetNetworkWrapper::selectNetwork(
    const PSetNetworkWrapper::NetworkInfo& info)
{
    DPRINT << "DUMMY WRAPPER";
}

void PSetNetworkWrapper::cancelRequest()
{
    DPRINT << "DUMMY WRAPPER";
}

class PSetCallBarringWrapperPrivate {
public:
    PSetCallBarringWrapperPrivate() {}
    ~PSetCallBarringWrapperPrivate() {}
};

PSetCallBarringWrapper::PSetCallBarringWrapper(
    CPsetContainer &psetContainer,
    QObject *parent)
    :
    QObject(parent)
{
    DPRINT << "DUMMY PSetCallBarringWrapper";
}


PSetCallBarringWrapper::~PSetCallBarringWrapper()
{
    DPRINT << "DUMMY PSetCallBarringWrapper";
}


// End of File.
