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
#include "lpdverifierquerylauncher.h"
#include "lpdglobalplugindialog.h"
#include "lpdresulthandler.h"

// CONSTANTS



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::CLpdVerifierQueryLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdVerifierQueryLauncher::CLpdVerifierQueryLauncher( 
    MLpdResultHandler& aHandler ) 
    : CLpdQueryLauncherBase( aHandler )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::ConstructL()
    {
    CLpdQueryLauncherBase::ConstructL();
    }

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdVerifierQueryLauncher* CLpdVerifierQueryLauncher::NewL( 
    MLpdResultHandler& aHandler )
    {
    CLpdVerifierQueryLauncher* self =
        new( ELeave ) CLpdVerifierQueryLauncher( aHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CLpdVerifierQueryLauncher::~CLpdVerifierQueryLauncher()
    {
    // Intentionally empty
    }


// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::ExtendedHandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::ExtendedHandleDlgDismissedL()
    {
    iHandler.HandleVerificationResultL( iResult );
    }

void CLpdVerifierQueryLauncher::RequestType(TRequestType& aRequestType)
    {
    aRequestType = CLpdQueryLauncherBase::EVerification;
    }
//  End of File
