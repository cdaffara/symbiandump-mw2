/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings API
*
*/


#include <e32property.h>
#include "spnotifychange.h"
#include "spdefaultvalues.h"
#include "mspnotifychangeobserver.h"
#include "spsapilogger.h" // For logging
#include "spsbufferedpublisher.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPNotifyChange::CSPNotifyChange( MSPNotifyChangeObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iObserver( aObserver )
    {
    XSPSLOGSTRING( "CSPNotifyChange::CSPNotifyChange() - IN" );
    
    CActiveScheduler::Add(this);
    
    XSPSLOGSTRING( "CSPNotifyChange::CSPNotifyChange() - OUT" );
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSPNotifyChange::ConstructL()
    {
    XSPSLOGSTRING( "CSPNotifyChange::ConstructL() - IN" );
    
    iData = CSpsBufferedPublisher::NewL( KUidSystemCategory, KSPNotifyChangeKey );
    
    User::LeaveIfError( iProperty.Attach( KUidSystemCategory, KSPNotifyChangeKey ) );
    
    XSPSLOGSTRING( "CSPNotifyChange::ConstructL() - OUT" );
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSPNotifyChange* CSPNotifyChange::NewL( MSPNotifyChangeObserver& aObserver )
    {
    XSPSLOGSTRING( "CSPNotifyChange::NewL() - IN" );
    
    CSPNotifyChange* self = new (ELeave) CSPNotifyChange( aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    XSPSLOGSTRING( "CSPNotifyChange::NewL() - OUT" );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPNotifyChange::~CSPNotifyChange()
    {
    XSPSLOGSTRING( "CSPNotifyChange::~CSPNotifyChange() - IN" );
    delete iData;
    Cancel();
    iIdArray.Reset();   
    iIdArray.Close();
    iProperty.Close();
    
    XSPSLOGSTRING( "CSPNotifyChange::~CSPNotifyChange() - OUT" );
    }

// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CSPNotifyChange::DoCancel()
    {
    XSPSLOGSTRING( "CSPNotifyChange::DoCancel() - IN" );
    iData->Stop();
    iProperty.Cancel();
    
    XSPSLOGSTRING( "CSPNotifyChange::DoCancel() - OUT" );
    }

// ---------------------------------------------------------------------------
// From class CActive.
// Handle registration event
// ---------------------------------------------------------------------------
//
void CSPNotifyChange::RunL()
    {
    XSPSLOGSTRING( "CSPNotifyChange::RunL() - IN" );
    
    User::LeaveIfError( iStatus.Int() );
    Subscribe();
    RArray<TUint32> array;
    iData->GetL( array );
    
    TInt count = iIdArray.Count();
    while( array.Count() )
        {
        for( TInt i = 0; i < count; i++ )
            {
            if( array[0] == iIdArray[i] )
                {
                iObserver.HandleNotifyChange( array[0] );
                break;
                }
            }
        if( count == 0 )
            {
            XSPSLOGSTRING2( 
                "CSPNotifyChange::RunL() - HandleNotifyChange(%i)", array[0] );
            iObserver.HandleNotifyChange( array[0] );
            }
        
        array.Remove(0);
        }

    array.Close();

    XSPSLOGSTRING( "CSPNotifyChange::RunL() - OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CSPNotifyChange::RunError( TInt aError )
    {
    XSPSLOGSTRING( "CSPNotifyChange::RunError() - IN" );
    
    iObserver.HandleError( aError );
    
    Subscribe();
    
    XSPSLOGSTRING( "CSPNotifyChange::RunError() - OUT" );
    
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CSPNotifyChange::NotifyChangeL( const RIdArray& aIdArray )
    {
    XSPSLOGSTRING( "CSPNotifyChange::NotifyChangeL() - IN" );
    
    iIdArray.Reset();
    
    TInt count = aIdArray.Count();
    
    for( TInt i = 0; i < count; i++)
        {
        User::LeaveIfError( iIdArray.Append( aIdArray[i] ) );
        }
        
    Subscribe();
        
    XSPSLOGSTRING( "CSPNotifyChange::NotifyChangeL() - OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CSPNotifyChange::NotifyChangeCancel()
    {
    XSPSLOGSTRING( "CSPNotifyChange::NotifyChangeCancel() - IN" );
    
    Cancel();
    
    XSPSLOGSTRING( "CSPNotifyChange::NotifyChangeCancel() - OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSPNotifyChange::Subscribe()
    {
    if( !IsActive() )
        {
        iData->Start();
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    }
