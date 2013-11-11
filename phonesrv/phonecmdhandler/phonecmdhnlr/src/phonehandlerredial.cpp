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
* Description:  Handles last number redial key press. 
*
*/


// INCLUDE FILES

#include "phonehandlerredial.h" 
#include "phonehandlercontrol.h" 
#include "phonehandlerdebug.h" 
#include <RemConCallHandlingTarget.h> 
#include <cphcltcommandhandler.h> 
#include <phclttypes.h> 

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <logfilterandeventconstants.hrh>
#endif

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::CPhoneHandlerLastNumberRedial
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerLastNumberRedial::CPhoneHandlerLastNumberRedial(
                            CPhoneHandlerControl& aControl,
                            TRemConExtCallHandlingApiOperationId aOperation )
: iControl( aControl ), 
  iOperation( aOperation )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerLastNumberRedial::ConstructL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::ConstructL() start" );
    
    User::LeaveIfError(iFsSession.Connect());

    iLogClientPtr = CLogClient::NewL(iFsSession);
    iLogViewRecentPtr = CLogViewRecent::NewL(*iLogClientPtr);
    LoadServiceL();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerLastNumberRedial* CPhoneHandlerLastNumberRedial::NewL( 
    CPhoneHandlerControl& aControl,
    TRemConExtCallHandlingApiOperationId aOperation )
    {
    CPhoneHandlerLastNumberRedial* self = 
        new (ELeave) CPhoneHandlerLastNumberRedial( aControl, aOperation );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CPhoneHandlerLastNumberRedial::~CPhoneHandlerLastNumberRedial()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::CPhoneHandlerLastNumberRedial() start" );
    
    delete iLogViewRecentPtr;
    delete iLogClientPtr;
    iFsSession.Close();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::CPhoneHandlerLastNumberRedial() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::Process
// Starts to process service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerLastNumberRedial::Process()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Process() start" );
    
    TBool ret = EFalse;
        
    TRAPD( err, ret = iLogViewRecentPtr->
        SetRecentListL( KLogRecentOutgoingCalls, GetStatus() ) ); 
    if( KErrNone == err && ret )
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Process() There is last redial number" );
        Activate();
        }
    else
        {
        if( err != KErrNone )
            {
            COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Process() failed err=%d", err );
            RequestCompleted( err ); 
            }
        else
            {
            COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Process() failed err=%d", KErrNotFound );
            RequestCompleted( KErrNotFound ); 
            }
        }
        
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Process() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::Delete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerLastNumberRedial::Delete()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::Delete()" );
        
    delete this;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerLastNumberRedial::RequestCompleted
// Handler for completed asynchronous calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerLastNumberRedial::RequestCompleted( const TInt aError )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::RequestCompleted() aError=%d, iState=%d", aError, iState );
    
    if( KErrNone != aError )
        {
        // cancel process and return an error code
        iState = EPhoneHandlerState2;
        }
    
    switch( iState )
        {
        case EPhoneHandlerState1:
            {
            NextState();
            
            const TDesC& number = iLogViewRecentPtr->Event().Number();
            COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::RequestCompleted() phone number is %S", &number );
            // make a call with last dialed phone number
            iCommandHandler->Atd( GetStatus(), number );

            Activate(); 
            break;
            }
        
        case EPhoneHandlerState2:
            {
            NextState();
            iControl.CommandInitiator().SendResponse( GetStatus(), 
                                                      iOperation,
                                                      aError );
            Activate();
            break;
            }
            
        case EPhoneHandlerState3:
            {
            IdleState();
            delete this; 
            break;
            }
                
        default:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::RequestCompleted() Unspecified state" );
            break;
            }
        };
        
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerLastNumberRedial::RequestCompleted() end" );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
