/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/


// INCLUDE FILES
#include <e32std.h>
#include "CCLFEventHandler.h"
#include "MCLFEventHandlerObserver.h"
#include "MGDebugPrint.h"

// MACROS
// Macro for calling a member function through function pointer
#define CLF_CALL_MEMBER_FN( aObject, aPtrToMember ) \
                                           (( aObject ).*( aPtrToMember ))

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFEventHandler::CCLFEventHandler
// -----------------------------------------------------------------------------
//
CCLFEventHandler::CCLFEventHandler( CCLFServerProxy& aEventHandler,
                                           MCLFEventHandlerObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iEventHandler( aEventHandler ),
      iObserver( aObserver ),
      iOperationFunction( &CCLFServerProxy::GetUpdateEndEvent ),
      iCancelFunction( &CCLFServerProxy::CancelGetEvent ),
      iObserverFunction( &MCLFEventHandlerObserver::HandleUpdateEndEventL )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::NewL
// -----------------------------------------------------------------------------
//
CCLFEventHandler* CCLFEventHandler::NewL( CCLFServerProxy& aEventHandler,
                                        MCLFEventHandlerObserver& aObserver )
    {
    return new( ELeave ) CCLFEventHandler( aEventHandler, aObserver );
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::~CCLFEventHandler
// Destructor
// -----------------------------------------------------------------------------
//
CCLFEventHandler::~CCLFEventHandler()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::DoCancel
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::DoCancel()
    {
    CLF_CALL_MEMBER_FN( iEventHandler, iCancelFunction )();
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::RunL
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::RunL()
    {
    // observer function can leave
    CLF_CALL_MEMBER_FN( iObserver, iObserverFunction )( iStatus.Int() );
    DoStart();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::RunError
// -----------------------------------------------------------------------------
//
TInt CCLFEventHandler::RunError( const TInt aError )
    {
    MG_DEBUG2( re1, "[CLF]\t CCLFEventHandler::RunError: %d", aError ); 

    return aError;
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::ObserverUpdateEndEvent
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::ObserverUpdateEndEvent()
    {
    Cancel();
    iOperationFunction = &CCLFServerProxy::GetUpdateEndEvent;
    iCancelFunction = &CCLFServerProxy::CancelGetEvent;
    iObserverFunction = &MCLFEventHandlerObserver::HandleUpdateEndEventL;
    DoStart();
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::ObserverUpdateStartEvent
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::ObserverUpdateStartEvent()
    {
    Cancel();
    iOperationFunction = &CCLFServerProxy::GetUpdateStartEvent;
    iCancelFunction = &CCLFServerProxy::CancelGetUpdateStartEvent;
    iObserverFunction = &MCLFEventHandlerObserver::HandleUpdateStartEventL;
    DoStart();
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::DoStart
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::DoStart()
    {
    CLF_CALL_MEMBER_FN( iEventHandler, iOperationFunction )( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCLFEventHandler::GetChangedItemsL
// -----------------------------------------------------------------------------
//
void CCLFEventHandler::GetChangedItemsL( RArray<TUint>& aItemArray )
    {
    User::LeaveIfError( iEventHandler.FetchItemListData( aItemArray ) );
    }

//  End of File
