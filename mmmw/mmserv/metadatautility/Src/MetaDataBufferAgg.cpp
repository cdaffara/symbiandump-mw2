/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object for buffer aggregation
*
*/


// INCLUDE FILES
#include "MetaDataUtilityBody.h"
#include "MDUChunkDataObserver.h"
#include "MetaDataBufferAgg.h"

// CONSTANTS
// (ID3v2 specification found in www.id3.org)
// ID3v2 header consists of following parts:
// - identifier "ID3", 3 bytes
// - version, 2 bytes
// - flags, 1 byte
// - data length, 4 bytes
const TInt KID3v2HeaderLength           = 10;
const TInt KID3v2TagLength              = 3;

_LIT8( KID3v2Tag, "ID3" );              // ID3 metadata format indication



// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::MetaDataBufferAgg
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataBufferAgg::CMetaDataBufferAgg( CMetaDataUtilityBody* aBody,
        MMDUChunkDataObserver& aObserver )  
: CActive( CActive::EPriorityStandard ), 
  iBody( aBody ),
  iObserver( aObserver ),
  iState( EFindIdentifier ),
  iUserChunk( NULL ),
  iFinalChunk( EFalse ),
  iMetaDataSize( 0 )  
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Two-Phase Constructor
// ---------------------------------------------------------------------------
//
CMetaDataBufferAgg* CMetaDataBufferAgg::NewL( CMetaDataUtilityBody* aBody,
        TDesC8& aMimeType, 
        MMDUChunkDataObserver& aObserver  )
    {
    CMetaDataBufferAgg* self = new(ELeave) CMetaDataBufferAgg( aBody, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aMimeType );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::Construct()
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::ConstructL( const TDesC8& aMimeType )
    {
    // copy MimeType
    iMimeType.CreateL( aMimeType.Size() );    
    iMimeType = aMimeType;
    
    // allocate the buffers
    iPrevBuf.CreateL( 0 );
    iMetaDataBuf.CreateL ( 0 );
    
    }

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::~CMetaDataBufferAgg()
// -----------------------------------------------------------------------------
//
CMetaDataBufferAgg::~CMetaDataBufferAgg()
    {
    iMimeType.Close();
    iPrevBuf.Close();
    iMetaDataBuf.Close();
    Cancel();
    }

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::RequestProcessChunkData
// -----------------------------------------------------------------------------
//
TInt CMetaDataBufferAgg::RequestProcessChunkData( TDesC8& aChunk, 
        TBool aFinalChunk )
    {
    if (iState == EParseComplete )
        {
        return KErrAlreadyExists;
        }
    if (iState == EParseData )
        {
        return KErrCompletion;
        }
    if ( IsActive() )
        {
        return KErrNotReady;
        }
    
    iUserChunk = &aChunk;
    iFinalChunk = aFinalChunk;

    iStatus = KRequestPending;
    SetActive();  

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::RequestParseChunkData
// -----------------------------------------------------------------------------
//
TInt CMetaDataBufferAgg::RequestParseChunkData()
    {
    if (iState == EParseComplete )
        {
        return KErrAlreadyExists;
        }
    if ( IsActive() || iState != EParseData)
        {
        return KErrNotReady;
        }

    
    iStatus = KRequestPending;
    SetActive();  

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::ReadyToGo
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::ReadyToGo()
{
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
}

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::DoCancel
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::DoCancel()
    {
    }
    
// -----------------------------------------------------------------------------
// CMetaDataUtility::RunL
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::RunL()
    {

    switch( iState )
        {
        case EFindIdentifier:
              {
              TRAPD( err, FindMetaDataIdentifierL( iUserChunk ));
              DoCallback( err );
              break;
              }
        case ECalculateSize:
              {
              TRAPD( err, CalculateMetaDataSizeL( iUserChunk ));
              DoCallback( err );
              break;
              }
        case ECollectData:
             {
             TRAPD( err, CollectMetaDataL( iUserChunk ));  
             DoCallback( err );
             break;
             }
        case EParseData:
             {
             TRAPD( err, iBody->DoParseChunkDataL( iMetaDataBuf, iMimeType )); 
             iState = EParseComplete;
             DoCallback( err );
             break;
             }
        default:
             break;
        }
      }


// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::DoCallback
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::DoCallback( TInt aError)
    {
    if ( iFinalChunk )
        { 
        switch (iState)
            {            
            case EFindIdentifier:
            case ECalculateSize:
                {
                iObserver.HandleChunkDataProcessed( KErrNotFound );                                              
                break;
                }
            case ECollectData:
            case EParseData:
                { 
                iState = EParseData;
                iObserver.HandleChunkDataReadyToBeParsed();
                break;
                }
            case EParseComplete:
                {
                iObserver.HandleChunkDataComplete( aError );                                              
                break;
                }
            }
        }
    else
        {
        switch (iState)
            {            
            case EFindIdentifier:
            case ECalculateSize:
            case ECollectData:
                {
                iObserver.HandleChunkDataProcessed( KErrNone );                                               
                break;
                }
            case EParseData:
                { 
                iObserver.HandleChunkDataReadyToBeParsed();
                break;
                }
            case EParseComplete:
                {
                iObserver.HandleChunkDataComplete( aError );                                              
                break;
                }
            }        
        }
        
    }


// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::FindMetaDataIdentifier
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::FindMetaDataIdentifierL( TDesC8* aBuf )
    {
    // search from client buffer first
    TInt offset = aBuf->Find( KID3v2Tag );
    if ( offset != KErrNotFound )
        {
        // Identifier is found, copy to iMetaDataBuf
        TUint8* ptr = const_cast<TUint8*>(aBuf->Ptr());
        CopyBufferL( &iMetaDataBuf, ptr+offset, aBuf->Size() - offset );
                      
        ComputeSize( &iMetaDataBuf, 0 );

        if ( !iMetaDataSize )
            {
            iState = ECalculateSize;              
            }
        else
            {
            if ( iMetaDataBuf.Size() >= iMetaDataSize )
                {
                iState = EParseData;      
                }  
            else
                {
                iState = ECollectData;                              
                }
            }       
        }
    else
        {
        // Identifier is NOT found, need to process the previous data
         ProcessPreviousChunkL( aBuf );
        }
    }
    
// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::FindMetaDataIdentifier
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::ProcessPreviousChunkL( TDesC8* aBuf )
    {
    TInt prevSize = iPrevBuf.Size();   
    TInt bufSize = aBuf->Size();
    TInt sizeToCopy = 0;
    
    //If there is not enough data for 2*KID3v2TagLength
    //The following lists the posibility of the iPrevBuf content:
    //      . . . . . .
    //      . . . . . I
    //      . . . . I D
    //      . . I D 3 .
    //  or it contains "ID3" at different byte
    if ( prevSize + bufSize < 2*KID3v2TagLength )
        {
        TUint8* ptr = const_cast<TUint8*> (aBuf->Ptr());
        
        // just do the append
        AppendBufferL( &iPrevBuf, ptr, bufSize );
        return;
        }
    else
        {
        sizeToCopy = 2*KID3v2TagLength - prevSize;
        }
       
    //Add into the previous buf (iPrevBuf)
    TUint8* ptr = const_cast<TUint8*> (aBuf->Ptr());
    AppendBufferL( &iPrevBuf, ptr, sizeToCopy );    
    
    // search from previous buffer 
    TInt offset = iPrevBuf.Find( KID3v2Tag );
    
    // tag IS found
    if ( offset != KErrNotFound )
        {
        //Found identifier, save previous buf data into iMetaDataBuf
        ptr = const_cast<TUint8*> (iPrevBuf.Ptr());
        AppendBufferL( &iMetaDataBuf, ptr+offset, iPrevBuf.Size() - offset );
        
        //Append the rest of aBuf
        ptr = const_cast<TUint8*> (aBuf->Ptr());
        AppendBufferL( &iMetaDataBuf, ptr+sizeToCopy, aBuf->Size() - sizeToCopy );
                
        ComputeSize( &iMetaDataBuf, 0 );  
        if ( !iMetaDataSize )
            {
            iState = ECalculateSize;              
            }
        else
            {
            if ( iMetaDataBuf.Size() >= iMetaDataSize )
                {
                iState = EParseData;      
                }  
            else
                {
                iState = ECollectData;                              
                }
            }     
        }
    
    // tag NOT found
    else
        {
        //Re-populate iPrevBuf 
        if ( aBuf->Size() < KID3v2TagLength )
            {
            //Append the rest of aBuf into iPrevBuf
            ptr = const_cast<TUint8*> (aBuf->Ptr());        
            AppendBufferL( &iPrevBuf, ptr+sizeToCopy, aBuf->Size() - sizeToCopy );
            
            //copy the last "KID3v2TagLength" size of aBuf into iPrevBuf
            iPrevBuf = iPrevBuf.Mid(iPrevBuf.Size() - KID3v2TagLength);
            }
        else
            {
            //copy the last "KID3v2TagLength" size of aBuf into iPrevBuf
            offset = aBuf->Size() - KID3v2TagLength;
            TUint8* ptr = const_cast<TUint8*>(aBuf->Ptr());
            CopyBufferL( &iPrevBuf, ptr+offset, KID3v2TagLength );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::CalculateMetaDataSize
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::CalculateMetaDataSizeL( TDesC8* aBuf )
    {
    TUint8* ptr = const_cast<TUint8*> (aBuf->Ptr());
    AppendBufferL( &iMetaDataBuf, ptr, aBuf->Size() );    

    // Try to compute the metadata size
    ComputeSize( &iMetaDataBuf, 0 );

    // check whether metadata size is available ( non zero)
    if ( iMetaDataSize )
        {
        iState = ECollectData;              
        }
    
    if ( iMetaDataBuf.Size() >= iMetaDataSize )
        {
        iState = EParseData;      
        }
    }   

// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::ComputeSize
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::ComputeSize( TDesC8* aBuf, TInt aOffset )
    {
    if ( aBuf->Size() < ( aOffset + KID3v2HeaderLength ) )
        {
        iMetaDataSize = 0;
        return;
        }
            
    // calculate the size
    for ( TInt i = 6; i < 10; i++ )
        {
        iMetaDataSize <<= 7;
        iMetaDataSize |= (*aBuf)[i+aOffset] & 0x7f;
        }
    iMetaDataSize += KID3v2HeaderLength; // add the header for total size
    
    }   
   
// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::CollectMetaData
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::CollectMetaDataL( TDesC8* aBuf )
    {
    TUint8* ptr = const_cast<TUint8*> (aBuf->Ptr());
    AppendBufferL( &iMetaDataBuf, ptr, aBuf->Size() );            
    
    if ( iMetaDataBuf.Size() >= iMetaDataSize )
        {
        iState = EParseData;      
        }
     }


// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::CopyBufferL
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::CopyBufferL(RBuf8* aDes, TUint8* aPtr, TInt aSize )
    {
     if ( aDes->MaxLength() < aSize)
        {
        aDes->ReAllocL( aSize );
        }
     aDes->Copy( aPtr, aSize );
    }


// -----------------------------------------------------------------------------
// CMetaDataBufferAgg::AppendBufferL
// -----------------------------------------------------------------------------
//
void CMetaDataBufferAgg::AppendBufferL(RBuf8* aDes, TUint8* aPtr, TInt aSize )
    {
    TInt size = aDes->Size();
    if ( aDes->MaxLength() < aSize+size)
        {
        aDes->ReAllocL( aSize+size );
        }
     aDes->Append( aPtr, aSize );
    }


