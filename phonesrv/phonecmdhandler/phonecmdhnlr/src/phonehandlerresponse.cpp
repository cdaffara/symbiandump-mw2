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
* Description:  Sends responses to RemCon FW.  
*
*/


// INCLUDE FILES
#include    "phonehandlerresponse.h" 
#include    "phonehandlercontrol.h" 
#include    "phonehandleractive.h" 
#include    "phonehandlerdebug.h" 
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
// CPhoneHandlerResponse::CPhoneHandlerResponse
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerResponse::CPhoneHandlerResponse( CPhoneHandlerControl& aControl )
: iControl( aControl )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerResponse::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerResponse::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::ConstructL() start" );
        
    iActive = CPhoneHandlerActive::NewL( *this );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerResponse::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerResponse* CPhoneHandlerResponse::NewL( 
        CPhoneHandlerControl& aControl )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::NewL() start" );
        
    CPhoneHandlerResponse* self = 
        new( ELeave ) CPhoneHandlerResponse( aControl );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::NewL() end" );
    
    return self;
    }

// Destructor
CPhoneHandlerResponse::~CPhoneHandlerResponse()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::~CPhoneHandlerResponse()" );
    
    delete iActive;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerResponse::SendResponse
// Sends response to RemCon FW.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerResponse::SetResponse( 
            TRemConExtCallHandlingApiOperationId aOperation, TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerResponse::Process(aOperation=%d,aError=%d)",aOperation, aError );
    
    iOperation = aOperation;
    iError = aError;
    }
        
// -----------------------------------------------------------------------------
// CPhoneHandlerResponse::SendResponse
// Sends response to RemCon FW.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerResponse::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::Process()" );
    
    iControl.CommandInitiator().SendResponse( iActive->iStatus, 
                                              iOperation,
                                              iError );
    iActive->SetActive();                       
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerResponse::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerResponse::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerResponse::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerResponse::RequestCompleted( const TInt aError )
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerResponse::RequestCompleted() aError=%d", aError );
    
    (void)aError; // avoid warning in WINSCW urel build
    
    // Think about adding value to TRemConExtCallHandlingApiOperationId 
    // in RemConExtensionApi.h like "ERemConExtNoCallHandlingOperation".
    // In this method iOperation should be set to that value.
    iError = KErrNone;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
