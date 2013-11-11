/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class represents the base class for all server-side subsession
*                classes.
*
*/


// INCLUDE FILES

#include "CbsCommon.h"
#include "CCbsSession.h"
#include "CCbsObject.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsObject::CCbsObject
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsObject::CCbsObject( CCbsSession& aSession )
    : iSession( aSession )
    {
    }

// Destructor    
CCbsObject::~CCbsObject()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsObject::~CCbsObject()");
    CBSLOGSTRING("CBSSERVER: <<< CCbsObject::~CCbsObject()");
    }

// -----------------------------------------------------------------------------
// CCbsObject::Message
// Returns the current message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
const RMessage2& CCbsObject::Message()
    {
    return iSession.Message();
    }

// -----------------------------------------------------------------------------
// CCbsObject::Session
// Returns the current session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CCbsSession& CCbsObject::Session()
    {
    return iSession;
    }

// -----------------------------------------------------------------------------
// CCbsObject::PanicClient
// Panics the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsObject::PanicClient( 
    TCbsSessionPanic aPanic ) const
    {
    iSession.PanicClient( aPanic );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
