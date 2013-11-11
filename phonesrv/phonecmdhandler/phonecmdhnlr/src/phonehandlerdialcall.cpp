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
* Description:  Handles dial and speed dial key presses. 
*
*/


// INCLUDE FILES
#include    "phonehandlerdialcall.h" 
#include    "phonehandlercontrol.h" 
#include    "phonehandlerdebug.h" 

#include    <cphcltcommandhandler.h> 
#include    <phclttypes.h> 
#include    <RemConCallHandlingTarget.h> 

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
// CPhoneHandlerDialCall::CPhoneHandlerDialCall
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerDialCall::CPhoneHandlerDialCall( 
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ),
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDialCall::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDialCall::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDialCall::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerDialCall* CPhoneHandlerDialCall::NewL( 
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::NewL()" );
        
    CPhoneHandlerDialCall* self = new( ELeave ) CPhoneHandlerDialCall( 
                                                    aControl, 
                                                    aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CPhoneHandlerDialCall::~CPhoneHandlerDialCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::~CPhoneHandlerDialCall()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDialCall::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDialCall::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::Process()" );
    
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerDialCall::Process() telephone number is %S", &(iControl.TelephoneNumber() ) );
    iCommandHandler->Atd( GetStatus(), iControl.TelephoneNumber() ); 
            
    Activate();                         
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDialCall::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDialCall::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDialCall::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDialCall::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerDialCall::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
    switch( iState )
        {
        case EPhoneHandlerState1:
            {
            NextState();
            iControl.CommandInitiator().SendResponse( GetStatus(), 
                                                      iOperation,
                                                      aError );
            Activate();
            break;
            }
            
        case EPhoneHandlerState2:
            {
            IdleState();
            delete this; 
            break;
            }
            
        default:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDialCall::AsynchronousRequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
