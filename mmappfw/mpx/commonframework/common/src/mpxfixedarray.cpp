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
* Description:  Implementation of array of fixed size elements on global chunk
*
*/

#include "mpxfixedarray.h"
#include "mpxheapmanager.h"


// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::RMPXFixedArrayBase
// -----------------------------------------------------------------------------
//
RMPXFixedArrayBase::RMPXFixedArrayBase()
    {    
    }

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::RMPXFixedArrayBase
// -----------------------------------------------------------------------------
//
RMPXFixedArrayBase::RMPXFixedArrayBase(const RMPXFixedArrayBase& aArray)
     : iCount(0),iDataOffset(0),iElementSize(aArray.iElementSize)
    {     
    (void)Copy(RMPXHeapManager::ClientHandle(),aArray);
    }
    
// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Set
// -----------------------------------------------------------------------------
//
TInt RMPXFixedArrayBase::Copy(TUint aClientHandle,const RMPXFixedArrayBase& aArray)
    {  
    MPX_ASSERT(aArray.iElementSize&&aArray.iCount&&aArray.iDataOffset&&aClientHandle);  
    // 
    Reset(aClientHandle); // Clears any current values
    TInt size=aArray.iElementSize*aArray.iCount;
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    TInt r=m.Copy(aClientHandle,aArray.iDataOffset,size);    
    if (r>0) // Its an offset
        {
        iDataOffset=r;
        iCount=aArray.iCount;
        iElementSize=aArray.iElementSize; 
        }
    // else the members are zero from Reset()
    return iDataOffset?KErrNone:KErrNoMemory; 
    }    

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::RMPXFixedArrayBase
// -----------------------------------------------------------------------------
//
RMPXFixedArrayBase::RMPXFixedArrayBase(TInt aElementSize)
    : iCount(0),iDataOffset(0),iElementSize(aElementSize)
    {    
    }

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Reset
// -----------------------------------------------------------------------------
//
void RMPXFixedArrayBase::Reset(TUint aClientHandle)
    {
    if (iDataOffset)
        {
        TAny* buf=RMPXHeapManager::Ptr<TAny>(aClientHandle,iDataOffset);
        RMPXHeapManager::HeapManager(aClientHandle).Free(aClientHandle,buf);
        iDataOffset=0;
        iCount=0;
        }
    }

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Alloc
// -----------------------------------------------------------------------------
//
TAny* RMPXFixedArrayBase::Alloc(TUint aClientHandle,TInt aCount)
    {
    Reset(aClientHandle); // Clear previous values
    TInt size=iElementSize*aCount;
    TAny* ptr=RMPXHeapManager::HeapManager(aClientHandle).Alloc(aClientHandle,size);
    if (ptr)
        {
        iDataOffset=RMPXHeapManager::Offset(aClientHandle,ptr);
        iCount=aCount;
        }    
    return ptr;
    }

// End of file
