/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CVmbxSimHandler class
*
*/


// INCLUDE FILES
#include <mpbutil.h>
#include <cvoicemailboxentry.h>

#include "vmbxpbkstore.h"
#include "vmbxlogger.h"
#include "vmbxsimhandler.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxSimHandler::CVmbxSimHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxSimHandler::CVmbxSimHandler( )             
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::CVmbxSimHandler =>" );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::CVmbxSimHandler <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxSimHandler::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::ConstructL =>" );
#ifndef __WINS__
    // open created phonebook
    iPbkStore = CVmbxPbkStore::NewL();
#endif
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxSimHandler* CVmbxSimHandler::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::NewL =>" );
    CVmbxSimHandler* self = new( ELeave ) CVmbxSimHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::~CVmbxSimHandler
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxSimHandler::~CVmbxSimHandler()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::~CVmbxSimHandler =>" );
#ifndef __WINS__
    delete iPbkStore;
#endif
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::~CVmbxSimHandler <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::GetL
// Fetches mailbox number from Sim
// ---------------------------------------------------------------------------
//
void CVmbxSimHandler::GetL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::GetL =>" );
    iPbkStore->GetL( aEntry );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::GetL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::Save
// Saves vmbx number to Sim
// ---------------------------------------------------------------------------
//
TInt CVmbxSimHandler::Save( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::Save =>" );
    TInt result = iPbkStore->Write( aEntry );
    VMBLOGSTRING2( "VMBX: CVmbxSimHandler::Save: result=%I <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::IsWritable
// Current file is writable
// ---------------------------------------------------------------------------
//
TBool CVmbxSimHandler::IsWritable()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::IsWritable: =>" );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::IsWritable: <=" );
    return iPbkStore->IsWritable();
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::PhoneBookType
// Returns Sim file type
// ---------------------------------------------------------------------------
//
TVmbxSimPhonebookType CVmbxSimHandler::PhoneBookType()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::PhoneBookType =>" );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::PhoneBookType <=" );
    return iPbkStore->PhoneBookType();
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::NotifyStoreEvent
// Notify phonebook store evnet
// ---------------------------------------------------------------------------
//
void CVmbxSimHandler::NotifyStoreEvent( TRequestStatus& aStatus,
                                         TUint32& aEvent,
                                         TInt& aIndex ) const
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::NotifyStoreEvent: =>" );
    iPbkStore->PhonebookStore().NotifyStoreEvent( aStatus, aEvent, aIndex );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::NotifyStoreEvent: <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSimHandler::CancelAsyncRequest
// Cancel phonebook store oberseved
// ---------------------------------------------------------------------------
//    
void CVmbxSimHandler::CancelAsyncRequest( TInt aReqToCancel ) const
    {
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::CancelAsyncRequest: =>" );
    iPbkStore->PhonebookStore().CancelAsyncRequest( aReqToCancel );
    VMBLOGSTRING( "VMBX: CVmbxSimHandler::CancelAsyncRequest: <=" );
    }

//End of file
