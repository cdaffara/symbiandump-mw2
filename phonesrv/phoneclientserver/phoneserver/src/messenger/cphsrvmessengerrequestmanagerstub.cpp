/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Stub version of CPhSrvMessengerRequestManager.cpp
*
*/



// INCLUDES
#include "cphsrvmessengerrequestmanager.h" 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::CPhSrvMessengerRequestManager
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerRequestManager::CPhSrvMessengerRequestManager( 
    CPhSrvServer& aServer )
:   iServer( aServer )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::~CPhSrvMessengerRequestManager
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerRequestManager::~CPhSrvMessengerRequestManager()
    {
    // Stub.
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::ConstructL
// 
// Complete construction.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::ConstructL()
    {
    // Stub.
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequest
// 
// Initiate a messenger request - called by the external messenger interface.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequest( 
    CPhSrvSubSessionBase& /*aSubSession*/,
    const TPhCltMessengerParameters& /*aParameters*/,
    const RMessage2& /*aMessage*/ )
    {
    // Stub.
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestCancel
// 
// Cancel a messenger request handling.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestCancel( 
    CPhSrvSubSessionBase& /*aSubSession*/,
    const TPhCltMessengerCommand /*aRequestType*/ )
    {
    // Stub.
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestConstructL
// 
// Reserve all space needed for the subsession given as parameter.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestConstructL( 
    CPhSrvSubSessionBase& /*aSubSession*/,
    const TPhCltMessengerParameters& /*aParameters*/,
    const RMessage2& /*aMessage*/ )
    {
    // Stub.
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestDelete
// 
// Delete all space reserved for the subsession given as parameter.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestDelete( 
    CPhSrvSubSessionBase& /*aSubSession*/ )
    {
    // Stub.
    }


// End of File
