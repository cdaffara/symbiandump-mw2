/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This implements CPropertyWatcher class.
*
*/


// SYSTEM INCLUDE
#include <e32base.h>
#include <e32std.h>  // For DLL.

// USER INCLUDE
#include "propertywatcher.h"
#include "harvesterlog.h" // For debugging.
#include "listener.h" 

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CPropertyWatcher::GetInstanceL
// ---------------------------------------------------------------------------
//
EXPORT_C CPropertyWatcher* CPropertyWatcher::GetInstanceL() 
    {
    WRITELOG( "CPropertyWatcher::GetInstanceL() - begin" ); 
  
    CPropertyWatcher* me = NULL;
    me = static_cast<CPropertyWatcher*>( Dll::Tls() );
    if ( !me )
        {
        CPropertyWatcher* self = new( ELeave ) CPropertyWatcher();
        User::LeaveIfError( Dll::SetTls( self ) );
    
        WRITELOG( "CPropertyWatcher::GetInstanceL() - end returning self" ); 
        return self;	
        }
    else
        {
        WRITELOG( "CPropertyWatcher::GetInstanceL() - end returning me" ); 
        return me;	
        }			    
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::Delete
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPropertyWatcher::Delete()
    {
    WRITELOG( "CPropertyWatcher::Delete() - begin" ); 
    
    CPropertyWatcher *me = NULL;
    me = static_cast<CPropertyWatcher*>( Dll::Tls() );
    
    if ( me )
        {
        delete me; 
        me = NULL;
        Dll::SetTls( NULL ); // Free TLS.
        }
    
    WRITELOG( "CPropertyWatcher::Delete() - end" ); 
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::ListenKeyChangesL
// ---------------------------------------------------------------------------
//
EXPORT_C void CPropertyWatcher::ListenKeyChangesL( 
		const TUid aPropertyCategory,
		const TUint aKey,
		const MKeyObserver* aValueObserver )
    {
	WRITELOG( "CPropertyWatcher::ListenKeyChanges() - begin" ); 

	const TInt listenerIndex = FindListener ( aPropertyCategory, aKey ); // Finds listener.
	if ( listenerIndex >=  0 )
		{
		iListenersArray[listenerIndex]->RegisterNewClientForKeyValueL( aValueObserver );
		return;
		}
    
	// Create new listener.
	CListener* listener = CListener::NewLC( 
			 aPropertyCategory,
			 aKey
			 ); 
	 
	listener->RegisterNewClientForKeyValueL( aValueObserver );
	
	// Add creted listener to array.
	User::LeaveIfError( iListenersArray.Append( listener ) ); // Transfer ownership.
	
	CleanupStack::Pop( listener );

	WRITELOG( "CPropertyWatcher::ListenKeyChanges() - end" ); 
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::ListenKeyAndStatusChanges
// ---------------------------------------------------------------------------
//
EXPORT_C void CPropertyWatcher::ListenKeyAndStatusChangesL( 
		const TUid aPropertyCategory,
		const TUint aKey,
		const MKeyAndStatusObserver* aStatusObserver )
    {
	WRITELOG( "CPropertyWatcher::ListenKeyAndStatusChanges - begin" ); 

	const TInt listenerIndex = FindListener ( aPropertyCategory, aKey ); // Finds listener.
	if ( listenerIndex >= 0 )
		{
		iListenersArray[listenerIndex]->RegisterNewClientForKeyAndStatusL( aStatusObserver );
		return;
		}
    
	// Create new listener.
	CListener* listener = CListener::NewLC( 
			 aPropertyCategory,
			 aKey
			 ); 
	 
	listener->RegisterNewClientForKeyAndStatusL( aStatusObserver );
	
	// Add created listener to array.
	User::LeaveIfError( iListenersArray.Append( listener ) ); // Transfer ownership.
	
	CleanupStack::Pop( listener );
	
	WRITELOG( "CPropertyWatcher::ListenKeyChanges() - end" ); 
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::StopListeningKeyChanges
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPropertyWatcher::StopListeningKeyChanges( 
		const TUid aPropertyCategory,
		const TUint aKey,
		const MKeyObserver* aKeyObserver )
    {
    WRITELOG( "CPropertyWatcher::StopListeningKeyChangesL() - begin" ); 
	
	const TInt listenerIndex = FindListener ( aPropertyCategory, aKey ); // Finds listener.
	if ( listenerIndex >= 0 )
		{
		iListenersArray[listenerIndex]->UnregisterKeyClient( aKeyObserver );
		}
	
	WRITELOG( "CPropertyWatcher::StopListeningKeyChangesL() - end" ); 
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::StopListeningKeyAndStatusChanges
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPropertyWatcher::StopListeningKeyAndStatusChanges( 
		const TUid aPropertyCategory,
		const TUint aKey,
		MKeyAndStatusObserver* aStatusObserver )
    {
    WRITELOG( "CPropertyWatcher::StopListeningKeyAndStatusChanges() - begin" ); 
	
	const TInt listenerIndex = FindListener ( aPropertyCategory, aKey ); // Finds listener.
	if ( listenerIndex >= 0 )
		{
		iListenersArray[listenerIndex]->UnregisterKeyAndStatusClient( aStatusObserver );
		}
	
	WRITELOG( "CPropertyWatcher::StopListeningKeyAndStatusChanges() - end" ); 
    }

// ---------------------------------------------------------------------------
// CPropertyWatcher::~CPropertyWatcher
// ---------------------------------------------------------------------------
//	
CPropertyWatcher::~CPropertyWatcher()
	{
	WRITELOG( "CPropertyWatcher::~CPropertyWatcher() - begin" ); 
	
	iListenersArray.ResetAndDestroy(); // Clean array and objects.
    iListenersArray.Close(); 
	
	WRITELOG( "CPropertyWatcher::~CPropertyWatcher() - end" ); 
	}

// ---------------------------------------------------------------------------
// CPropertyWatcher::FindListener
// ---------------------------------------------------------------------------
//	
TInt CPropertyWatcher::FindListener( const TUid aPropertyCategory, const TUint aKey )	const
    {
    WRITELOG( "CPropertyWatcher::FindListener() - begin" ); 
	
    TInt listenerIndex ( KErrNotFound );
    
    const TInt count( iListenersArray.Count() );
    for ( TInt i = count; --i >= 0; )
    	{
    	CListener& listenerItem = *iListenersArray[i];
    	
    	if ( aKey == listenerItem.ObservedKey() &&
			aPropertyCategory == listenerItem.ObservedCategory() ) 
			{
			listenerIndex =  i; 
			}
		}
	
	WRITELOG( "CPropertyWatcher::FindListener() - end" ); 
	return listenerIndex;
    }

// End of file.
