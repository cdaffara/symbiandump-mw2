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
#include <QObject>
#include <rsssettings.h>
#include <psetcontainer.h>
#include <psetcallwaiting.h>
#include <smcmockclassincludes.h>
#include "psetcallwaitingwrapper_p.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::PSetCallWaitingWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCallWaitingWrapperPrivate::PSetCallWaitingWrapperPrivate( 
        PSetCallWaitingWrapper & owner )
    : m_Owner(owner)
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::~PSetCallWaitingWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCallWaitingWrapperPrivate::~PSetCallWaitingWrapperPrivate(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::HandleCallWaitingGetStatusL
// -----------------------------------------------------------------------------
//
void PSetCallWaitingWrapperPrivate::HandleCallWaitingGetStatusL( 
        const MPsetCallWaiting::TGetCallWaitingStatus aStatus,
        TUint8 aBsc[KPSetNumberOfBsc])
    {
    Q_UNUSED(aStatus)
    Q_UNUSED(aBsc)
    // TODO: make compile
    //    SMC_MOCK_METHOD2( void, const MPsetCallWaiting::TGetCallWaitingStatus, aStatus, 
    //        TUint8[KPSetNumberOfBsc], aBsc)
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::HandleCallWaitingChangedL
// -----------------------------------------------------------------------------
//
void PSetCallWaitingWrapperPrivate::HandleCallWaitingChangedL( 
        const MPsetCallWaiting::TSetCallWaiting aSetting,
        const TInt aResult )
    {
    SMC_MOCK_METHOD2( void, const MPsetCallWaiting::TSetCallWaiting, aSetting, 
        const TInt, aResult )
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::HandleCWRequestingL
// -----------------------------------------------------------------------------
//
void PSetCallWaitingWrapperPrivate::HandleCWRequestingL( 
        TBool aOngoing,
        TBool aInterrupted )
    {
    SMC_MOCK_METHOD2( void, TBool, aOngoing, 
        TBool, aInterrupted )
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::HandleCWErrorL
// -----------------------------------------------------------------------------
//
void PSetCallWaitingWrapperPrivate::HandleCWErrorL( 
        TInt aReason )
    {
    SMC_MOCK_METHOD1( void, TInt, aReason )
    }


// -----------------------------------------------------------------------------
// PSetCallWaitingWrapperPrivate::SetEngineContact
// -----------------------------------------------------------------------------
//
void PSetCallWaitingWrapperPrivate::SetEngineContact( 
        MPsetCallWaiting & aEngineContact )
    {
    SMC_MOCK_METHOD1( void, MPsetCallWaiting &, aEngineContact )
    }


