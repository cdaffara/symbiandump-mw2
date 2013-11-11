/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Global functions for the landmarks server
*
*
*/


#include "EPos_LmServerGlobal.h"
#include "EPos_LandmarksServerPanics.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// ---------------------------------------------------------
//
TInt LmServerGlobal::Read(
    const RMessagePtr2& aMessagePtr,
    TInt aParam,
    TDes8& aDes,
    TInt aOffset)
    {
    TInt ret = aMessagePtr.Read(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        PanicClient(aMessagePtr, EPosUnableToReadOrWriteDataToClient);
        }
    return ret;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
TInt LmServerGlobal::Read(
    const RMessagePtr2& aMessagePtr,
    TInt aParam,
    TDes16& aDes,
    TInt aOffset)
    {
    TInt ret = aMessagePtr.Read(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        PanicClient(aMessagePtr, EPosUnableToReadOrWriteDataToClient);
        }
    return ret;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
TInt LmServerGlobal::Write(
    const RMessagePtr2& aMessagePtr,
    TInt aParam,
    const TDesC8& aDes,
    TInt aOffset)
    {
    TInt ret = aMessagePtr.Write(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        PanicClient(aMessagePtr, EPosUnableToReadOrWriteDataToClient);
        }
    return ret;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
TInt LmServerGlobal::Write(
    const RMessagePtr2& aMessagePtr,
    TInt aParam,
    const TDesC16& aDes,
    TInt aOffset)
    {
    TInt ret = aMessagePtr.Write(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        PanicClient(aMessagePtr, EPosUnableToReadOrWriteDataToClient);
        }
    return ret;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void LmServerGlobal::Complete(
    const RMessagePtr2& aMessagePtr,
    TInt aReason)
    {
    if (!aMessagePtr.IsNull())
        {
        aMessagePtr.Complete(aReason);
        }
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC8* LmServerGlobal::CopyClientBuffer8LC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam )
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = aMessage.GetDesLengthL( aClientBufferParam );
    HBufC8* buffer = HBufC8::NewLC( dataSize );

    // Populate server side buffer with client data
    TPtr8 ptrToBuf = buffer->Des();
    aMessage.ReadL( aClientBufferParam, ptrToBuf );
    return buffer;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC* LmServerGlobal::CopyClientBufferLC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = aMessage.GetDesLengthL( aClientBufferParam );
    HBufC* buffer = HBufC::NewLC( dataSize );

    // Populate server side buffer with client data
    TPtr ptrToBuf = buffer->Des();
    aMessage.ReadL( aClientBufferParam, ptrToBuf );
    return buffer;
    }

//  End of File
