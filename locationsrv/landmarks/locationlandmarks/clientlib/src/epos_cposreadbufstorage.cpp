/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosReadBufStorage class implementation
*
*/


#include "epos_cposreadbufstorage.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPosReadBufStorage::CPosReadBufStorage( const TDesC8& aBuffer ) 
    : iBuffer( aBuffer ) 
    {
    iPointer = iBuffer.Ptr();
    iBoundary = iPointer + iBuffer.Size();
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPosReadBufStorage* CPosReadBufStorage::NewLC( const TDesC8& aBuffer ) 
    {
    CPosReadBufStorage* self = new ( ELeave ) CPosReadBufStorage( aBuffer );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Copies aSize bytes from internal buffer to aTrg address
// Leaves if buffer is shorter than requested amount of data.
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TUint8* aTrg, TUint aSize ) 
    {
    if ( iPointer + aSize <= iBoundary ) 
        {
        memcpy( aTrg, iPointer, aSize );
        iPointer += aSize;
        }
    else
        {
        User::Leave( KErrCorrupt );
        }
    }

// ---------------------------------------------------------------------------
// Copies TInt from internal buffer
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TInt& aData ) 
    {
    GetL( (TUint8*) &aData, sizeof( TInt ) );
    }

// ---------------------------------------------------------------------------
// Copies TUint32 from internal buffer
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TUint32& aData ) 
  {
  GetL( (TUint8*) &aData, sizeof( TUint32 ) );
  }

// ---------------------------------------------------------------------------
// Copies TUint16 from internal buffer
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TUint16& aData ) 
  {
  GetL( (TUint8*) &aData, sizeof( TUint16 ) );
  }

// ---------------------------------------------------------------------------
// Copies descriptor from internal buffer to aData buffer.
// Size of the data to be copied is read from the buffer.
// Leaves if target descriptor cannot accept descriptor in buffer.
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TDes8& aData ) 
  {
  TInt size;
  GetL( size );
  if ( size >= 0 && 
     aData.MaxSize() >= size &&
     iPointer + size <= iBoundary ) 
    {
    aData = TPtrC8( (TUint8*) iPointer, size );
    iPointer += size;
    }
  else
    {
    User::Leave( KErrCorrupt );
    }
  }

// ---------------------------------------------------------------------------
// Sets passed descriptor to data from internal buffer.
// Size and length of the data are read from the buffer.
// ---------------------------------------------------------------------------
//
void CPosReadBufStorage::GetL( TPtrC& aData ) 
  {
  TInt size, length;
  GetL( length );
  GetL( size );
  if ( size >= 0 && length >= 0 && size >= length && iPointer + size <= iBoundary ) 
    {
    aData.Set( (TUint16*) iPointer, length );
    iPointer += size;  
    }
  else
    {
    User::Leave( KErrCorrupt );
    }
  }
