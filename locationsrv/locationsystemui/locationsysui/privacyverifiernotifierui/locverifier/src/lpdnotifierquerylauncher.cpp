/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class which handles the dialog and expiration timer.
*
*/


// INCLUDE FILES
#include "lpdnotifierquerylauncher.h"
#include "lpdglobalplugindialog.h"

#include "lpdresulthandler.h"
#include <lbs/epos_privacy.h>

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::CLpdNotifierQueryLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdNotifierQueryLauncher::CLpdNotifierQueryLauncher( 
    MLpdResultHandler& aHandler ) 
    : CLpdQueryLauncherBase( aHandler )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::ConstructL()
    {
    CLpdQueryLauncherBase::ConstructL();
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdNotifierQueryLauncher* CLpdNotifierQueryLauncher::NewL( 
    MLpdResultHandler& aHandler )
    {
    CLpdNotifierQueryLauncher* self = new( ELeave )
             CLpdNotifierQueryLauncher( aHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CLpdNotifierQueryLauncher::~CLpdNotifierQueryLauncher()
    {
    // Intentionally empty
    }


// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::ExtendedHandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::ExtendedHandleDlgDismissedL()
    {
    iHandler.HandleNotificationResultL( iResult );
    }


// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::RequestType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::RequestType(TRequestType& aRequestType)
    {
    aRequestType = CLpdQueryLauncherBase::ENotification;
    }
//  End of File
