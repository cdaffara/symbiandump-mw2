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
* Description:  This class implements the shutdown timer that is used by the RadioServer
*				 to shutdown the server 2 seconds after the last client has disconnected.
*
*/



// INCLUDE FILES
#include    "RadioServerShutdown.h"
#include    "RadioDebug.h"

// CONSTANTS
const TInt KServerShutdownDelay = 1000000;	// 1 secs

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioServerShutdown::CRadioServerShutdown
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioServerShutdown::CRadioServerShutdown()
	:	CTimer(EPriorityStandard)
    {
    }

// -----------------------------------------------------------------------------
// CRadioServerShutdown::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioServerShutdown::ConstructL()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerShutdown::ConstructL()"));
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CRadioServerShutdown::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioServerShutdown* CRadioServerShutdown::NewL()
    {
    CRadioServerShutdown* self = new( ELeave ) CRadioServerShutdown();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CRadioServerShutdown::~CRadioServerShutdown()
    {
	if ( IsActive() )
		{
		Cancel();
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerShutdown::Start
// -----------------------------------------------------------------------------
//
void CRadioServerShutdown::Start()
    {
	if ( !IsActive() )
		{
		After(KServerShutdownDelay);
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerShutdown::RunL
// -----------------------------------------------------------------------------
//
void CRadioServerShutdown::RunL()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerShutdown::RunL()"));
	CActiveScheduler::Stop();
    }

//  End of File

