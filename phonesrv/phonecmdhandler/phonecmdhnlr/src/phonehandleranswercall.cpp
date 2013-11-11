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
* Description:  Handles answer call key press. 
*
*/


// INCLUDE FILES
#include    "phonehandleranswercall.h" 
#include    "phonehandlercontrol.h" 
#include    "phonehandlerdebug.h" 
#include    <RemConCallHandlingTarget.h> 

#include    <cphcltcommandhandler.h> 

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
// CPhoneHandlerAnswerCall::CPhoneHandlerAnswerCall
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerAnswerCall::CPhoneHandlerAnswerCall( 
                            CPhoneHandlerControl& aControl, 
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ),
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerAnswerCall::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerAnswerCall::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerAnswerCall::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerAnswerCall* CPhoneHandlerAnswerCall::NewL( 
                            CPhoneHandlerControl& aControl, 
                            TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::NewL() start" );
        
    CPhoneHandlerAnswerCall* self = new( ELeave ) CPhoneHandlerAnswerCall( 
                                                    aControl, 
                                                    aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::NewL() end" );
    
    return self;
    }

// Destructor
CPhoneHandlerAnswerCall::~CPhoneHandlerAnswerCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::~CPhoneHandlerAnswerCall()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerAnswerCall::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerAnswerCall::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::Process()" );
        
    iCommandHandler->Ata( GetStatus() );
    Activate();     
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerAnswerCall::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerAnswerCall::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerAnswerCall::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerAnswerCall::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
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
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerAnswerCall::RequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
