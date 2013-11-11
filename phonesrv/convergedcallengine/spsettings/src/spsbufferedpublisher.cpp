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
* Description: Logic for bufferred publish and subcribe notifier
*
*/

#include <e32property.h>
#include "spdefaultvalues.h"

#include "spsbufferedpublisher.h"

const TUint32 KRealGlobalIndex = 0;
_LIT( KSemaphore, "CSpsBufferedPublisher" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
CSpsBufferedPublisher* CSpsBufferedPublisher::NewL( TUid aCategory, TUint aKey )
    {
    CSpsBufferedPublisher* self = new (ELeave) CSpsBufferedPublisher( aCategory, aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
CSpsBufferedPublisher::CSpsBufferedPublisher( TUid aCategory, TUint aKey ):
    iCategory( aCategory ),
    iKey( aKey ),
    iIndex( KErrNotReady )
    {

    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
CSpsBufferedPublisher::~CSpsBufferedPublisher()
    {

    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::ConstructL()
    {
    static _LIT_SECURITY_POLICY_PASS( KAllowAllPolicy );

    TInt err = RProperty::Define( 
            iCategory, iKey, 
            RProperty::EByteArray,
            KAllowAllPolicy, KAllowAllPolicy, 
            RProperty::KMaxPropertySize );
    
    if( !err )
        {
        TBuf8<RProperty::KMaxPropertySize> data;
        data.FillZ( data.MaxLength() );
        User::LeaveIfError( RProperty::Set( iCategory, iKey, data ) );
        }
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::SetL( TUid aCategory, TUint aKey, TUint32 aData )
    {
    RSemaphore wait;
    TInt err = wait.OpenGlobal( KSemaphore );
     
    if ( err )
        {
        User::LeaveIfError( wait.CreateGlobal( KSemaphore, 1 ) );
        }
    
    wait.Wait();
    TCleanupItem cleanup( CSpsBufferedPublisher::SignalAndCloseSemaphore, &wait );
    CleanupStack::PushL( cleanup );
    
    TBuf8<RProperty::KMaxPropertySize> data;
    User::LeaveIfError(
        RProperty::Get( aCategory, aKey, data ) );

    // Increment index
    GlobalIndex( data )++;

    if( Base( data, GlobalIndex( data ) ) == GlobalIndex( data ) )
        {
        // Skip index if real index is same as KRealGlobalIndex
        GlobalIndex( data )++;
        }
        
    // Write data to index
    At( data, GlobalIndex( data ) ) = aData;
    
    User::LeaveIfError(
        RProperty::Set( aCategory, aKey, data ) );
    
    CleanupStack::PopAndDestroy(); // wait
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::GetL( RArray<TUint32>& aData )
    {
    User::LeaveIfError( iIndex );
    
    TBuf8<RProperty::KMaxPropertySize> data;
    User::LeaveIfError(
            RProperty::Get( iCategory, iKey, data ) );
    
    const TUint32 lastModified = GlobalIndex( data );
    const TUint32 indexDifference = lastModified - iIndex;
    if( Max( data ) < indexDifference )
        {
        // If some of data is missed
        iIndex = lastModified - Max( data );
        }
    
    // Start reading
    while( lastModified != iIndex ) 
        {
        iIndex++;
        
        if( Base( data, iIndex ) == iIndex )
            {
            // Skip index if real index is same as KRealGlobalIndex
            iIndex++;
            }
        aData.AppendL( At(data, iIndex) );
        }

    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::Start()
    {
    if( KErrNone > iIndex )
        {
        TBuf8<RProperty::KMaxPropertySize> data;
        RProperty::Get( iCategory, iKey, data );
        if( data.Length() )
            {
            // When no error occurs
            iIndex = GlobalIndex( data );
            }
        else
            {
            iIndex = 0;
            }
        }

    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::Stop()
    {
    iIndex = KErrNotReady;
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
void CSpsBufferedPublisher::SignalAndCloseSemaphore( TAny* aPtr )
    {
    RSemaphore* wait = (RSemaphore*)aPtr;
    wait->Signal();
    wait->Close();
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
TUint32& CSpsBufferedPublisher::At( TDes8& aData, TInt aIndex )
    {
    const TUint32 baseIndex = Base( aData, aIndex );
    TUint32 realIndex = aIndex - baseIndex;
    ASSERT( KRealGlobalIndex != realIndex ); // Check if global index is not used
    return (TUint32&)aData[ realIndex * sizeof(TUint32) ];
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
TUint CSpsBufferedPublisher::Max( TDesC8& aData )
    {
    return ( aData.Length() / sizeof(TUint32) );
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
TInt32 CSpsBufferedPublisher::Base( TDesC8& aData, TInt aIndex )
    {
    const TInt max = Max( aData );
    return ( max * (TUint32)( aIndex / max ) );
    }

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//
TUint32& CSpsBufferedPublisher::GlobalIndex( TDes8& aData )
    {
    return (TUint32&)aData[ KRealGlobalIndex ];
    }
