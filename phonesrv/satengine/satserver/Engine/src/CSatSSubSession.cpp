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
* Description:  Base class for sub-sessions.
*
*/



// INCLUDE FILES
#include    "CSatSSubSession.h"
#include    "CSatSServer.h"
#include    "CSatSSession.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatSSubSession::CSatSSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSSubSession::CSatSSubSession( CSatSSession& aSession ) :
    CObject(),
    iSession( aSession )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSSubSession::CSatSSubSession calling-exiting" )
    }

// Destructor
CSatSSubSession::~CSatSSubSession()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSSubSession::~CSatSSubSession calling-exiting" )
    }

//  End of File
