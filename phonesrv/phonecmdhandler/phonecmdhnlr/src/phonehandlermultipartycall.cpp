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
* Description:  Handles multiparty call key press. 
*
*/


// INCLUDE FILES
#include    "phonehandlermultipartycall.h" 
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
// CPhoneHandlerMultipartyCall::CPhoneHandlerMultipartyCall
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerMultipartyCall::CPhoneHandlerMultipartyCall(
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ), 
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerMultipartyCall::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerMultipartyCall::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerMultipartyCall::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerMultipartyCall* CPhoneHandlerMultipartyCall::NewL( 
            CPhoneHandlerControl& aControl,
            TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::NewL()" );
        
    CPhoneHandlerMultipartyCall* self = 
        new( ELeave ) CPhoneHandlerMultipartyCall( aControl, aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CPhoneHandlerMultipartyCall::~CPhoneHandlerMultipartyCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::~CPhoneHandlerMultipartyCall()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerMultipartyCall::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerMultipartyCall::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::Process()" );
    
    /*
    if( iControl.SwitchCall() )
        {
        iCommandHandler->Chld( GetStatus(), EPhCltChldTwo, 0 ); 
        }
    else
        { */
        // get command
        TInt command = const_cast<TInt&>( iControl.ChldCommand() );

        // get number
        TInt number = const_cast<TInt&>( iControl.ChldCallNumber() );
        
        iCommandHandler->Chld( GetStatus(),
                               static_cast<TPhCltChldCommand>( command ),
                               number ); 
        //}
        
    Activate();     
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerMultipartyCall::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerMultipartyCall::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerMultipartyCall::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerMultipartyCall::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
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
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerMultipartyCall::RequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
