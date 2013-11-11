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
* Description:  Handles end call and reject call key presses. 
*
*/


// INCLUDE FILES
#include    "phonehandlerendcall.h" 
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
// CPhoneHandlerEndCall::CPhoneHandlerEndCall
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerEndCall::CPhoneHandlerEndCall(  
                            CPhoneHandlerControl& aControl, 
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ),
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerEndCall::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerEndCall::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerEndCall::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerEndCall* CPhoneHandlerEndCall::NewL( 
                        CPhoneHandlerControl& aControl,
                        TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::NewL()" );
        
    CPhoneHandlerEndCall* self = new( ELeave ) CPhoneHandlerEndCall( 
                                                    aControl,
                                                    aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CPhoneHandlerEndCall::~CPhoneHandlerEndCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::~CPhoneHandlerEndCall()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerEndCall::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerEndCall::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::Process()" );
    
    iCommandHandler->Chup( GetStatus() );    
    Activate();                         
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerEndCall::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerEndCall::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerEndCall::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerEndCall::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerEndCall::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
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
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerEndCall::RequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
