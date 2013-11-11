/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Command Handler Request Manager Stub implementation.
*
*/



// INCLUDES
#include "cphsrvcomhandrequestmanager.h" 
#include "cphsrvcomhandrequest.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvutils.h" 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::CPhSrvComHandRequestManager
// 
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequestManager::CPhSrvComHandRequestManager(
    CPhSrvServer& aServer )
:   iServer( aServer )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::~CPhSrvComHandRequestManager
// 
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequestManager::~CPhSrvComHandRequestManager()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::NegotiatorRequestL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::NegotiatorRequestL( 
    const RMessage2& /* aMessage */, 
    const TPhCltComHandCommandParameters& /* aParams */, 
    CPhSrvSubSessionBase& /* aSubSession */ )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::NegotiatorRequestCancel
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::NegotiatorRequestCancel( 
    CPhSrvSubSessionBase& /* aSubSession */ )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorReadyRequestL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorReadyRequestL(
    MPhSrvComHandInitiator& /* aInitiator */ )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorReadyRequestCancel
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorReadyRequestCancel( 
    MPhSrvComHandInitiator& /* aInitiator */ )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorAttemptedRequestResponseValue
// 
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorAttemptedRequestResponseValue( 
    TPhCltPhoneResults /* aResult */ )
    {
    }


// End of File
