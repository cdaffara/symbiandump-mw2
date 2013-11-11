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
#include    "phonehandlervoicedial.h" 
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
// CPhoneHandlerVoiceDial::CPhoneHandlerVoiceDial
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerVoiceDial::CPhoneHandlerVoiceDial( 
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ), 
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerVoiceDial::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerVoiceDial::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::ConstructL() start" );
        
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerVoiceDial::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerVoiceDial* CPhoneHandlerVoiceDial::NewL( 
                            CPhoneHandlerControl& aControl, 
                            TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::NewL() start" );
        
    CPhoneHandlerVoiceDial* self = new( ELeave ) CPhoneHandlerVoiceDial( 
                                                    aControl, 
                                                    aOperation );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::NewL() end" );
        
    return self;
    }

    
// Destructor
CPhoneHandlerVoiceDial::~CPhoneHandlerVoiceDial()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::~CPhoneHandlerVoiceDial()" );
    
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerVoiceDial::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerVoiceDial::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::Process()" );

#if 0 // removed due to SCB CR PMEO-7JRFAX      
    iCommandHandler->Bvra( GetStatus(), iControl.VoiceDialStatus() ); 
#endif // #if 0     
    Activate(); 
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerVoiceDial::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerVoiceDial::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerVoiceDial::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerVoiceDial::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::RequestCompleted() aError=%d, iState=%d", aError, iState );
        
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
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerVoiceDial::RequestCompleted() Unspecified state" );
            break;
            }
        };
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
