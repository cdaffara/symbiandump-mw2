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

#include "psetcliwrapper_p.h"
#include "psetcliwrapper.h"
#include "logging.h"
#include <rsssettings.h>
#include <msssettingsobserver.h>
#include <mpsetcliobserver.h>
#include <psetcontainer.h>
#include <psetcli.h>

/*!
  PSetCliWrapper::PSetCliWrapper
 */
PSetCliWrapper::PSetCliWrapper( CPsetContainer &psetContainer, 
        QObject *parent): QObject(parent),
            m_privateImpl(new PSetCliWrapperPrivate(*this))
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(
        m_psetCli = psetContainer.CreateCliObjectL(*m_privateImpl));
}

/*!
  PSetCliWrapper::~PSetCliWrapper
 */
PSetCliWrapper::~PSetCliWrapper()
{
    DPRINT << ": IN ";
    
    delete m_psetCli;
    
    DPRINT << ": OUT ";
}
    
// --------  Wrapper interface  --------- // 
/*!
  PSetCliWrapper::getColpMode
 */
void PSetCliWrapper::getColpMode()
{
    DPRINT << ": IN ";
     
    QT_TRAP_THROWING(m_psetCli->GetColpModeL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCliWrapper::getClipMode
 */
void PSetCliWrapper::getClipMode()
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetCli->GetClipModeL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCliWrapper::getClirMode
 */
void PSetCliWrapper::getClirMode()
{
    DPRINT << ": IN ";
        
    QT_TRAP_THROWING(m_psetCli->GetClirModeL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCliWrapper::getColrMode
 */
void PSetCliWrapper::getColrMode()
{
    DPRINT << ": IN ";
        
    QT_TRAP_THROWING(m_psetCli->GetColrModeL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCliWrapper::getCnap
 */
void PSetCliWrapper::getCnap()
{
    DPRINT << ": IN ";
    
    QT_TRAP_THROWING(m_psetCli->GetCnapL());
    
    DPRINT << ": OUT ";
}

/*!
  PSetCliWrapper::cancelAll
 */
void PSetCliWrapper::cancelAll()
{
    DPRINT << ": IN ";
    
    m_psetCli->CancelAll();  
    
    DPRINT << ": OUT ";
}
    
// End of File. 
