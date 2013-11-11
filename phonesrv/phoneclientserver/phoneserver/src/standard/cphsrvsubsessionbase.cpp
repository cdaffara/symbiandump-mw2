/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub Session Base.
*
*/



// INCLUDE FILES
#include "cphsrvsubsessionbase.h" 
#include "cphsrvsession.h" 
#include "cphsrvsubsessionnotifier.h" 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::CPhSrvSubSessionBase
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase::CPhSrvSubSessionBase( 
    CPhSrvSession& aSession, 
    TPhSrvSubSessionType aType )
:   iSession(aSession), 
    iSubSessionType(aType)
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::PhoneSession
// 
// Access a non-constant phone session
// -----------------------------------------------------------------------------
//
CPhSrvSession& CPhSrvSubSessionBase::PhoneSession()
    {
    return iSession;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::PhoneSession
// 
// Access the session
// -----------------------------------------------------------------------------
//
const CPhSrvSession& CPhSrvSubSessionBase::PhoneSession() const
    {
    return iSession;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::SubSessionUniqueHandle
// 
// Return the unique handle for this subsession
// -----------------------------------------------------------------------------
//
TInt CPhSrvSubSessionBase::SubSessionUniqueHandle() const
    {
    return PhoneSession().SubSessionUniqueHandle( *this );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::Type
// 
// The type of the subsession
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase::TPhSrvSubSessionType CPhSrvSubSessionBase::Type() const
    {
    return iSubSessionType;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::Write
// 
// Write to client address space
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionBase::Write( 
    const RMessage2& aMessage,
    TInt aLocation,
    const TDesC8& aDes,
    TInt aOffset )
    {
    PhoneSession().Write( 
        aMessage,
        aLocation,
        aDes,
        aOffset );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionBase::Read
// 
// Read from client address space
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionBase::Read(
    const RMessage2& aMessage,
    TInt aLocation,
    TDes8& aDes,
    TInt aOffset )
    {
    PhoneSession().Read(
        aMessage,
        aLocation,
        aDes,
        aOffset );
    }


// End of File
