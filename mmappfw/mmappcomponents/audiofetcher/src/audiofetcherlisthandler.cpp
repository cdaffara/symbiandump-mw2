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
* Description:
*
*/

#include "audiofetcherlisthandler.h"
#include "audiofetcherlog.h"

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::NewL
// -----------------------------------------------------------------------------
//
CAudioFetcherListHandler* CAudioFetcherListHandler::NewL()
    {
    CAudioFetcherListHandler* self = new (ELeave) CAudioFetcherListHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::~CMFListHandler
// -----------------------------------------------------------------------------
//
CAudioFetcherListHandler::~CAudioFetcherListHandler()
    {
    WLOG("CAudioFetcherListHandler::~CAudioFetcherListHandler");
    }

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CAudioFetcherListHandler::ConstructL()
    {
    WLOG("CAudioFetcherListHandler::ConstructL");
    }

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::CAudioFetcherListHandler
// -----------------------------------------------------------------------------
//
CAudioFetcherListHandler::CAudioFetcherListHandler()
    {
    }

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::SetObserver
// -----------------------------------------------------------------------------
//
void CAudioFetcherListHandler::SetObserver( MAudioFetcherListHandlerObserver* aObserver )
    {
    iObserver = aObserver;
    }
    
// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::MdcaCount (from MDesCArray)
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherListHandler::MdcaCount() const
    {
    WLOG("CAudioFetcherListHandler::MdcaCount");
    return iObserver->ListboxItemCount();
    }

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::MdcaPoint (from MDesCArray)
// CEikListBox calls this function just before it draws list box item.
// -----------------------------------------------------------------------------
//
TPtrC16 CAudioFetcherListHandler::MdcaPoint( TInt aIndex ) const
    {
    WLOG("CAudioFetcherListHandler::MdcaPoint");
    return iObserver->ConstructListboxItem( aIndex );
    }

// -----------------------------------------------------------------------------
// CAudioFetcherListHandler::Panic
// -----------------------------------------------------------------------------
//
void CAudioFetcherListHandler::Panic( TInt aReason ) 
    {
    _LIT( KPanicCategory, "CAudioFetcherListHandler" );
    User::Panic( KPanicCategory, aReason ); 
    }

//  End of File  
