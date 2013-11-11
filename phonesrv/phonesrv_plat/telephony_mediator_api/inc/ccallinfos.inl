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
* Description:  Inline implementation
*
*/


#include <s32mem.h>


// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CCallInfos::CCallInfos()
    {    
    }

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos* CCallInfos::NewL()
    {
    return new( ELeave )CCallInfos();
    }

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos* CCallInfos::NewLC()
    {
    CCallInfos* self = new( ELeave )CCallInfos();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Desctructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos::~CCallInfos()
    {    
    iCallInfos.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// Desctructor.
// -----------------------------------------------------------------------------
//
inline void  CCallInfos::AddL( TCallInfo& aCallInfo )
    {
    
    //take heap based copy and add it to the array
    TCallInfo* copy = new( ELeave )TCallInfo;    
    
    *copy = aCallInfo;    
    CleanupStack::PushL( copy );
    
    iCallInfos.AppendL( copy );    
    CleanupStack::Pop( copy );
    }


// -----------------------------------------------------------------------------
// Returns information about all calls.
// -----------------------------------------------------------------------------
//
inline const RPointerArray<TCallInfo>& CCallInfos::CallInfos() const
    {
    return iCallInfos;
    }

// -----------------------------------------------------------------------------
// Packages this class for IPC transfer.
// -----------------------------------------------------------------------------
//
inline HBufC8*  CCallInfos::ExternalizeL( ) const
    {    
    const TInt callInfoCount( iCallInfos.Count() );
    HBufC8* data = HBufC8::NewLC( callInfoCount*sizeof(TCallInfo)+sizeof(TInt) );            
    TPtr8 dataPtr = data->Des();
    
    RDesWriteStream stream( dataPtr );
    CleanupClosePushL( stream );
    stream.WriteInt32L( callInfoCount );
            
    for ( TInt i = 0; i < callInfoCount; i++ )
        {    
        TPckgBuf<TCallInfo> packedInfo( *iCallInfos[i] );    
        stream.WriteL( packedInfo );
        }
    
    CleanupStack::PopAndDestroy( 1 ); // stream
    CleanupStack::Pop( data );
    return data;    
    }

// -----------------------------------------------------------------------------
// Intializes the class from descriptor.
// -----------------------------------------------------------------------------
//
inline void CCallInfos::InternalizeL( const TDesC8& aCallInfos ) 
    {        
    RDesReadStream stream( aCallInfos );    
    CleanupClosePushL( stream );
            
    const TInt callInfoCount( stream.ReadInt32L() );        
    iCallInfos.Reset();    
            
    for ( TInt i = 0; i < callInfoCount; i++ )
        {
        TPckgBuf<TCallInfo> packedInfo;        
        stream.ReadL( packedInfo );           
        
        //take heap based copy and add it to the array
        TCallInfo* copy = new( ELeave )TCallInfo;
        TCallInfo orig = packedInfo();        
        *copy = orig;        
        CleanupStack::PushL( copy );  
        iCallInfos.AppendL( copy );        
        CleanupStack::Pop( copy );
        }
    
    CleanupStack::PopAndDestroy(); // stream    
    }




 

