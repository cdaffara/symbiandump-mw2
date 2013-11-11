/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the class CMcn.
*
*/


// INCLUDE FILES
#include <cmcn.h>
#include <ccbsmcnlistener.h>
#include <mcbsmcnobserver.h>
#include <cbsmcnpanic.h>
#include "CbsLogger.h"
#include <cmcntopicarray.h>

// CONSTANTS
const TInt KReservedSpaceForMcnObservers = 3;

// ==================== LOCAL FUNCTIONS ====================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMcn::CMcn
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMcn::CMcn()
    {
    }

// -----------------------------------------------------------------------------
// CMcn::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMcn::ConstructL()
    {    
    CBSLOGSTRING("CBSMCNCLIENT: >>> CMcn::ConstructL()");    

    User::LeaveIfError( iSession.Connect() );
    iObservers = new ( ELeave ) CArrayPtrFlat< MCbsMcnObserver >
        ( KReservedSpaceForMcnObservers );
    iMcnListener = CCbsMcnListener::NewL( *this, iSession );

    CBSLOGSTRING("CBSMCNCLIENT: <<< CMcn::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CMcn::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMcn* CMcn::NewL() 
    {
    CMcn* self = new ( ELeave ) CMcn;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
EXPORT_C CMcn::~CMcn() 
    {
    if ( iObservers != NULL ) 
        {
        iObservers->ResetAndDestroy();
        }
    delete iObservers;
    delete iMcnListener;
    iSession.Close();
    }

// -----------------------------------------------------------------------------
// CMcn::RegisterL
// Registers an observer to CBS server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMcn::RegisterL( 
    MCbsMcnObserver* aObserver, 
    const CMcnTopicArray& aArray ) 
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> CMcn::RegisterL()");

    __ASSERT_DEBUG( aObserver != 0, CbsMcnPanic( EMcnObserverNull ) );
    iObservers->AppendL( aObserver );
    iSession.RegisterL( aArray );

    CBSLOGSTRING("CBSMCNCLIENT: <<< CMcn::RegisterL()");
    }

// -----------------------------------------------------------------------------
// CMcn::UnregisterL
// Unregisters a previously registered MCN observer clearing
// any topic subscriptions of this client in CBS server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMcn::Unregister( 
    MCbsMcnObserver* aObserver )
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> CMcn::Unregister()");

    __ASSERT_DEBUG( aObserver !=0 , CbsMcnPanic( EMcnObserverNull ) );
    TInt count( iObservers->Count() );
    for( TInt i = 0; i < count; i++ )
        {
        MCbsMcnObserver* observer = iObservers->At( i );
        if ( observer == aObserver )
            {
            iObservers->Delete( i );
            break;
            }
        }
    
    // ignore result for now
    TInt result( KErrNone );
    TRAP( result, iSession.ClearSubscriptionsL() );

    CBSLOGSTRING("CBSMCNCLIENT: <<< CMcn::Unregister()");
    }

// -----------------------------------------------------------------------------
// CMcn::GetCellInfo
// Retrieves the current MCN message (Cell Info, topic 050) from CBS 
// server. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMcn::GetCellInfo( 
    TDes& aMcnMessage ) const
    {
    return iSession.GetCellInfo( aMcnMessage );
    }

// -----------------------------------------------------------------------------
// CMcn::RelayMessageL
// Relays aMcnMessage and topic number to the registered observers.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMcn::RelayMessage( 
    TCbsMcnMessage& aMcnMessage )
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> CMcn::RelayMessage()");

    TInt count( iObservers->Count() );

    CBSLOGSTRING2("CBSMCNCLIENT: CMcn::RelayMessage(): topicNumber: %d", aMcnMessage.iTopicNumber );
    CBSLOGSTRING2("CBSMCNCLIENT: CMcn::RelayMessage(): Network mode: %d (0=GSM, 1=WCDMA, 2=Not defined)", aMcnMessage.iNetworkMode );
    CBSLOGSTRING2("CBSMCNCLIENT: CMcn::RelayMessage(): Observer count: %d", count );

    for ( TInt i = 0; i < count; )
        {
        TInt prevCount = count;

        CBSLOGSTRING2("CBSMCNCLIENT: CMcn::RelayMessage(): Calling CellInfoChanged... Observer: %d", i);
        
        // Notify all registered observers        
        iObservers->At( i )->CellInfoChanged( aMcnMessage );        
        CBSLOGSTRING("CBSMCNCLIENT: CMcn::RelayMessage(): CellInfoChanged call finished.");

        // Check if the client unregistered it's observer. If so, we don't
        // increase the loop counter.
        count = iObservers->Count();
        if ( count == prevCount )
            {
            i++;
            }        
        }
    CBSLOGSTRING("CBSMCNCLIENT: <<< CMcn::RelayMessage()");
    }

// -----------------------------------------------------------------------------
// CMcn::GetInfoMessage
// Retrieves the current info message from CBS server. 
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMcn::GetInfoMessage( 
    TDes& aInfoMessage, 
    TInt aTopicNumber ) const
    {
    return iSession.GetInfoMessage( aInfoMessage, aTopicNumber );   
    }

//  End of File  
