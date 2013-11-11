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
* Description:  Scheduler.
*
*/


// INCLUDE FILES
#include "cphsrvscheduler.h" 
#include "phsrvutils.h" 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvScheduler::CPhSrvScheduler
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvScheduler::CPhSrvScheduler()
    {
    }

// -----------------------------------------------------------------------------
// CPhSrvScheduler::Error
// 
// Panic the server.
// -----------------------------------------------------------------------------
//
void CPhSrvScheduler::Error( TInt /*aError*/ ) const
    {
    PhoneServerUtils::Panic( EPhoneServerPanicSchedulerError );
    }


//  End of File
