/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Listens to call state changes and informs control. 
*
*/


// INCLUDE FILES
#include "phonehandlercallstate.h" 
#include "phonehandlercontrol.h" 
#include "phonehandlerdebug.h" 
#include <ctsydomainpskeys.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::CPhoneHandlerCallState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerCallState::CPhoneHandlerCallState( CPhoneHandlerControl& aControl )
:   CActive(CActive::EPriorityStandard),
    iControl( aControl )
    {
    CActiveScheduler::Add(this);
    } 

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::ConstructL() start" );
    
    // Allows CPhoneHandlerCallState to start to listen S60 call states.
    User::LeaveIfError( 
        iProperty.Attach( KPSUidCtsyCallInformation, KCTsyCallState ) );
        
    Subscribe();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
CPhoneHandlerCallState* CPhoneHandlerCallState::NewL( CPhoneHandlerControl& aControl )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::NewL()" );
    
    CPhoneHandlerCallState* self = new(ELeave) CPhoneHandlerCallState( aControl );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// Destructor
CPhoneHandlerCallState::~CPhoneHandlerCallState()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::~CPhoneHandlerCallState() start" );
    
    Cancel();
    iProperty.Cancel();
    iProperty.Close();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::~CPhoneHandlerCallState() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::RunL()
// Informs caller of an asynchronous request that it has been completed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::RunL()
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerCallState::RunL() - return code = %d", iStatus.Int() );
        
    TInt state( 0 );
    iProperty.Get( state );
    
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerCallState - KTelephonyCallState = %d", state );
        
    if( iStatus.Int() == KErrNone )
        {
        iControl.NotifyCallState( state );
        }
        
    Subscribe();
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::DoCancel()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::DoCancel()" );
    
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::Subscribe
// Start to listen changes in call state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::Subscribe()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::Subscribe()" );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    }

//
// End of file
