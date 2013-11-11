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
  PSetCallWaitingWrapper::PSetCallWaitingWrapper
 */
PSetCallWaitingWrapper::PSetCallWaitingWrapper( 
        CPsetContainer &psetContainer, 
        QObject *parent) : 
        QObject(parent), 
        m_psetCallWaiting(NULL),
        m_privateImpl(new PSetCallWaitingWrapperPrivate(*this))
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetCallWaiting = 
        psetContainer.CreateCWObjectL(*m_privateImpl));
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallWaitingWrapper::~PSetCallWaitingWrapper
 */
PSetCallWaitingWrapper::~PSetCallWaitingWrapper()
{
    DPRINT << ": IN ";
    
    delete m_psetCallWaiting;
    
    DPRINT << ": OUT ";
}
    
// --------  Wrapper interface  --------- // 
/*!
  PSetCallWaitingWrapper::setCallWaiting
 */
void PSetCallWaitingWrapper::setCallWaiting(PsCallWaitingCommand aSetting, 
                                            int aBasicServiceGroup )
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetCallWaiting->SetCallWaitingL(
        (MPsetCallWaiting::TSetCallWaiting)aSetting, 
        (TBasicServiceGroups)aBasicServiceGroup));
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallWaitingWrapper::getCallWaitingStatus
 */
void PSetCallWaitingWrapper::getCallWaitingStatus()
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetCallWaiting->GetCallWaitingStatusL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallWaitingWrapper::cancelProcess
 */
void PSetCallWaitingWrapper::cancelProcess()
{
    DPRINT << ": IN ";
    
    m_psetCallWaiting->CancelProcess();
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallWaitingWrapper::getCPsetCallWaiting
 */
CPsetCallWaiting &  PSetCallWaitingWrapper::getCPsetCallWaiting() const
    {
    DPRINT;
    
    return *m_psetCallWaiting;
    }

// End of File. 
