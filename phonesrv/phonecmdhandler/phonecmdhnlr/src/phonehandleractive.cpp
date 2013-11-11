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
* Description:  Active object used to call asynchronous requests. 
*
*/


// INCLUDE FILES
#include "phonehandleractive.h" 
#include "phonehandlerobserver.h" 
#include "phonehandlerdebug.h" 

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
 
CPhoneHandlerActive* CPhoneHandlerActive::NewL( MPhoneHandlerObserver& aObserver )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::NewL() start" );
    CPhoneHandlerActive* self = new(ELeave) CPhoneHandlerActive( aObserver );
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::NewL() end" );
    return self;
    }

CPhoneHandlerActive::~CPhoneHandlerActive()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::~CPhoneHandlerActive() start" );
    
    Cancel();
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::~CPhoneHandlerActive() end" );
    }

CPhoneHandlerActive::CPhoneHandlerActive( MPhoneHandlerObserver& aObserver )
:   CActive(CActive::EPriorityStandard),
    iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerActive::SetActive()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerActive::SetActive()
    {
    CActive::SetActive();
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerActive::RunL()
// Informs caller of an asynchronous request that it has been completed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerActive::RunL()
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerActive::RunL() iStatus.Int()=%d", iStatus.Int() );
    
    TInt error( iStatus.Int() );
    iStatus =  KErrNone;
    iObserver.RequestCompleted( error );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerActive::RunError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPhoneHandlerActive::RunError( TInt aError )
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerActive::RunError(%d)", aError );

    // Avoid Panic in CActiveScheduler
    aError = KErrNone;

    COM_TRACE_( "[AccFW:PHONECMDHANDLER] CPhoneHandlerActive::RunError() - return KErrNone" );
            
    return aError; 
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerActive::DoCancel
// No need to cancel an asynchronous request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerActive::DoCancel()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::DoCancel() start" );
        
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerActive::DoCancel() end" );
    }

//
// End of file
