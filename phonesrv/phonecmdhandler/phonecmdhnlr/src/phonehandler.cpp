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
* Description:  Provides interface for client to load PhoneCmdHandler.
*
*/


// INCLUDE FILES
#include "phonehandlercontrol.h" 
#include "phonehandlerdebug.h" 
#include "PhoneHandler.h" 

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
// CPhoneHandler::CPhoneHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandler::CPhoneHandler()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandler::ConstructL( CRemConInterfaceSelector* aIfSelector )
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandler::ConstructL() *aIfSelector=%d", aIfSelector );
        
    iControl = CPhoneHandlerControl::NewL( aIfSelector );
    }

// -----------------------------------------------------------------------------
// CPhoneHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneHandler* CPhoneHandler::NewL()
    {
    CPhoneHandler* self = new ( ELeave ) CPhoneHandler();
    
    CleanupStack::PushL( self );
    self->ConstructL( NULL );
    CleanupStack::Pop( self );

    return self;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//    
EXPORT_C CPhoneHandler* CPhoneHandler::NewL( 
                            CRemConInterfaceSelector& aIfSelector )
    {
    CPhoneHandler* self = new ( ELeave ) CPhoneHandler();
    
    CleanupStack::PushL( self );
    self->ConstructL( &aIfSelector );
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
EXPORT_C CPhoneHandler::~CPhoneHandler()
    {
    delete iControl;
    }

// ======================== OTHER EXPORTED FUNCTIONS ===========================

// End of File
