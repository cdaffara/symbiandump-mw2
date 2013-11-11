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
#include "psetwrappertypes.h"
#include "logging.h"
#include <rsssettings.h>
#include <psetcontainer.h>
#include <psetcli.h>

/*!
  PSetCliWrapperPrivate::PSetCliWrapperPrivate
 */
PSetCliWrapperPrivate::PSetCliWrapperPrivate(
        PSetCliWrapper &owner) :
    m_Owner(owner)
{
    DPRINT;
}

/*!
  PSetCliWrapperPrivate::~PSetCliWrapperPrivate
 */
PSetCliWrapperPrivate::~PSetCliWrapperPrivate()
{
    DPRINT;
}

// --------  Observer interface  --------- // 
/*!
  PSetCliWrapperPrivate::SetEngineContact
 */
void PSetCliWrapperPrivate::SetEngineContact(MPsetCli* aEngine)
{
    DPRINT << "aEngine: " << aEngine;
}

/*!
  PSetCliWrapperPrivate::HandleCliRequestingL
 */
void PSetCliWrapperPrivate::HandleCliRequestingL(TBool aOngoing,
        TBool aInterrupted)
{
    DPRINT << "aOngoing: " << aOngoing
        << " aInterrupted: " << aInterrupted;

    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCliRequesting(static_cast<bool>(aOngoing),
                static_cast<bool> (aInterrupted));
    )
}

/*!
  PSetCliWrapperPrivate::CliInformationL
 */
void PSetCliWrapperPrivate::CliInformationL(TPsuiCli aType)
{
    DPRINT << "aType: " << aType;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.cliInformation(static_cast<PsCallLineIdentity>(aType));
    )
}

/*!
  PSetCliWrapperPrivate::HandleCliStatusL
 */
void PSetCliWrapperPrivate::HandleCliStatusL(
        TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode)
{
    DPRINT << "aBsc[0]: " << aBsc[0]
            << " aMode: " << aMode;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCliStatus(static_cast<unsigned char *>(aBsc),
            KPSetNumberOfBsc, static_cast<PsCallLineIdentity> (aMode));
    )
}

/*!
  PSetCliWrapperPrivate::HandleCnapStatusL
 */
void PSetCliWrapperPrivate::HandleCnapStatusL(TInt aStatus)
{
    DPRINT << "aStatus: " << aStatus;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCnapStatus(aStatus);
    )
}

/*!
  PSetCliWrapperPrivate::HandleCliErrorL
 */
void PSetCliWrapperPrivate::HandleCliErrorL(TInt aError)
{
    DPRINT << "aError: " << aError;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCliError(aError);
    )
}

// End of File. 
