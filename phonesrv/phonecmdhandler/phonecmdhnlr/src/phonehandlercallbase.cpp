/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for services. 
*
*/



// INCLUDE FILES
#include "phonehandlercallbase.h" 
#include "phonehandleractive.h" 
#include "phonehandlerdebug.h" 

#include <cphcltcommandhandler.h> 

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
// CPhoneHandlerCallBase::CPhoneHandlerCallBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerCallBase::CPhoneHandlerCallBase()
    : iState( EPhoneHandlerState1 )
    {
    }

// Destructor
CPhoneHandlerCallBase::~CPhoneHandlerCallBase()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::~CPhoneHandlerCallBase()" );
            
    iServer.Close();
    delete iCommandHandler;
    delete iActive;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallBase::LoadServiceL
// Initializes API that is used to request call handling related services.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallBase::LoadServiceL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::LoadServiceL() start" );
    
    
    
    iCommandHandler = CPhCltCommandHandler::NewL();
    
    User::LeaveIfError( iServer.Connect() ); // create session
    
    // Create active object
    iActive = CPhoneHandlerActive::NewL( *this );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::LoadServiceL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallBase::NextState
// Increments state used to control service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallBase::NextState()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::NextState() start" );
        
    TInt temp = static_cast<TInt>( iState );
    ++temp;
    iState = static_cast<TPhoneHandlerServiceState>( temp );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::NextState() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerCallBase::IdleState
// Sets a specific state when service has been completed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallBase::IdleState()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::IdleState() start" );
    
    iState = EPhoneHandlerStateIdle;
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::IdleState() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallBase::GetStatus
// Returns active object's TRequestStatus for asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TRequestStatus& CPhoneHandlerCallBase::GetStatus()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::GetStatus() start" );
    
    return iActive->iStatus;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerCallBase::Activate
// Activates an active object for asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CPhoneHandlerCallBase::Activate()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::Activate() start" );
    
    iActive->SetActive();                       
        
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallBase::Activate() end" );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
