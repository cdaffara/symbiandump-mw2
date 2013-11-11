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
* Description:  Handles DTMF tone. 
*
*/


// INCLUDE FILES
#include    "phonehandlerdtmf.h" 
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
// CPhoneHandlerDTMF::CPhoneHandlerDTMF
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerDTMF::CPhoneHandlerDTMF( 
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ), 
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDTMF::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDTMF::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDTMF::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerDTMF* CPhoneHandlerDTMF::NewL(  
            CPhoneHandlerControl& aControl,
            TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::NewL()" );
        
    CPhoneHandlerDTMF* self = new( ELeave ) CPhoneHandlerDTMF( aControl, 
                                                               aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CPhoneHandlerDTMF::~CPhoneHandlerDTMF()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::~CPhoneHandlerDTMF()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDTMF::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDTMF::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::Process()" );
        
    iCommandHandler->Vts( GetStatus(), 
                          iControl.Tone(), 
                          EPhCltDtmfNotUsed );
    Activate(); 
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDTMF::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDTMF::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerDTMF::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerDTMF::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerDTMF::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
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
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerDTMF::RequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
