/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Iterator class for going through remote party info
*               of ongoing calls.
*
*/

#include <callremotepartyinformation.h>    

#include "ccallremotepartyinfoiter.h"
#include "mcallremotepartyinfo.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfoIter::
    CCallRemotePartyInfoIter( CCallRemotePartyInfos& aRemotePartyInfos )
    : iRemotePartyInfos( aRemotePartyInfos )
    { 
    First();    
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfoIter::~CCallRemotePartyInfoIter()
    {
    
    }
// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfoIter*
    CCallRemotePartyInfoIter::NewLC( CCallRemotePartyInfos& aRemotePartyInfos )
    {
    CCallRemotePartyInfoIter* self =
        new (ELeave)CCallRemotePartyInfoIter( aRemotePartyInfos );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfoIter*
    CCallRemotePartyInfoIter::NewL( CCallRemotePartyInfos& aRemotePartyInfos )
    {
    CCallRemotePartyInfoIter* self =
        CCallRemotePartyInfoIter::NewLC( aRemotePartyInfos );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfoIter::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Intializes the current item to the first remote party information.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallRemotePartyInfoIter::First()
    {
    iCurrentIndex = 0; 
    }

// ---------------------------------------------------------------------------
// Tests whether the last item has been reached.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CCallRemotePartyInfoIter::IsDone() const
    {
    const RPointerArray<CCallRemotePartyInfo>& infoArray =
        iRemotePartyInfos.RemotePartyInfos();
    
    TInt count (infoArray.Count());    
    if ( iCurrentIndex < count  )
        return EFalse;
    else
        return ETrue;
    }

// ---------------------------------------------------------------------------
// Advances the current item to the next remote party information.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallRemotePartyInfoIter::Next()
    {
    iCurrentIndex += 1;
    }

// ---------------------------------------------------------------------------
// Returns the count of items.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCallRemotePartyInfoIter::Count() const
    {
    const RPointerArray<CCallRemotePartyInfo>& infoArray =
            iRemotePartyInfos.RemotePartyInfos();
        
    return infoArray.Count();   
    }

// ---------------------------------------------------------------------------
// Returns the current remote party information.
// ---------------------------------------------------------------------------
//
EXPORT_C const MCallRemotePartyInfo& CCallRemotePartyInfoIter::Current() const
    {    
    return *iRemotePartyInfos.RemotePartyInfos()[iCurrentIndex];     
    }

