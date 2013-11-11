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
* Description:  This implements CListener class.
*
*/


// INCLUDE FILES
#include "listener.h"
#include "mpropertyobserver.h"
#include "harvesterlog.h" // For debugging.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CListener::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CListener* CListener::NewL( const TUid aPropertyCategory,
		const TUint aKey )
    {
    WRITELOG( "CListener::NewL() - begin" );
    
    CListener* self = CListener::NewLC( aPropertyCategory, aKey );
    CleanupStack::Pop( self );
    
    WRITELOG( "CListener::NewL() - end" );
    return self;
    }

// -----------------------------------------------------------------------------
// CListener::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CListener* CListener::NewLC( const TUid aPropertyCategory,
		const TUint aKey )
    {
    WRITELOG( "CListener::NewLC() - begin" );
    
    CListener* self = new ( ELeave ) CListener( aPropertyCategory, aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    WRITELOG( "CListener::NewLC - end" );
    return self;
    }

// -----------------------------------------------------------------------------
// CListener::~CListener
// C++ default destructor
// -----------------------------------------------------------------------------
//    
CListener::~CListener()
    {
    WRITELOG( "CListener::~CListener - begin" );
    
    Cancel(); // Cancels any existing active object orders for iStatus.
    
    iProperty.Close();
    
    iArrayForKeyValue.Reset();
    iArrayForKeyValue.Close();
    
    iArrayForKeyAndStatus.Reset();
    iArrayForKeyAndStatus.Close();

    WRITELOG( "CListener::~CListener - end" );   
    }

// -----------------------------------------------------------------------------
// CListener::ObservedKey
// Returns observed key.
// -----------------------------------------------------------------------------
//
TUint CListener::ObservedKey() const
    {
    WRITELOG( "CListener::ObservedKey" );
    
    return iKey; 
    }

// -----------------------------------------------------------------------------
// CListener::ObservedCategory
// Returns observed key's Category.
// -----------------------------------------------------------------------------
//
TUid CListener::ObservedCategory() const
    {
    WRITELOG( "CListener::ObservedCategory" );
    
    return iPropertyCategory; 
    }

// -----------------------------------------------------------------------------
// CListener::RegisterNewClientForKeyValueL
// 
// -----------------------------------------------------------------------------
//
void CListener::RegisterNewClientForKeyValueL( const MKeyObserver* aKeyObserver )
    {
    WRITELOG( "CListener::RegisterNewClientForKeyValue" );
    
    User::LeaveIfError( iArrayForKeyValue.Append( aKeyObserver ) );
    }

// -----------------------------------------------------------------------------
// CListener::RegisterNewClientForKeyAndStatusL
// 
// -----------------------------------------------------------------------------
//
void CListener::RegisterNewClientForKeyAndStatusL( 
		const MKeyAndStatusObserver* aKeyAndStatusObserver )
    {
    WRITELOG( "CListener::RegisterNewClientForKeyAndStatus" );
    
    User::LeaveIfError( iArrayForKeyAndStatus.Append( aKeyAndStatusObserver ) );
    }

// -----------------------------------------------------------------------------
// CListener::UnregisterKeyClient
//
// -----------------------------------------------------------------------------
//
void CListener::UnregisterKeyClient( const MKeyObserver* aKeyObserver )
    {
    WRITELOG( "CListener::RemoveClient - begin" ); 
    
    const TInt listIndex = iArrayForKeyValue.Find( aKeyObserver );
    if( listIndex != KErrNotFound )
    	{
    	iArrayForKeyValue.Remove( listIndex );
    	}
 
    WRITELOG( "CListener::RemoveClient - end" );   
    }

// -----------------------------------------------------------------------------
// CListener::UnregisterKeyAndStatusClient
//
// -----------------------------------------------------------------------------
//
void CListener::UnregisterKeyAndStatusClient( const MKeyAndStatusObserver* aKeyAndStatusObserver )
    {
    WRITELOG( "CListener::UnregisterKeyAndStatusClient - begin" ); 
    
    const TInt listIndex = iArrayForKeyAndStatus.Find( aKeyAndStatusObserver );
    if( listIndex != KErrNotFound )
    	{
    	iArrayForKeyAndStatus.Remove( listIndex );
    	}
 
    WRITELOG( "CListener::UnregisterKeyAndStatusClient - end" );   
    }

// -----------------------------------------------------------------------------
// CListener::CListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CListener::CListener( const TUid aPropertyCategory, const TUint aKey ) :
		CActive( CActive::EPriorityUserInput ),
		iPropertyCategory( aPropertyCategory ),
		iKey( aKey )
 	{
 	// No implementation required.
 	}

// -----------------------------------------------------------------------------
// CListener::ConstructL
// Construct member variable that is to access the
// kernel. 
// -----------------------------------------------------------------------------
//
void CListener::ConstructL()
    {
    WRITELOG( "CListener::ConstructL - begin" );
    
    CActiveScheduler::Add( this );
    
	// Attach to key.
 	User::LeaveIfError( iProperty.Attach( 
 			iPropertyCategory,
 			iKey,
 			EOwnerThread ) );
 	
 	// Sets object active.
	StartListening();
    
    WRITELOG( "CListener::ConstructL - end" );
    }

// -----------------------------------------------------------------------------
// CListener::RunL
// This function will be called upon a change in the watched key.
// From class CActive.
// -----------------------------------------------------------------------------
//
void CListener::RunL()
    {
#ifdef _DEBUG
    WRITELOG1( "CListener::RunL - begin, iStatus: %d",  iStatus.Int() ); 
#endif
    
    if( iStatus.Int() == KErrNone )
    	{
    	TInt keyValue( KErrNone );
    	User::LeaveIfError( iProperty.Get( keyValue ) );
    	SendNotificationsL( keyValue, iStatus );
    	}
    
    // Start listening again.
    StartListening();
    
    WRITELOG( "CListener::RunL - end" );
    }

// -----------------------------------------------------------------------------
// CListener::RunError
// Standard active object error function. From class CActive.
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CListener::RunError( TInt aError )
#else
TInt CListener::RunError( TInt /*aError*/ )
#endif
    {
    WRITELOG1( "CListener::RunError, aError: %d",  aError );
    
    Cancel();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CListener::DoCancel
// Standard active object cancellation function. From class CActive.
// -----------------------------------------------------------------------------
//
void CListener::DoCancel()
    {
    WRITELOG( "CListener::DoCancel" );
    
    iProperty.Cancel();	
    }

// -----------------------------------------------------------------------------
// CListener::StartListeningL
// Then start listening of the changes in the key.
// -----------------------------------------------------------------------------
//
void CListener::StartListening()
    {
    WRITELOG( "CListener::StartListeningL - begin" ); 
    
    if( IsActive() )
        {
        return;   
        }

	iProperty.Subscribe( iStatus );
	SetActive();
 
   WRITELOG( "CListener::StartListeningL - end" );   
   }

// -----------------------------------------------------------------------------
// CListener::SendNotificationsL
// Send notifications to clients.
// -----------------------------------------------------------------------------
//
void CListener::SendNotificationsL( const TUint aKeyValue,
		TRequestStatus& aStatus ) const 
    {
    WRITELOG( "CListener::SendNotifications - begin" ); 
    
    // For value only.
    for ( TInt i = iArrayForKeyValue.Count(); --i >= 0; ) 
    	{
        iArrayForKeyValue[i]->NotifyKeyL(
        		aKeyValue,
        		iPropertyCategory,
        		iKey );
        }
    
    // For status and value.
    for ( TInt i = iArrayForKeyAndStatus.Count(); --i >= 0; ) 
    	{
        iArrayForKeyAndStatus[i]->NotifyKeyAndStatusL( 
        		aKeyValue,
        		aStatus,
        		iPropertyCategory,
        		iKey );
        }

    WRITELOG( "CListener::SendNotifications - end" );   
    }

// End of file.
