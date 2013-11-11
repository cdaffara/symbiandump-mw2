/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Publish and Subscribe key watcher
*
*/


#include <e32base.h>
#include <e32property.h>
#include "mpxpskeywatcher.h"
#include "mpxpskeyobserver.h"

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXPSKeyWatcher::CMPXPSKeyWatcher(  TUid aUid, TInt aKey,
                                     MMPXPSKeyObserver* aObserver ) :
                                                     CActive( EPriorityHigh ),
                                                     iObserver( aObserver ),
                                                     iUid( aUid ),
                                                     iKey( aKey )
                                                      
    {
    }


// ---------------------------------------------------------------------------
// 2nd phased Constructor
// ---------------------------------------------------------------------------
//
void CMPXPSKeyWatcher::ConstructL()
    {
    CActiveScheduler::Add(this);
    
    iProperty.Attach( iUid, iKey );
    iProperty.Subscribe( iStatus );
    SetActive();
    }


// ---------------------------------------------------------------------------
// Two-Phased COnstructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXPSKeyWatcher* CMPXPSKeyWatcher::NewL( TUid aUid, TInt aKey,
                                                   MMPXPSKeyObserver* aObserver )
    {
    CMPXPSKeyWatcher* self = new(ELeave) CMPXPSKeyWatcher( aUid, 
                                                           aKey, 
                                                           aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXPSKeyWatcher::~CMPXPSKeyWatcher()
    {
    Cancel();  // Cancels waiting request
    iProperty.Close();
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::RunL
// ---------------------------------------------------------------------------
//
void CMPXPSKeyWatcher::RunL()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    
    // Notify Observer and start watching again
    if( iObserver )
        {
        iObserver->HandlePSEvent( iUid, iKey );
        }
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::DoCancel
// ---------------------------------------------------------------------------
//
void CMPXPSKeyWatcher::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::GetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::GetValue( TInt& aValue )
    {
    return iProperty.Get( aValue );
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::GetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::GetValue( TDes8& aValue )
    {
    return iProperty.Get( aValue );
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::GetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::GetValue( TDes16& aValue )
    {
    return iProperty.Get( aValue );
    }
    
// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::SetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::SetValue( TInt aValue )
    {
    return iProperty.Set( aValue );
    }

// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::SetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::SetValue( const TDesC8& aValue )
    {
    return iProperty.Set( aValue );
    }
    
// ---------------------------------------------------------------------------
// CMPXPSKeyWatcher::SetValue
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMPXPSKeyWatcher::SetValue( const TDesC16& aValue )
    {
    return iProperty.Set( aValue );
    }

